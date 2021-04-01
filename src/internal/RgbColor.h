/*-------------------------------------------------------------------------
RgbColor provides a color object that can be directly consumed by NeoPixelBus

Written by Michael C. Miller.

I invest time and resources providing this open source code,
please support me by dontating (see https://github.com/Makuna/NeoPixelBus)

-------------------------------------------------------------------------
This file is part of the Makuna/NeoPixelBus library.

NeoPixelBus is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of
the License, or (at your option) any later version.

NeoPixelBus is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with NeoPixel.  If not, see
<http://www.gnu.org/licenses/>.
-------------------------------------------------------------------------*/
#pragma once

#include <cstdint>
#include <cstdlib>
#include <algorithm>

#include "NeoSettings.h"

struct HslColor;
struct HsbColor;

// ------------------------------------------------------------------------
// RgbColor represents a color object that is represented by Red, Green, Blue
// component values.  It contains helpful color routines to manipulate the
// color.
// ------------------------------------------------------------------------
struct RgbColor
{
    using SettingsObject = NeoRgbCurrentSettings;

    // ------------------------------------------------------------------------
    // Construct a RgbColor that will have its values set in latter operations
    // ------------------------------------------------------------------------
    constexpr RgbColor();

    // ------------------------------------------------------------------------
    // Construct a RgbColor using R, G, B values (0-255)
    // ------------------------------------------------------------------------
    constexpr RgbColor(uint8_t r, uint8_t g, uint8_t b);

    // ------------------------------------------------------------------------
    // Construct a RgbColor using a single brightness value (0-255)
    // This works well for creating gray tone colors
    // (0) = black, (255) = white, (128) = gray
    // ------------------------------------------------------------------------
    constexpr RgbColor(uint8_t brightness);

    // ------------------------------------------------------------------------
    // Construct a RgbColor using HslColor
    // ------------------------------------------------------------------------
    constexpr RgbColor(const RgbColor& color);

    // ------------------------------------------------------------------------
    // Construct a RgbColor using HslColor
    // ------------------------------------------------------------------------
    constexpr RgbColor(const HslColor& color);

    // ------------------------------------------------------------------------
    // Construct a RgbColor using HsbColor
    // ------------------------------------------------------------------------
    constexpr RgbColor(const HsbColor& color);

    // ------------------------------------------------------------------------
    // Comparison operators
    // ------------------------------------------------------------------------
    constexpr bool operator==(const RgbColor& other) const;

    constexpr bool operator!=(const RgbColor& other) const;

    // ------------------------------------------------------------------------
    // CalculateBrightness will calculate the overall brightness
    // NOTE: This is a simple linear brightness
    // ------------------------------------------------------------------------
    constexpr uint8_t CalculateBrightness() const;

    // ------------------------------------------------------------------------
    // Dim will return a new color that is blended to black with the given ratio
    // ratio - (0-255) where 255 will return the original color and 0 will return black
    //
    // NOTE: This is a simple linear blend
    // ------------------------------------------------------------------------
    constexpr RgbColor Dim(uint8_t ratio) const;

    // ------------------------------------------------------------------------
    // Brighten will return a new color that is blended to white with the given ratio
    // ratio - (0-255) where 255 will return the original color and 0 will return white
    //
    // NOTE: This is a simple linear blend
    // ------------------------------------------------------------------------
    constexpr RgbColor Brighten(uint8_t ratio) const;

    // ------------------------------------------------------------------------
    // Darken will adjust the color by the given delta toward black
    // NOTE: This is a simple linear change
    // delta - (0-255) the amount to dim the color
    // ------------------------------------------------------------------------
    constexpr void Darken(uint8_t delta);

    // ------------------------------------------------------------------------
    // Lighten will adjust the color by the given delta toward white
    // NOTE: This is a simple linear change
    // delta - (0-255) the amount to lighten the color
    // ------------------------------------------------------------------------
    constexpr void Lighten(uint8_t delta);

    // ------------------------------------------------------------------------
    // LinearBlend between two colors by the amount defined by progress variable
    // left - the color to start the blend at
    // right - the color to end the blend at
    // progress - (0.0 - 1.0) value where 0 will return left and 1.0 will return right
    //     and a value between will blend the color weighted linearly between them
    // ------------------------------------------------------------------------
    static constexpr RgbColor LinearBlend(const RgbColor& left, const RgbColor& right, float progress);

