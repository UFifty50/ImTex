#include "ImTeX.hpp"

#include <iostream>

#include "latex.h"

#include "Calibri.ttf.hpp"
#include "ImTeX.hpp"
#include "MicroTeX_ImTeXFontImpl.hpp"
#include "MicroTeX_ImTeXGraphics2DImpl.hpp"
#include "MicroTeX_ImTeXTextLayoutImpl.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


namespace ImTeX {
    ERRCODE Runtime::Init(const Config& conf) {
        if (!glfwInit()) {
            return ERRCODE::GLFW_INIT;
        }

#ifndef _WIN32  // don't require this on win32, and works with more cards
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

#ifdef MSAA
        glfwWindowHint(GLFW_SAMPLES, 4);
#endif
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);  // TODO: make it offscreen

        runtimeData.window =
            glfwCreateWindow(conf.imageWidth, conf.imageHeight, "", NULL, NULL);
        if (!runtimeData.window) {
            glfwTerminate();
            return ERRCODE::GLFW_CREATE_CONTEXT;
        }

        glfwMakeContextCurrent(runtimeData.window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            return ERRCODE::GLFW_CREATE_CONTEXT;
        }

#ifdef MSAA
        runtimeData.nvgCtx = nvgCreateGL3(NVG_STENCIL_STROKES | NVG_DEBUG);
#else
        runtimeData.nvgCtx = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
#endif

        if (runtimeData.nvgCtx == nullptr) {
            return ERRCODE::NVG_CREATE_CONTEXT;
        }

        int winWidth, winHeight;
        int fbWidth, fbHeight;
        glfwGetWindowSize(runtimeData.window, &winWidth, &winHeight);
        glfwGetFramebufferSize(runtimeData.window, &fbWidth, &fbHeight);

        runtimeData.nvgFBO.pixelRatio = (float)fbWidth / winWidth;

        float scaledWidth = winWidth * runtimeData.nvgFBO.pixelRatio;
        float scaledHeight = winHeight * runtimeData.nvgFBO.pixelRatio;

        runtimeData.nvgFBO.FBO =
            nvgluCreateFramebuffer(runtimeData.nvgCtx, scaledWidth, scaledHeight, 0);
        runtimeData.nvgFBO.width = scaledWidth;
        runtimeData.nvgFBO.height = scaledHeight;

        if (runtimeData.nvgFBO.FBO == nullptr) {
            return ERRCODE::NVG_CREATE_FRAMEBUFFER;
        }

        // Load default Serif and SansSerif fonts
        nvgCreateFontMem(runtimeData.nvgCtx, "Serif", calibri_ttf, calibri_ttf_len, 0);
        nvgCreateFontMem(runtimeData.nvgCtx, "SansSerif", calibri_ttf, calibri_ttf_len, 0);

        glfwSwapInterval(0);  // TODO: is this needed?

        runtimeData.config = conf;

        tex::LaTeX::init();

        return ERRCODE::OK;
    }


    void Runtime::Shutdown() {
        tex::LaTeX::release();

        nvgluDeleteFramebuffer(runtimeData.nvgFBO.FBO);
        nvgDeleteGL3(runtimeData.nvgCtx);
        glfwTerminate();
    }


    ERRCODE Runtime::ResetConfig(const Config& config) {
        runtimeData.config = config;

        glfwSetWindowSize(runtimeData.window, config.imageWidth, config.imageHeight);

        float scaledWidth = config.imageWidth * runtimeData.nvgFBO.pixelRatio;
        float scaledHeight = config.imageHeight * runtimeData.nvgFBO.pixelRatio;
        runtimeData.nvgFBO.FBO =
            nvgluCreateFramebuffer(runtimeData.nvgCtx, scaledWidth, scaledHeight, 0);

        if (runtimeData.nvgFBO.FBO == nullptr) {
            return ERRCODE::NVG_CREATE_FRAMEBUFFER;
        }

        return ERRCODE::OK;
    }


    Image Runtime::RenderCodeToImage(const std::wstring& code, float x, float y) {
        return RenderCodeToImage_local(code, x, y, runtimeData.config);
    }


    Image Runtime::RenderCodeToImage_local(const std::wstring& code, float x, float y,
                                           const Config& localConfig) {
        auto r = tex::LaTeX::parse(
            code, localConfig.imageWidth, localConfig.fontSize, localConfig.lineHeight,
            toMicroTeXColour(localConfig.foreground)  // doesnt work for now
        );

        Colour bg = localConfig.background;
        RuntimeData::FrameBuffer fb = runtimeData.nvgFBO;

        nvgluBindFramebuffer(fb.FBO);
        glViewport(0, 0, (GLsizei)fb.width, (GLsizei)fb.height);
        glClearColor(bg.r, bg.g, bg.b, bg.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        MicroTeX_ImTeXGraphics2DImpl g2;


        nvgBeginFrame(runtimeData.nvgCtx, fb.width, fb.height, fb.pixelRatio);
        nvgBeginPath(runtimeData.nvgCtx);

        r->draw(g2, x, y);

        nvgEndFrame(runtimeData.nvgCtx);
        nvgluBindFramebuffer(nullptr);

        delete r;

        return g2.getImage();
    }
};  // namespace ImTeX


int main() {
    ImTeX::Config config{
        //    .customFont = "Serif",
        .imageWidth = 720,
        .imageHeight = 300,
        .fontSize = 40,
        .lineHeight = 10,
        .foreground = {0.83f, 0.95f, 0.6f, 1.0f},
        .background = {0.93f, 0.95f, 0.93f, 1.0f},
    };

    switch (ImTeX::Runtime::Init(config)) {
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


    std::wstring code = L"\\int_{now}^{+\\infty} \\text{Keep trying}+\\sqrt{72}";

    ImTeX::Image img = ImTeX::Runtime::RenderCodeToImage(code, 10, 10);

    stbi_flip_vertically_on_write(true);
    stbi_write_png("output.png", img.width, img.height, 4, img.data.data(), img.width * 4);

    ImTeX::Runtime::Shutdown();

    return 0;
}
