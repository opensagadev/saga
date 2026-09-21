#include <clang-c/Index.h>

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

namespace {

    class ClangString {
      public:
        explicit ClangString(CXString value) : value_{value} {
        }
        ~ClangString() {
            clang_disposeString(this->value_);
        }

        ClangString(const ClangString &) = delete;
        ClangString &operator=(const ClangString &) = delete;

        [[nodiscard]] std::string str() const {
            const auto *value = clang_getCString(this->value_);
            return value != nullptr ? value : "";
        }

      private:
        CXString value_;
    };

    struct FunctionDeclaration {
        std::string path;
        unsigned line{};
        unsigned column{};
        bool is_definition{};
        std::string name;
        std::string semantic_identity;
        std::string symbol_usr;
        std::string canonical_type;
        std::string structural_type;

        [[nodiscard]] auto fields() const {
            return std::tie(this->path, this->line, this->column, this->is_definition, this->name,
                            this->semantic_identity, this->symbol_usr, this->canonical_type, this->structural_type);
        }

        [[nodiscard]] bool operator<(const FunctionDeclaration &other) const {
            return this->fields() < other.fields();
        }
        [[nodiscard]] bool operator==(const FunctionDeclaration &other) const {
            return this->fields() == other.fields();
        }
    };

    struct VisitorContext {
        std::filesystem::path workspace;
        std::vector<FunctionDeclaration> declarations;
    };

    [[nodiscard]] std::string clean_field(std::string value) {
        std::replace_if(
            value.begin(), value.end(),
            [](char character) { return character == '\t' || character == '\n' || character == '\r'; }, ' ');
        return value;
    }

    [[nodiscard]] bool is_inside(const std::filesystem::path &path, const std::filesystem::path &directory) {
        const auto relative = path.lexically_relative(directory);
        return !relative.empty() && *relative.begin() != "..";
    }

    [[nodiscard]] std::string semantic_identity(CXCursor cursor, std::string_view name) {
        const auto parent = clang_getCursorSemanticParent(cursor);
        const auto parent_usr = ClangString{clang_getCursorUSR(parent)}.str();
        return parent_usr.empty() ? std::string{name} : parent_usr + "@" + std::string{name};
    }

    [[nodiscard]] std::string structural_type_identity(CXType original) {
        const auto type = clang_getCanonicalType(original);
        std::string result;
        if (clang_isConstQualifiedType(type)) {
            result += "const ";
        }
        if (clang_isVolatileQualifiedType(type)) {
            result += "volatile ";
        }
        if (clang_isRestrictQualifiedType(type)) {
            result += "restrict ";
        }
        result += std::to_string(type.kind);

        switch (type.kind) {
            case CXType_Bool:
                // C's _Bool and C++'s bool are the same ABI type. libclang
                // spells them according to the source language, so normalize
                // that cosmetic difference before comparing declarations.
                return result + "<bool>";

            case CXType_Pointer:
            case CXType_LValueReference:
            case CXType_RValueReference:
            case CXType_BlockPointer:
            case CXType_MemberPointer:
                return result + "<" + structural_type_identity(clang_getPointeeType(type)) + ">";

            case CXType_Record:
            case CXType_Enum: {
                const auto usr = ClangString{clang_getCursorUSR(clang_getTypeDeclaration(type))}.str();
                return result + "<" + (usr.empty() ? ClangString{clang_getTypeSpelling(type)}.str() : usr) + ">";
            }

            case CXType_FunctionProto:
            case CXType_FunctionNoProto: {
                result += "<cc=" + std::to_string(clang_getFunctionTypeCallingConv(type));
                result += ";result=" + structural_type_identity(clang_getResultType(type));
                const auto argument_count = clang_getNumArgTypes(type);
                result += ";args=" + std::to_string(argument_count);
                for (int index = 0; index < argument_count; ++index) {
                    result += ";" + structural_type_identity(clang_getArgType(type, static_cast<unsigned>(index)));
                }
                if (clang_isFunctionTypeVariadic(type)) {
                    result += ";variadic";
                }
                return result + ">";
            }

            case CXType_ConstantArray:
                return result + "<" + std::to_string(clang_getArraySize(type)) + ";" +
                       structural_type_identity(clang_getArrayElementType(type)) + ">";

            case CXType_IncompleteArray:
            case CXType_VariableArray:
            case CXType_DependentSizedArray:
                return result + "<" + structural_type_identity(clang_getArrayElementType(type)) + ">";

            case CXType_Atomic:
                return result + "<" + structural_type_identity(clang_Type_getValueType(type)) + ">";

            default:
                return result + "<" + ClangString{clang_getTypeSpelling(type)}.str() + ">";
        }
    }

