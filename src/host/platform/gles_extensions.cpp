#include <EGL/egl.h>
#include <GLES2/gl2.h>

// Keep the extension-loader interface while giving host pointer storage local
// names that cannot collide with the GL implementation's exported functions.
namespace {
void (*host_glGetProgramBinaryOES)(GLuint, GLsizei, GLsizei *, GLenum *, void *);
void (*host_glProgramBinaryOES)(GLuint, GLenum, const void *, GLint);
void (*host_glDiscardFramebufferEXT)(GLenum, GLsizei, const GLenum *);
void (*host_glGenVertexArraysOES)(GLsizei, GLuint *);
void (*host_glBindVertexArrayOES)(GLuint);
void (*host_glDeleteVertexArraysOES)(GLsizei, const GLuint *);
}

void NuGLES2ExtensionsInit() {
    host_glGetProgramBinaryOES = reinterpret_cast<decltype(host_glGetProgramBinaryOES)>(eglGetProcAddress("glGetProgramBinaryOES"));
    host_glProgramBinaryOES = reinterpret_cast<decltype(host_glProgramBinaryOES)>(eglGetProcAddress("glProgramBinaryOES"));
    host_glDiscardFramebufferEXT = reinterpret_cast<decltype(host_glDiscardFramebufferEXT)>(eglGetProcAddress("glDiscardFramebufferEXT"));
    host_glGenVertexArraysOES = reinterpret_cast<decltype(host_glGenVertexArraysOES)>(eglGetProcAddress("glGenVertexArraysOES"));
    host_glBindVertexArrayOES = reinterpret_cast<decltype(host_glBindVertexArrayOES)>(eglGetProcAddress("glBindVertexArrayOES"));
    host_glDeleteVertexArraysOES = reinterpret_cast<decltype(host_glDeleteVertexArraysOES)>(eglGetProcAddress("glDeleteVertexArraysOES"));
}

extern "C" void glGenVertexArraysOESC(GLsizei count, GLuint *arrays) {
    host_glGenVertexArraysOES(count, arrays);
}

extern "C" void glDeleteVertexArraysOESC(GLsizei count, const GLuint *arrays) {
    host_glDeleteVertexArraysOES(count, arrays);
}
