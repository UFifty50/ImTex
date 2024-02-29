#ifndef MICROTEX_IMTEXGRAPHICS2DIMPL_HPP
#define MICROTEX_IMTEXGRAPHICS2DIMPL_HPP

#include <array>
#include <cstdarg>
#include <iostream>
#include <string>

#include "graphic/graphic.h"

#include "Calibri.ttf.hpp"
#include "Globals.hpp"
#include "MicroTeX_ImTeXFontImpl.hpp"
#include "Utils.hpp"


namespace MicroTeX = tex;

namespace ImTeX {
    class MicroTeX_ImTeXGraphics2DImpl : public MicroTeX::Graphics2D {
    public:
        MicroTeX_ImTeXGraphics2DImpl()
            : m_color(MicroTeX::black), m_stroke(), m_font(nullptr) {
            if (g_nvgFbo == nullptr) return;


            //      nvgluBindFramebuffer(g_nvgFbo->fbo);
        }

        void begin() {
            float winWidth = g_nvgFbo->width / g_nvgFbo->pxRatio;
            float winHeight = g_nvgFbo->height / g_nvgFbo->pxRatio;
            nvgBeginFrame(g_nvgCtx, winWidth, winHeight, g_nvgFbo->pxRatio);
            nvgBeginPath(g_nvgCtx);
        }
        void end() {
            /* flushText();*/
            nvgEndFrame(g_nvgCtx);
        }

        std::vector<uint8_t> getImage() {
            std::cout << g_nvgFbo->width << " " << g_nvgFbo->height << " " << g_nvgFbo->pxRatio
                      << "\n";
            // nvgluBindFramebuffer(nullptr);


            std::vector<uint8_t> imageData(640 * 480 * 4);
            //    glReadPixels(0, 0, g_nvgFbo->width, g_nvgFbo->height, GL_RGBA,
            //    GL_UNSIGNED_BYTE,
            //               imageData.data());

            return imageData;
        }


        virtual MicroTeX::color getColor() const override { return m_color; }
        virtual const MicroTeX::Stroke& getStroke() const override { return m_stroke; }
        virtual const MicroTeX::Font* getFont() const override { return m_font; }


        virtual void setColor(MicroTeX::color c) override {
            m_color = c;

            nvgFillColor(g_nvgCtx, toNvgColour(c));
            nvgStrokeColor(g_nvgCtx, toNvgColour(c));
        }

        virtual void setStroke(const MicroTeX::Stroke& s) override {
            m_stroke = s;

            // -2x^2 + 3x + 3, convert from 0,1,2 to 3,4,1
            int x = s.join;
            nvgLineJoin(g_nvgCtx, -2 * x * x + 3 * x + 3);

            nvgLineCap(g_nvgCtx, s.cap);
            nvgStrokeWidth(g_nvgCtx, s.lineWidth);
            nvgMiterLimit(g_nvgCtx, s.miterLimit);
        }

        virtual void setStrokeWidth(float w) override {
            m_stroke.lineWidth = w;

            nvgStrokeWidth(g_nvgCtx, w);
        }

        virtual void setFont(const MicroTeX::Font* font) override {
            m_font = font;

            nvgFontSize(g_nvgCtx, font->getSize());
            nvgFontFaceId(g_nvgCtx, castTo<const MicroTeX_ImTeXFontImpl*>(font)->getFontId());
        }

        virtual void translate(float dx, float dy) override {
            nvgTranslate(g_nvgCtx, dx * m_scale.x, dy * m_scale.y);
        }

        virtual void scale(float sx, float sy) override {
            //       nvgScale(g_nvgCtx, sx, sy);

            m_scale.x *= sx;
            m_scale.y *= sy;
        }

        virtual void rotate(float angle) override { nvgRotate(g_nvgCtx, angle); }

        virtual void rotate(float angle, float px, float py) {
            nvgTranslate(g_nvgCtx, px, py);
            nvgRotate(g_nvgCtx, angle);
            nvgTranslate(g_nvgCtx, px, py);
        }

        virtual void reset() override {
            nvgResetTransform(g_nvgCtx);

            m_scale.x = m_scale.y = 1.0f;
        }

        virtual float sx() const override { return m_scale.x; }
        virtual float sy() const override { return m_scale.y; }

        virtual void drawChar(wchar_t wc, float x, float y) override {
            drawText({wc, L'\0'}, x, y);
        }

        virtual void drawText(const std::wstring& ws, float x, float y) override {
            std::string str;
            wcstombs(str.data(), ws.c_str(), ws.size());


            nvgFillColor(g_nvgCtx, {0, 0, 0, 1});

            nvgText(g_nvgCtx, x, y, str.c_str(), nullptr);
        }


        virtual void drawLine(float x1, float y1, float x2, float y2) override {
            nvgMoveTo(g_nvgCtx, x1, y1);
            nvgLineTo(g_nvgCtx, x2, y2);
        }

        virtual void drawRect(float x, float y, float w, float h) override {
            nvgRect(g_nvgCtx, x, y, w, h);
        }

        virtual void drawRoundRect(float x, float y, float w, float h, float rx,
                                   float ry) override {
            nvgRoundedRectVarying(g_nvgCtx, x, y, w, h, rx, -rx, ry, -ry);  // TODO
        }

        virtual void fillRect(float x, float y, float w, float h) override {
            drawRect(x, y, w, h);
            nvgFill(g_nvgCtx);
        }

        virtual void fillRoundRect(float x, float y, float w, float h, float rx,
                                   float ry) override {
            drawRoundRect(x, y, w, h, rx, ry);
            nvgFill(g_nvgCtx);
        }

    private:
        struct Coord2D {
            float x, y;
        };

        MicroTeX::color m_color;
        MicroTeX::Stroke m_stroke;
        const MicroTeX::Font* m_font;

        Coord2D m_scale = {1, 1};
    };
}  // namespace ImTeX

#endif  // MICROTEX_IMTEXGRAPHICS2DIMPL_HPP
