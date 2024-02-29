#ifndef MICROTEX_IMTEXFONTIMPL_HPP
#define MICROTEX_IMTEXFONTIMPL_HPP

#include <filesystem>
#include <string>

#include "graphic/graphic.h"

#include "Globals.hpp"
#include "Utils.hpp"


namespace MicroTeX = tex;
namespace fs = std::filesystem;


namespace ImTeX {
    static float FontSize = 40;

    class MicroTeX_ImTeXFontImpl : public MicroTeX::Font {
    public:
        MicroTeX_ImTeXFontImpl(const std::string& name, int style, float size)
            : m_name(name), m_style(style), m_size(size) {
            m_fontIdx = nvgFindFont(g_nvgCtx, name.c_str());

            if (m_fontIdx == -1) {
                std::cout << "Could not find font: " << name << std::endl;
            }

            m_size = 40;
        }

        MicroTeX_ImTeXFontImpl(const std::string& file, float pointsPerPixel)
            : m_name(fileName(file)), m_style(MicroTeX::PLAIN) {
            m_fontIdx = nvgFindFont(g_nvgCtx, file.c_str());

            if (m_fontIdx != -1) return;

            m_fontIdx = nvgCreateFont(g_nvgCtx, file.c_str(), file.c_str());

            if (m_fontIdx == -1) {
                throw std::runtime_error("Failed to load font: " + file);
            }

            m_size = 40;
        }

        MicroTeX_ImTeXFontImpl(const std::string& name, unsigned char* data, int dataSize,
                               int style, float size)
            : m_style(style), m_size(size) {
            m_fontIdx = nvgFindFont(g_nvgCtx, name.c_str());

            if (m_fontIdx == -1) {
                m_fontIdx = nvgCreateFontMem(g_nvgCtx, name.c_str(), data, dataSize, 0);
            }

            m_size = 40;
        }

        virtual ~MicroTeX_ImTeXFontImpl() override = default;


        virtual float getSize() const override { return m_size; }

        virtual MicroTeX::sptr<MicroTeX::Font> deriveFont(int style) const override {
            return MicroTeX::sptrOf<MicroTeX_ImTeXFontImpl>(m_name, style, m_size);
        }

        virtual bool operator==(const MicroTeX::Font& other) const override {
            auto& otherFont = castTo<decltype(*this)>(other);
            return m_name == otherFont.m_name && m_style == otherFont.m_style &&
                   m_size == otherFont.m_size;
        }

        virtual bool operator!=(const MicroTeX::Font& other) const override {
            return !operator==(other);
        }

        int32_t getFontId() const { return m_fontIdx; }


        static MicroTeX_ImTeXFontImpl* create(const std::string& name, unsigned char* data,
                                              int dataSize, int style, float size) {
            return new MicroTeX_ImTeXFontImpl(name, data, dataSize, style, size);
        }


    private:
        std::string m_name;
        int m_style;
        float m_size;

        int32_t m_fontIdx;

        std::string fileName(const std::string& name, bool hasExtension = false) {
            fs::path asPath = name;
            return hasExtension ? asPath.filename().string() : asPath.stem().string();
        }
    };
}  // namespace ImTeX

namespace tex {
    MicroTeX::Font* MicroTeX::Font::create(const std::string& file, float size) {
        return new ImTeX::MicroTeX_ImTeXFontImpl(file, size);
    }

    MicroTeX::sptr<MicroTeX::Font> MicroTeX::Font::_create(const std::string& name, int style,
                                                           float size) {
        return MicroTeX::sptrOf<ImTeX::MicroTeX_ImTeXFontImpl>(name, style, size);
    }
}  // namespace tex

#endif  // MICROTEX_IMTEXFONTIMPL_HPP
