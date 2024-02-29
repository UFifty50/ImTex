#ifndef IMTEX_GLOBALS_HPP
#define IMTEX_GLOBALS_HPP

#include "glad/glad.h"

#define GLFW_INCLUDE_GLEXT
#include "GLFW/glfw3.h"
#include "nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"
#include "nanovg_gl_utils.h"


struct ImTeXFramebuffer {
    NVGLUframebuffer* fbo;
    float width, height;
    float pxRatio;
};

extern NVGcontext* g_nvgCtx;
extern ImTeXFramebuffer* g_nvgFbo;

#endif
