#include <string>

#include "graphic/graphic.h"

#include "MicroTeX_ImTeXFontImpl.hpp"


namespace MicroTeX = tex;

class MicroTeX_ImTeXGraphics2DImpl : public MicroTeX::Graphics2D {
public:
    MicroTeX_ImTeXGraphics2DImpl() : m_color(MicroTeX::black), m_stroke(), m_sx(1), m_sy(1) {
        setColor(MicroTeX::black);
        setStroke(m_stroke);
        setFont(m_font);
    }

    virtual void setColor(MicroTeX::color c) override { m_color = c; }
    virtual MicroTeX::color getColor() const override { return m_color; }

    virtual void setStroke(const MicroTeX::Stroke& s) override { m_stroke = s; }
    virtual const MicroTeX::Stroke& getStroke() const override { return m_stroke; }
    virtual void setStrokeWidth(float w) override { m_stroke.lineWidth = w; }

    virtual const MicroTeX::Font* getFont() const override { return m_font; }
    virtual void setFont(const MicroTeX::Font* font) override { m_font = font; }

    virtual void translate(float dx, float dy) override;

    virtual void scale(float sx, float sy) override {
        m_sx *= sx;
        m_sy *= sy;
    }

    virtual void rotate(float angle) override;
    virtual void rotate(float angle, float px, float py) override;
    virtual void reset() override {
        m_sx = 1;
        m_sy = 1;
    }

    virtual float sx() const override { return m_sx; }
    virtual float sy() const override { return m_sy; }

    virtual void drawChar(wchar_t c, float x, float y) override {
        drawText(std::wstring(1, c), x, y);
    }

    virtual void drawText(const std::wstring& c, float x, float y) override;
    virtual void drawLine(float x1, float y1, float x2, float y2) override;
    virtual void drawRect(float x, float y, float w, float h) override;
    virtual void drawRoundRect(float x, float y, float w, float h, float rx,
                               float ry) override {
        double r = std::max(rx, ry);
        double d = NVG_PI / 180.0f;
        // TODO
    }

    virtual void fillRect(float x, float y, float w, float h) override;
    virtual void fillRoundRect(float x, float y, float w, float h, float rx,
                               float ry) override {
        double r = std::max(rx, ry);
        double d = NVG_PI / 180.0f;
        // TODO
    }

private:
    MicroTeX::color m_color;
    MicroTeX::Stroke m_stroke;
    const MicroTeX::Font* m_font =
        MicroTeX_ImTeXFontImpl::create("Default Font", defaultFontData, 0, MicroTeX::PLAIN);
    float m_sx;
    float m_sy;
};

constexpr unsigned char* defaultFontData = nullptr;
