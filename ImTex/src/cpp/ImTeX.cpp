#include "ImTeX.hpp"

#include <iostream>

#include "latex.h"

#include "Calibri.ttf.hpp"
#include "Globals.hpp"
#include "MicroTeX_ImTeXFontImpl.hpp"
#include "MicroTeX_ImTeXGraphics2DImpl.hpp"
#include "MicroTeX_ImTeXTextLayoutImpl.hpp"


NVGcontext* g_nvgCtx;
ImTeXFramebuffer* g_nvgFbo;

namespace ImTeX {

    union Colour {
        struct {
            float r;
            float g;
            float b;
            float a;
        };
        float rgba[4];
    };


    struct Config {
        //    std::string customFont;
        float fontSize;
        float lineHeight;
        Colour colour;
    };

    static Config GlobalConfig;

    enum class ERRCODE {
        OK = 0,

        // GLFW errors
        GLFW_INIT,
        GLFW_CREATE_CONTEXT,

        // NVG errors
        NVG_CREATE_CONTEXT,
        NVG_CREATE_FRAMEBUFFER,
    };

    static ERRCODE Init() {
        if (!glfwInit()) {
            return ERRCODE::GLFW_INIT;
        }

#ifndef _WIN32  // don't require this on win32, and works with more cards
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

#ifdef MSAA
        glfwWindowHint(GLFW_SAMPLES, 4);
#endif
        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

        GLFWwindow* offscreenCTX = glfwCreateWindow(640, 480, "", NULL, NULL);
        if (!offscreenCTX) {
            glfwTerminate();
            return ERRCODE::GLFW_CREATE_CONTEXT;
        }

        glfwMakeContextCurrent(offscreenCTX);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            return ERRCODE::GLFW_CREATE_CONTEXT;
        }

#ifdef MSAA
        g_nvgCtx = nvgCreateGL3(NVG_STENCIL_STROKES | NVG_DEBUG);
#else
        g_nvgCtx = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
#endif

        if (g_nvgCtx == nullptr) {
            return ERRCODE::NVG_CREATE_CONTEXT;
        }

        // Load default Serif and SansSerif fonts
        nvgCreateFontMem(g_nvgCtx, "Serif", calibri_ttf, calibri_ttf_len, 0);
        nvgCreateFontMem(g_nvgCtx, "SansSerif", calibri_ttf, calibri_ttf_len, 0);

        int ctxWidth, ctxHeight;
        glfwGetWindowSize(offscreenCTX, &ctxWidth, &ctxHeight);

        float pxRatio = 640.0f / (float)ctxWidth;

        g_nvgFbo = new ImTeXFramebuffer();
        //    g_nvgFbo->fbo = nvgluCreateFramebuffer(g_nvgCtx, 100 * pxRatio, 100 * pxRatio,
        //    0);
        g_nvgFbo->width = 640.0f;
        g_nvgFbo->height = 480.0f;
        g_nvgFbo->pxRatio = pxRatio;

        //    if (g_nvgFbo == nullptr) {
        //         return ERRCODE::NVG_CREATE_FRAMEBUFFER;
        //     }


        tex::LaTeX::init();

        return ERRCODE::OK;
    }

    static void Shutdown() {
        tex::LaTeX::release();

        //     nvgluDeleteFramebuffer(g_nvgFbo->fbo);
        nvgDeleteGL3(g_nvgCtx);
        delete g_nvgFbo;
        glfwTerminate();
    }


    void SetupGlobalConfig(Config config) { GlobalConfig = config; }

    tex::color toMicroTeXColour(Colour c) { return tex::argb(c.a, c.r, c.g, c.b); }

    void RenderCode(const std::wstring& code, float width, float x, float y) {
        //   Convert the code to a paintable object (TeXRender)
        auto r =
            tex::LaTeX::parse(code,   // LaTeX code to parse
                              width,  // logical width of the graphics context (in pixel)
                              GlobalConfig.fontSize,    // font size (in point)
                              GlobalConfig.lineHeight,  // space between 2 lines (in pixel)
                              toMicroTeXColour(GlobalConfig.colour)  // foreground color
            );

        MicroTeX_ImTeXGraphics2DImpl g2;

        g2.begin();
        r->draw(g2, x, y);
        g2.end();

        delete r;
    }

    void RenderCode_local(const std::wstring& code, float width, float x, float y,
                          Config localConfig) {
        Config oldConfig = GlobalConfig;
        SetupGlobalConfig(localConfig);
        RenderCode(code, width, x, y);
        SetupGlobalConfig(oldConfig);
    }
};  // namespace ImTeX

int main() {
    switch (ImTeX::Init()) {
        case ImTeX::ERRCODE::GLFW_INIT:
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return 1;
        case ImTeX::ERRCODE::GLFW_CREATE_CONTEXT:
            std::cerr << "Failed to create GLFW context" << std::endl;
            return 1;
        case ImTeX::ERRCODE::NVG_CREATE_CONTEXT:
            std::cerr << "Failed to create NanoVG context" << std::endl;
            return 1;
        case ImTeX::ERRCODE::NVG_CREATE_FRAMEBUFFER:
            std::cerr << "Failed to create NanoVG framebuffer" << std::endl;
            return 1;
        case ImTeX::ERRCODE::OK:
            break;
    }


    ImTeX::Config config{
        //    .customFont = "Serif",
        .fontSize = 40,
        .lineHeight = 10,
        .colour = {0.83f, 0.95f, 0.6f, 1.0f},
    };
    ImTeX::SetupGlobalConfig(config);

    std::wstring code = L"\\int_{now}^{+\\infty} \\text{Keep trying}";

    // ImTeX::Image img = ImTeX::GetRenderedImage();

    while (!glfwWindowShouldClose(glfwGetCurrentContext())) {
        glViewport(0, 0, (GLsizei)g_nvgFbo->width, (GLsizei)g_nvgFbo->height);
        glClearColor(0.93f, 0.95f, 0.93f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


        ImTeX::RenderCode(code, 640, 10, 10);

        ImTeX::Config localConfig{
            .fontSize = 20,
            .lineHeight = 10,
            .colour = {0.83f, 0.95f, 0.6f, 1.0f},
        };
        ImTeX::RenderCode_local(code, 640, 10, 10, localConfig);


        glfwSwapBuffers(glfwGetCurrentContext());
        glfwPollEvents();
    }

    // std::vector<uint8_t> imageData = g2.getImage();
    // std::cout << "Image data: " << imageData.data() << std::endl;
    ImTeX::Shutdown();

    return 0;
}
