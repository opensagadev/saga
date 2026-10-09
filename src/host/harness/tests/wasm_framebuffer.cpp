#include <GLES3/gl3.h>
#include <emscripten/html5_webgl.h>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "host/platform/wasm/graphics.hpp"

extern "C" {
    GLuint g_earlyColorFramebuffer = 0;
}
i32 g_backingWidth = 16;
i32 g_backingHeight = 16;

static void require(bool ok, const char *message) {
    if (!ok) {
        std::printf("FAIL: %s\n", message);
        std::abort();
    }
}

static GLuint compile_shader(GLenum type, const char *source) {
    const GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    GLint compiled = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    require(compiled == GL_TRUE, "state fixture shader");
    return shader;
}

int main() {
    EmscriptenWebGLContextAttributes attr;
    emscripten_webgl_init_context_attributes(&attr);
    attr.majorVersion = 2;
    attr.explicitSwapControl = true;
    attr.proxyContextToMainThread = EMSCRIPTEN_WEBGL_CONTEXT_PROXY_ALWAYS;
    attr.renderViaOffscreenBackBuffer = true;
    auto context = emscripten_webgl_create_context("#canvas", &attr);
    require(context > 0 && emscripten_webgl_make_context_current(context) == EMSCRIPTEN_RESULT_SUCCESS,
            "WebGL2 context");
    GLint width, height;
    emscripten_webgl_get_drawing_buffer_size(context, &width, &height);
    GLuint texture, buffer, vao;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 16, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glGenFramebuffers(1, &g_earlyColorFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, g_earlyColorFramebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    require(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "source framebuffer");
    glClearColor(1, 0, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    GLfloat vertices[8] = {};
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, false, 0, nullptr);
    const GLuint program = glCreateProgram();
    glAttachShader(program,
                   compile_shader(GL_VERTEX_SHADER,
                                  "#version 300 es\nin vec2 position; void main(){gl_Position=vec4(position,0,1);}"));
    glAttachShader(
        program,
        compile_shader(GL_FRAGMENT_SHADER,
                       "#version 300 es\nprecision mediump float; out vec4 color; void main(){color=vec4(1);}"));
    glLinkProgram(program);
    GLint linked = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    require(linked == GL_TRUE, "state fixture program");
    glUseProgram(program);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_FRONT_AND_BACK);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_SCISSOR_TEST);
    glScissor(0, 0, 1, 1);
    glColorMask(false, false, false, false);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, g_earlyColorFramebuffer);
    for (int frame = 0; frame < 64; ++frame) {
        // Change the source every frame so a stale backbuffer cannot pass.
        const bool magenta = (frame & 1) == 0;
        glDisable(GL_SCISSOR_TEST);
        glColorMask(true, true, true, true);
        glClearColor(magenta ? 1 : 0, magenta ? 0 : 1, magenta ? 1 : 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        glColorMask(false, false, false, false);
        const bool scissor_enabled = (frame & 2) == 0;
        if (scissor_enabled)
            glEnable(GL_SCISSOR_TEST);
        HostPresentWasmFramebuffer(width, height);
        require(glGetError() == GL_NO_ERROR, "presentation GL error");
        GLint value;
        glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &value);
        require(value == 0, "read framebuffer restored");
        glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &value);
        require(value == int(g_earlyColorFramebuffer), "draw framebuffer restored");
        glGetIntegerv(GL_ACTIVE_TEXTURE, &value);
        require(value == GL_TEXTURE3, "active texture preserved");
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &value);
        require(value == int(texture), "texture preserved");
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &value);
        require(value == int(buffer), "vertex buffer preserved");
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &value);
        require(value == int(vao), "vertex array preserved");
        glGetIntegerv(GL_CURRENT_PROGRAM, &value);
        require(value == int(program), "shader program preserved");
        require(bool(glIsEnabled(GL_SCISSOR_TEST)) == scissor_enabled && glIsEnabled(GL_CULL_FACE) &&
                    glIsEnabled(GL_DEPTH_TEST) && glIsEnabled(GL_BLEND),
                "render enables preserved");
        GLboolean mask[4];
        glGetBooleanv(GL_COLOR_WRITEMASK, mask);
        require(!mask[0] && !mask[1] && !mask[2] && !mask[3], "color mask preserved");
        std::vector<unsigned char> pixels(width * height * 4);
        glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
        require(glGetError() == GL_NO_ERROR, "backbuffer readback");
        for (size_t i = 0; i < pixels.size(); i += 4)
            require(pixels[i] == (magenta ? 255 : 0) && pixels[i + 1] == (magenta ? 0 : 255) &&
                        pixels[i + 2] == (magenta ? 255 : 0) && pixels[i + 3] == 255,
                    "complete scaled image despite scissor/cull/color mask");
    }
    std::puts("PASS: 64 full framebuffer copies, pixels and renderer GL state preserved");
    return 0;
}