    // ------------------------------------------------------------------------
    // BilinearBlend between four colors by the amount defined by 2d variable
    // c00 - upper left quadrant color
    // c01 - upper right quadrant color
    // c10 - lower left quadrant color
    // c11 - lower right quadrant color
    // x - unit value (0.0 - 1.0) that defines the blend progress in horizontal space
    // y - unit value (0.0 - 1.0) that defines the blend progress in vertical space
    // ------------------------------------------------------------------------
    static constexpr RgbColor BilinearBlend(const RgbColor& c00,
        const RgbColor& c01,
        const RgbColor& c10,
        const RgbColor& c11,
        float x,
        float y);

    constexpr uint32_t CalcTotalTenthMilliAmpere(const SettingsObject& settings) const;

    // ------------------------------------------------------------------------
    // Red, Green, Blue color members (0-255) where
    // (0,0,0) is black and (255,255,255) is white
    // ------------------------------------------------------------------------
    uint8_t R{};
    uint8_t G{};
    uint8_t B{};

private:
    inline static constexpr uint8_t _elementDim(uint8_t value, uint8_t ratio);
    inline static constexpr uint8_t _elementBrighten(uint8_t value, uint8_t ratio);
    inline static constexpr float _CalcColor(float p, float q, float t);
    inline static constexpr RgbColor convertToRgbColor(const HslColor& color);
    inline static constexpr RgbColor convertToRgbColor(HsbColor color);
    inline static constexpr bool fuzzyCompare(double p1, double p2);
    inline static constexpr bool fuzzyCompare(float p1, float p2);
};

#include "HslColor.h"
#include "HsbColor.h"

constexpr RgbColor::RgbColor() = default;

constexpr RgbColor::RgbColor(uint8_t r, uint8_t g, uint8_t b) :
    R{r}, G{g}, B{b}
{
}

constexpr RgbColor::RgbColor(uint8_t brightness) :
    R{brightness}, G{brightness}, B{brightness}
{
}

constexpr RgbColor::RgbColor(const RgbColor& color) = default;

constexpr RgbColor::RgbColor(const HslColor& color) :
    RgbColor{convertToRgbColor(color)}
{
}

constexpr RgbColor::RgbColor(const HsbColor& color) :
    RgbColor{convertToRgbColor(color)}
{
}

constexpr bool RgbColor::operator==(const RgbColor& other) const
{
    return (R == other.R && G == other.G && B == other.B);
};

constexpr bool RgbColor::operator!=(const RgbColor& other) const
{
    return !(*this == other);
};

constexpr uint8_t RgbColor::CalculateBrightness() const
{
    return (uint8_t)(((uint16_t)R + (uint16_t)G + (uint16_t)B) / 3);
}

constexpr RgbColor RgbColor::Dim(uint8_t ratio) const
{
    // specifically avoids float math
    return RgbColor(_elementDim(R, ratio), _elementDim(G, ratio), _elementDim(B, ratio));
}

constexpr RgbColor RgbColor::Brighten(uint8_t ratio) const
{
    // specifically avoids float math
    return RgbColor(_elementBrighten(R, ratio), _elementBrighten(G, ratio), _elementBrighten(B, ratio));
}

constexpr void RgbColor::Darken(uint8_t delta)
{
    if (R > delta)
    {
        R -= delta;
    }
    else
    {
        R = 0;
    }

    if (G > delta)
    {
        G -= delta;
    }
    else
    {
        G = 0;
    }

    if (B > delta)
    {
        B -= delta;
    }
    else
    {
        B = 0;
    }
}

constexpr void RgbColor::Lighten(uint8_t delta)
{
    if (R < 255 - delta)
    {
        R += delta;
    }
    else
    {
        R = 255;
    }

    if (G < 255 - delta)
    {
        G += delta;
    }
    else
    {
        G = 255;
    }

    if (B < 255 - delta)
    {
        B += delta;
    }
    else
    {
        B = 255;
    }
}

constexpr RgbColor RgbColor::LinearBlend(const RgbColor& left, const RgbColor& right, float progress)
{
    return RgbColor( left.R + ((right.R - left.R) * progress),
        left.G + ((right.G - left.G) * progress),
        left.B + ((right.B - left.B) * progress));
}

