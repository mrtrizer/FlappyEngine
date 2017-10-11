#pragma once

#include <unordered_map>

#include <Tools.h>
#include <Res.h>
#include <JsonRes.h>

namespace flappy {

/// @details [About format](http://www.angelcode.com/products/bmfont/doc/file_format.html)
class GlyphSheetRes: public Res<GlyphSheetRes>
{
public:
    struct Info {
        /// This is the name of the true type font.
        std::string face;
        /// The size of the true type font.
        int size;
        /// The font is bold.
        int bold;
        /// The font is italic.
        int italic;
        std::vector<char> charset;
        /// Set to true if it is the unicode charset.
        int unicode;
        /// The font height stretch in percentage. 100% means no stretch.
        int stretchH;
        /// Set to true if smoothing was turned on.
        int smooth;
        /// The supersampling level used. true means no supersampling was used.
        int aa;
        /// The padding for each character (up, right, down, left).
        std::vector<float> padding;
        /// The spacing for each character (horizontal, vertical).
        std::vector<float> spacing;
        /// The outline thickness for the characters.
        float outline;
    };

    struct Common {
        /// This is the distance in pixels between each line of text.
        float lineHeight;
        /// The number of pixels from the absolute top of the line to the base of the characters.
        float base;
        /// The width of the texture, normally used to scale the x pos of the character image.
        int scaleW;
        /// The height of the texture, normally used to scale the y pos of the character image.
        int scaleH;
        /// The number of texture pages included in the font.
        int pages;
        /// Set to 1 if the monochrome characters have been packed into each of the texture channels. In this case alphaChnl describes what is stored in each channel.
        int packed;
        /// Set to 0 if the channel holds the glyph data, 1 if it holds the outline, 2 if it holds the glyph and the outline, 3 if its set to zero, and 4 if its set to one.
        int alphaChnl;
        /// Set to 0 if the channel holds the glyph data, 1 if it holds the outline, 2 if it holds the glyph and the outline, 3 if its set to zero, and 4 if its set to one.
        int redChnl;
        /// Set to 0 if the channel holds the glyph data, 1 if it holds the outline, 2 if it holds the glyph and the outline, 3 if its set to zero, and 4 if its set to one.
        int greenChnl;
        /// Set to 0 if the channel holds the glyph data, 1 if it holds the outline, 2 if it holds the glyph and the outline, 3 if its set to zero, and 4 if its set to one.
        int blueChnl;
    };

    struct Glyph {
        int id;
        float x;
        float y;
        float width;
        float height;
        /// How much the current position should be offset when copying the image from the texture to the screen.
        float xoffset;
        /// How much the current position should be offset when copying the image from the texture to the screen.
        float yoffset;
        ///	How much the current position should be advanced after drawing the character.
        float xadvance;
        /// The texture page where the character image is found.
        float page;
        /// The texture channel where the character image is found (1 = blue, 2 = green, 4 = red, 8 = alpha, 15 = all channels).
        float chnl;
    };

    struct Kerning {
        /// The first character id.
        int first;
        /// The second character id.
        int second;
        /// How much the x position should be adjusted when drawing the second character immediately following the first.
        float amount;
    };

    GlyphSheetRes(std::shared_ptr<JsonRes> jsonRes = nullptr);

    void addGlyph(int id, Glyph);
    Glyph glyph(int id) const;

    void addKerning(Kerning);
    Kerning kerning(int firstId, int secondId) const;

    void setCommon(Common common) { m_common = common; }

    void setInfo(Info info) { m_info = info; }

    std::list<std::shared_ptr<ResBase>> dependencyList() const final { return {m_jsonRes}; }


private:  
    std::unordered_map<int, Glyph> m_glyphMap;
    std::list<Kerning> m_kerningList;
    Common m_common;
    Info m_info;
    std::shared_ptr<JsonRes> m_jsonRes;
};

} // flappy
