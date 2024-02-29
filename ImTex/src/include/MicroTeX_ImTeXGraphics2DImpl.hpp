#ifndef MICROTEX_IMTEXGRAPHICS2DIMPL_HPP
#define MICROTEX_IMTEXGRAPHICS2DIMPL_HPP

#include <array>
#include <cstdarg>
#include <iostream>
#include <string>

#include "graphic/graphic.h"

#include "Calibri.ttf.hpp"
#include "ImTeX.hpp"
#include "MicroTeX_ImTeXFontImpl.hpp"
#include "Utils.hpp"


namespace MicroTeX = tex;

namespace ImTeX {
    class MicroTeX_ImTeXGraphics2DImpl : public MicroTeX::Graphics2D {
    public:
        MicroTeX_ImTeXGraphics2DImpl()
            : m_color(MicroTeX::black), m_stroke(), m_font(nullptr) {}


        Image getImage() {
            nvgluBindFramebuffer(runtimeData.nvgFBO.FBO);

            std::vector<uint8_t> data(runtimeData.nvgFBO.width * runtimeData.nvgFBO.height *
                                      4);
            glReadPixels(0, 0, runtimeData.nvgFBO.width, runtimeData.nvgFBO.height, GL_RGBA,
                         GL_UNSIGNED_BYTE, data.data());

            nvgluBindFramebuffer(nullptr);

            return {runtimeData.nvgFBO.width, runtimeData.nvgFBO.height, data};
        }


        virtual MicroTeX::color getColor() const override { return m_color; }
        virtual const MicroTeX::Stroke& getStroke() const override { return m_stroke; }
        virtual const MicroTeX::Font* getFont() const override { return m_font; }


        virtual void setColor(MicroTeX::color c) override {
            m_color = c;

            nvgFillColor(runtimeData.nvgCtx, toNvgColour(c));
            nvgStrokeColor(runtimeData.nvgCtx, toNvgColour(c));
        }

        virtual void setStroke(const MicroTeX::Stroke& s) override {
            m_stroke = s;

            // -2x^2 + 3x + 3, convert from 0,1,2 to 3,4,1
            int x = s.join;
            nvgLineJoin(runtimeData.nvgCtx, -2 * x * x + 3 * x + 3);

            nvgLineCap(runtimeData.nvgCtx, s.cap);
            nvgStrokeWidth(runtimeData.nvgCtx, s.lineWidth);
            nvgMiterLimit(runtimeData.nvgCtx, s.miterLimit);
        }

        virtual void setStrokeWidth(float w) override {
            m_stroke.lineWidth = w;

            nvgStrokeWidth(runtimeData.nvgCtx, w);
        }

        virtual void setFont(const MicroTeX::Font* font) override {
            m_font = font;

            nvgFontSize(runtimeData.nvgCtx, font->getSize());
            nvgFontFaceId(runtimeData.nvgCtx,
                          castTo<const MicroTeX_ImTeXFontImpl*>(font)->getFontId());
        }

        virtual void translate(float dx, float dy) override {
            nvgTranslate(runtimeData.nvgCtx, dx * m_scale.x, dy * m_scale.y);
        }

        virtual void scale(float sx, float sy) override {
            m_scale.x *= sx;
            m_scale.y *= sy;
        }

        virtual void rotate(float angle) override { nvgRotate(runtimeData.nvgCtx, angle); }

        virtual void rotate(float angle, float px, float py) {
            nvgTranslate(runtimeData.nvgCtx, px, py);
            nvgRotate(runtimeData.nvgCtx, angle);
            nvgTranslate(runtimeData.nvgCtx, px, py);
        }

        virtual void reset() override {
            nvgResetTransform(runtimeData.nvgCtx);

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


            nvgFillColor(runtimeData.nvgCtx, {0, 0, 0, 1});

            nvgText(runtimeData.nvgCtx, x, y, str.c_str(), nullptr);
        }


        virtual void drawLine(float x1, float y1, float x2, float y2) override {
            nvgMoveTo(runtimeData.nvgCtx, x1 * m_scale.x, y1 * m_scale.y);
            nvgLineTo(runtimeData.nvgCtx, x2 * m_scale.x, y2 * m_scale.y);
        }

        virtual void drawRect(float x, float y, float w, float h) override {
            nvgRect(runtimeData.nvgCtx, x * m_scale.x, y * m_scale.y, w * m_scale.x,
                    h * m_scale.y);
        }

        virtual void drawRoundRect(float x, float y, float w, float h, float rx,
                                   float ry) override {
            nvgRoundedRectVarying(runtimeData.nvgCtx, x * m_scale.x, y * m_scale.y,
                                  w * m_scale.x, h * m_scale.y, rx, -rx, ry,
                                  -ry);  // TODO
        }

        virtual void fillRect(float x, float y, float w, float h) override {
            drawRect(x, y, w, h);
            nvgFill(runtimeData.nvgCtx);
        }

        virtual void fillRoundRect(float x, float y, float w, float h, float rx,
                                   float ry) override {
            drawRoundRect(x, y, w, h, rx, ry);
            nvgFill(runtimeData.nvgCtx);
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
