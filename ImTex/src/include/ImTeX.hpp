#ifndef IMTEX_HPP
#define IMTEX_HPP
#include <iostream>
#include <string>

#include "glad/glad.h"

#define GLFW_INCLUDE_GLEXT
#include "GLFW/glfw3.h"
#include "nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "latex.h"
#include "nanovg_gl.h"
#include "nanovg_gl_utils.h"


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
        float imageWidth, imageHeight;

        float fontSize;
        float lineHeight;
        Colour foreground;
        Colour background;
    };

    struct RuntimeData {
        GLFWwindow* window;
        NVGcontext* nvgCtx;
        struct FrameBuffer {
            NVGLUframebuffer* FBO;
            float width, height;
            float pixelRatio;
        } nvgFBO;

        Config config;

        float getFontSize() { return config.fontSize; }
    };

    static RuntimeData runtimeData;


    struct Image {
        float width, height;
        std::vector<uint8_t> data;
    };

    enum class ERRCODE {
        OK = 0,

        // GLFW errors
        GLFW_INIT,
        GLFW_CREATE_CONTEXT,

        // NVG errors
        NVG_CREATE_CONTEXT,
        NVG_CREATE_FRAMEBUFFER,
    };


    class Runtime {
    public:
        static ERRCODE Init(const Config& conf);

        static void Shutdown();


        // configuration
        static ERRCODE ResetConfig(const Config& config);


        // rendering
        static Image RenderCodeToImage(const std::wstring& code, float x, float y);
        static Image RenderCodeToImage_local(const std::wstring& code, float x, float y,
                                             const Config& localConfig);

        // RenderCodeToFrambuffer
        // RenderCodeToFrambuffer_local


        // helpers
        static tex::color toMicroTeXColour(const Colour& c) {
            return tex::argb(c.a, c.r, c.g, c.b);
        }
    };
};  // namespace ImTeX
#endif