    CXChildVisitResult visit(CXCursor cursor, CXCursor, CXClientData raw_context) {
        auto &context = *static_cast<VisitorContext *>(raw_context);
        if (clang_getCursorKind(cursor) != CXCursor_FunctionDecl) {
            return CXChildVisit_Recurse;
        }

        CXFile file{};
        unsigned line{};
        unsigned column{};
        unsigned offset{};
        clang_getSpellingLocation(clang_getCursorLocation(cursor), &file, &line, &column, &offset);
        if (file == nullptr) {
            return CXChildVisit_Continue;
        }

        auto path = std::filesystem::absolute(ClangString{clang_getFileName(file)}.str()).lexically_normal();
        if (!is_inside(path, context.workspace / "src")) {
            return CXChildVisit_Continue;
        }

        const auto is_definition = clang_isCursorDefinition(cursor) != 0;
        const auto cursor_type = clang_getCursorType(cursor);
        if (!is_definition && clang_getCanonicalType(cursor_type).kind == CXType_FunctionNoProto) {
            // An old-style C declaration such as `int function()` specifies no
            // parameter signature, so there is nothing meaningful to compare.
            return CXChildVisit_Continue;
        }

        auto name = ClangString{clang_getCursorSpelling(cursor)}.str();
        auto identity = semantic_identity(cursor, name);
        if (clang_getCursorLinkage(cursor) == CXLinkage_Internal) {
            identity = path.lexically_relative(context.workspace).generic_string() + "@" + name;
        }
        context.declarations.push_back({
            path.lexically_relative(context.workspace).generic_string(),
            line,
            column,
            is_definition,
            clean_field(name),
            clean_field(identity),
            clean_field(ClangString{clang_getCursorUSR(cursor)}.str()),
            clean_field(ClangString{clang_getTypeSpelling(clang_getCanonicalType(cursor_type))}.str()),
            clean_field(structural_type_identity(cursor_type)),
        });
        return CXChildVisit_Continue;
    }

} // namespace

int main(int argc, char **argv) {
    std::vector<std::string> arguments;
    for (int index = 1; index < argc; ++index) {
        const std::string_view argument{argv[index]};
        if (argument.empty() || argument.front() != '@') {
            arguments.emplace_back(argument);
            continue;
        }

        std::ifstream response_file{std::string{argument.substr(1)}};
        for (std::string line; std::getline(response_file, line);) {
            arguments.push_back(std::move(line));
        }
        if (!response_file.eof()) {
            std::cerr << "could not read response file " << argument.substr(1) << '\n';
            return EXIT_FAILURE;
        }
    }

    if (arguments.size() < 3) {
        std::cerr << "usage: collect_function_declarations OUTPUT WORKSPACE SOURCE [COMPILER_ARGUMENT ...]\n";
        return EXIT_FAILURE;
    }

    const auto output = std::filesystem::path{arguments[0]};
    VisitorContext context{std::filesystem::absolute(arguments[1]).lexically_normal(), {}};
    const auto source = std::filesystem::path{arguments[2]};
    std::vector<const char *> compiler_arguments;
    std::transform(arguments.begin() + 3, arguments.end(), std::back_inserter(compiler_arguments),
                   [](const std::string &argument) { return argument.c_str(); });

    auto *index = clang_createIndex(/*excludeDeclarationsFromPCH=*/1, /*displayDiagnostics=*/0);
    auto *unit = clang_parseTranslationUnit(index, source.c_str(), compiler_arguments.data(),
                                            static_cast<int>(compiler_arguments.size()), nullptr, 0,
                                            CXTranslationUnit_KeepGoing);
    if (unit == nullptr) {
        std::cerr << "libclang could not parse " << source << '\n';
        clang_disposeIndex(index);
        return EXIT_FAILURE;
    }

    bool has_parse_error = false;
    for (unsigned diagnostic_index = 0; diagnostic_index < clang_getNumDiagnostics(unit); ++diagnostic_index) {
        const auto diagnostic = clang_getDiagnostic(unit, diagnostic_index);
        if (clang_getDiagnosticSeverity(diagnostic) >= CXDiagnostic_Error) {
            has_parse_error = true;
            std::cerr << ClangString{clang_formatDiagnostic(diagnostic, clang_defaultDiagnosticDisplayOptions())}.str()
                      << '\n';
        }
        clang_disposeDiagnostic(diagnostic);
    }
    if (has_parse_error) {
        std::cerr << "refusing to index a translation unit with parse errors: " << source << '\n';
        clang_disposeTranslationUnit(unit);
        clang_disposeIndex(index);
        return EXIT_FAILURE;
    }

    const auto target_info = clang_getTranslationUnitTargetInfo(unit);
    const auto pointer_width = clang_TargetInfo_getPointerWidth(target_info);
    const auto target_triple = ClangString{clang_TargetInfo_getTriple(target_info)}.str();
    clang_TargetInfo_dispose(target_info);
    if (pointer_width != 32) {
        std::cerr << "refusing to index " << target_triple << " (" << pointer_width
                  << "-bit pointers); declaration checking requires a 32-bit build configuration\n";
        clang_disposeTranslationUnit(unit);
        clang_disposeIndex(index);
        return EXIT_FAILURE;
    }
    clang_visitChildren(clang_getTranslationUnitCursor(unit), visit, &context);
    std::sort(context.declarations.begin(), context.declarations.end());
    context.declarations.erase(std::unique(context.declarations.begin(), context.declarations.end()),
                               context.declarations.end());

    std::ofstream stream{output};
    for (const auto &declaration : context.declarations) {
        stream << declaration.path << '\t' << declaration.line << '\t' << declaration.column << '\t'
               << declaration.is_definition << '\t' << declaration.name << '\t' << declaration.semantic_identity << '\t'
               << declaration.symbol_usr << '\t' << declaration.canonical_type << '\t' << declaration.structural_type
               << '\n';
    }

    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);
    return stream ? EXIT_SUCCESS : EXIT_FAILURE;
}
