#include <string>

#include "graphic/graphic.h"

#include "Globals.hpp"


namespace MicroTeX = tex;

class MicroTeX_ImTeXTextLayoutImpl : public MicroTeX::TextLayout {
public:
    MicroTeX_ImTeXTextLayoutImpl(const std::wstring& src,
                                 const MicroTeX::sptr<MicroTeX::Font>& font, int width,
                                 int height)
        : m_str(src), m_font(font), m_bounds({width, height}) {}


    virtual void getBounds(MicroTeX::Rect& bounds) override {
        bounds.x = 0;
        bounds.y = 0;
        bounds.w = (float)m_bounds.width;
        bounds.h = (float)m_bounds.height;
    }

    virtual void draw(MicroTeX::Graphics2D& g2, float x, float y) override {
        g2.setFont(m_font.get());
        g2.translate(x, y);
        g2.drawText(m_str, x, y);
        g2.translate(-x, -y);
    }

    static MicroTeX::sptr<MicroTeX::TextLayout> create(
        const std::wstring& src, const MicroTeX::sptr<MicroTeX::Font>& font, int width,
        int height) {
        return MicroTeX::sptrOf<MicroTeX_ImTeXTextLayoutImpl>(src, font, width, height);
    }

private:
    std::wstring m_str;
    MicroTeX::sptr<MicroTeX::Font> m_font;
    struct {
        int width;
        int height;
    } m_bounds;
};
