#include <string>

#include "graphic/graphic.h"

#include "Globals.hpp"


namespace MicroTeX = tex;

class MicroTeX_ImTeXFontImpl : public MicroTeX::Font {
public:
    MicroTeX_ImTeXFontImpl(const std::string& file, float size)
        : MicroTeX_ImTeXFontImpl("", MicroTeX::PLAIN, size) {}

    MicroTeX_ImTeXFontImpl(const std::string& name, int style, float size)
        : m_name(name), m_style(style), m_size(size) {
        m_fontIdx = nvgFindFont(g_nvgCtx, name.c_str());

        if (m_fontIdx == -1) {
            m_fontIdx = nvgCreateFont(g_nvgCtx, name.c_str(), name.c_str());
        }
    }

    MicroTeX_ImTeXFontImpl(const std::string& name, unsigned char* data, int dataSize,
                           float size)
        : m_style(MicroTeX::PLAIN), m_size(size) {
        m_fontIdx = nvgCreateFontMem(g_nvgCtx, name.c_str(), data, dataSize, 0);
    }

    virtual ~MicroTeX_ImTeXFontImpl() override = default;


    virtual float getSize() const override { return m_size; }

    virtual MicroTeX::sptr<MicroTeX::Font> deriveFont(int style) const override {
        return MicroTeX::sptrOf<MicroTeX_ImTeXFontImpl>(m_name, style, m_size);
    }

    virtual bool operator==(const MicroTeX::Font& other) const override {
        auto& otherFont = castFrom(other);
        return m_name == otherFont.m_name && m_style == otherFont.m_style &&
               m_size == otherFont.m_size;
    }

    virtual bool operator!=(const MicroTeX::Font& other) const override {
        return !operator==(other);
    }

    static MicroTeX::Font* create(const std::string& file, float size) {
        return new MicroTeX_ImTeXFontImpl(file, size);
    }

    static MicroTeX::Font* create(const std::string& name, unsigned char* data, int dataSize,
                                  float size) {
        return new MicroTeX_ImTeXFontImpl(name, data, dataSize, size);
    }

    static MicroTeX::sptr<MicroTeX::Font> _create(const std::string& name, int style,
                                                  float size) {
        return MicroTeX::sptrOf<MicroTeX_ImTeXFontImpl>(name, style, size);
    }

private:
    std::string m_name;
    int m_style;
    float m_size;

    int32_t m_fontIdx;


    const MicroTeX_ImTeXFontImpl& castFrom(const MicroTeX::Font& font) const {
        return dynamic_cast<const MicroTeX_ImTeXFontImpl&>(font);
    }
};