constexpr RgbColor RgbColor::BilinearBlend(const RgbColor& c00,
    const RgbColor& c01,
    const RgbColor& c10,
    const RgbColor& c11,
    float x,
    float y)
{
    float v00 = (1.0f - x) * (1.0f - y);
    float v10 = x * (1.0f - y);
    float v01 = (1.0f - x) * y;
    float v11 = x * y;

    return RgbColor(
        c00.R * v00 + c10.R * v10 + c01.R * v01 + c11.R * v11,
        c00.G * v00 + c10.G * v10 + c01.G * v01 + c11.G * v11,
        c00.B * v00 + c10.B * v10 + c01.B * v01 + c11.B * v11);
}

constexpr uint32_t RgbColor::CalcTotalTenthMilliAmpere(const SettingsObject& settings) const
{
    auto total = 0;

    total += R * settings.RedTenthMilliAmpere / 255;
    total += G * settings.GreenTenthMilliAmpere / 255;
    total += B * settings.BlueTenthMilliAmpere / 255;

    return total;
}

constexpr uint8_t RgbColor::_elementDim(uint8_t value, uint8_t ratio)
{
    return (static_cast<uint16_t>(value) * (static_cast<uint16_t>(ratio) + 1)) >> 8;
}

constexpr uint8_t RgbColor::_elementBrighten(uint8_t value, uint8_t ratio)
{
    uint16_t element = ((static_cast<uint16_t>(value) + 1) << 8) / (static_cast<uint16_t>(ratio) + 1);

    if (element > 255)
    {
        element = 255;
    }
    else
    {
        element -= 1;
    }
    return element;
}

constexpr float RgbColor::_CalcColor(float p, float q, float t)
{
    if (t < 0.0f)
        t += 1.0f;
    if (t > 1.0f)
        t -= 1.0f;

    if (t < 1.0f / 6.0f)
        return p + (q - p) * 6.0f * t;

    if (t < 0.5f)
        return q;

    if (t < 2.0f / 3.0f)
        return p + ((q - p) * (2.0f / 3.0f - t) * 6.0f);

    return p;
}

constexpr RgbColor RgbColor::convertToRgbColor(const HslColor& color)
{
    float r{};
    float g{};
    float b{};

    float h = color.H;
    float s = color.S;
    float l = color.L;

    if (color.S == 0.0f || color.L == 0.0f)
    {
        r = g = b = l; // achromatic or black
    }
    else
    {
        float q = l < 0.5f ? l * (1.0f + s) : l + s - (l * s);
        float p = 2.0f * l - q;
        r = _CalcColor(p, q, h + 1.0f / 3.0f);
        g = _CalcColor(p, q, h);
        b = _CalcColor(p, q, h - 1.0f / 3.0f);
    }

    return RgbColor{(uint8_t)(r * 255.0f), (uint8_t)(g * 255.0f), (uint8_t)(b * 255.0f)};
}

constexpr RgbColor RgbColor::convertToRgbColor(HsbColor color)
{
    if (fuzzyCompare(color.S, 0.0f))
        return RgbColor{uint8_t(color.B), uint8_t(color.B), uint8_t(color.B)}; // achromatic or black

    RgbColor rgb{};

    if (color.H < 0.0f)
        color.H += 1.0f;
    else if (color.H >= 1.0f)
        color.H -= 1.0f;

    color.H *= 6.0f;
    int i = (int)color.H;
    float f = color.H - i;
    float q = color.B * (1.0f - color.S * f);
    float p = color.B * (1.0f - color.S);
    float t = color.B * (1.0f - color.S * (1.0f - f));

    switch (i)
    {
    case 0:
        rgb.R = color.B;
        rgb.G = t;
        rgb.B = p;
        break;
    case 1:
        rgb.R = q;
        rgb.G = color.B;
        rgb.B = p;
        break;
    case 2:
        rgb.R = p;
        rgb.G = color.B;
        rgb.B = t;
        break;
    case 3:
        rgb.R = p;
        rgb.G = q;
        rgb.B = color.B;
        break;
    case 4:
        rgb.R = t;
        rgb.G = p;
        rgb.B = color.B;
        break;
    default:
        rgb.R = color.B;
        rgb.G = p;
        rgb.B = q;
        break;
    }

    return rgb;
}

constexpr bool RgbColor::fuzzyCompare(double p1, double p2)
{
    return (std::abs(p1 - p2) * 1000000000000. <= std::min(std::abs(p1), std::abs(p2)));
}

constexpr bool RgbColor::fuzzyCompare(float p1, float p2)
{
    return (std::abs(p1 - p2) * 100000.f <= std::min(std::abs(p1), std::abs(p2)));
}
