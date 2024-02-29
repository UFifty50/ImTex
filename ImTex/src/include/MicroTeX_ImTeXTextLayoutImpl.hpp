#ifndef MICROTEX_IMTEXTEXTLAYOUTIMPL_HPP
#define MICROTEX_IMTEXTEXTLAYOUTIMPL_HPP

#include <string>

#include "graphic/graphic.h"

namespace MicroTeX = tex;


namespace ImTeX {
    class MicroTeX_ImTeXTextLayoutImpl : public MicroTeX::TextLayout {
    public:
        MicroTeX_ImTeXTextLayoutImpl(const std::wstring& src,
                                     const MicroTeX::sptr<MicroTeX::Font>& font,
                                     uint32_t width, uint32_t height)
            : m_str(src), m_font(font), m_bounds({width, height}) {
            std::cout << "initialise" << std::endl;
        }


        virtual void getBounds(MicroTeX::Rect& bounds) override {
            std::cout << "getBounds" << std::endl;
            bounds.x = 0;
            bounds.y = 0;
            bounds.w = (float)m_bounds.width;
            bounds.h = (float)m_bounds.height;
        }

        virtual void draw(MicroTeX::Graphics2D& g2, float x, float y) override {
            std::cout << "draw" << std::endl;
            const MicroTeX::Font* old = g2.getFont();

            g2.setFont(m_font.get());
            g2.drawText(m_str, x, y);
            g2.setFont(old);
        }

        static MicroTeX::sptr<MicroTeX_ImTeXTextLayoutImpl> create(
            const std::wstring& src, const MicroTeX::sptr<MicroTeX::Font>& font,
            uint32_t width, uint32_t height) {
            return MicroTeX::sptrOf<ImTeX::MicroTeX_ImTeXTextLayoutImpl>(src, font, width,
                                                                         height);
        }

    private:
        std::wstring m_str;
        MicroTeX::sptr<MicroTeX::Font> m_font;
        struct {
            uint32_t width;
            uint32_t height;
        } m_bounds;
    };
}  // namespace ImTeX

namespace tex {
    MicroTeX::sptr<MicroTeX::TextLayout> MicroTeX::TextLayout::create(
        const std::wstring& src, const MicroTeX::sptr<MicroTeX::Font>& font) {
        return ImTeX::MicroTeX_ImTeXTextLayoutImpl::create(src, font, 0, 0);
    }
}  // namespace tex

#endif  // MICROTEX_IMTEXTEXTLAYOUTIMPL_HPP
