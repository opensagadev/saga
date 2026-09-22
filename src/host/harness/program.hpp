#pragma once

#include <cstddef>
#include <string_view>

namespace saga::host::harness {

    class Arguments final {
      public:
        constexpr Arguments() = default;
        constexpr Arguments(int count, char *const *values) : count{count}, values{values} {
        }

        [[nodiscard]] constexpr bool empty() const noexcept {
            return this->count == 0;
        }

        [[nodiscard]] constexpr std::size_t size() const noexcept {
            return static_cast<std::size_t>(this->count);
        }

        [[nodiscard]] std::string_view operator[](std::size_t index) const noexcept {
            return this->values[index];
        }

        [[nodiscard]] constexpr Arguments drop(std::size_t count) const noexcept {
            const auto dropped = count < size() ? count : size();
            return {this->count - static_cast<int>(dropped), this->values ? this->values + dropped : nullptr};
        }

      private:
        int count = 0;
        char *const *values = nullptr;
    };

    struct Invocation final {
        std::string_view executable;
        Arguments arguments;
    };

    using ProgramMain = int (*)(const Invocation &);

    struct Program final {
        std::string_view name;
        std::string_view description;
        ProgramMain run;
    };

} // namespace saga::host::harness
