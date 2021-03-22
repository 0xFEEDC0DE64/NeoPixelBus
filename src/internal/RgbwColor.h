/*-------------------------------------------------------------------------
RgbwColor provides a color object that can be directly consumed by NeoPixelBus

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

#include "NeoSettings.h"

struct RgbColor;
struct HslColor;
struct HsbColor;

// ------------------------------------------------------------------------
// RgbwColor represents a color object that is represented by Red, Green, Blue
// component values and an extra White component.  It contains helpful color 
// routines to manipulate the color.
// ------------------------------------------------------------------------
struct RgbwColor
{
    using SettingsObject = NeoRgbwCurrentSettings;

    // ------------------------------------------------------------------------
    // Construct a RgbwColor that will have its values set in latter operations
    // ------------------------------------------------------------------------
    constexpr RgbwColor();

    // ------------------------------------------------------------------------
    // Construct a RgbwColor using R, G, B, W values (0-255)
    // ------------------------------------------------------------------------
    constexpr RgbwColor(uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0);

    // ------------------------------------------------------------------------
    // Construct a RgbColor using a single brightness value (0-255)
    // This works well for creating gray tone colors
    // (0) = black, (255) = white, (128) = gray
    // ------------------------------------------------------------------------
    constexpr RgbwColor(uint8_t brightness);

    // ------------------------------------------------------------------------
    // Construct a RgbwColor using RgbwColor
    // ------------------------------------------------------------------------
    constexpr RgbwColor(const RgbwColor& color);

    // ------------------------------------------------------------------------
    // Construct a RgbwColor using RgbColor
    // ------------------------------------------------------------------------
    constexpr RgbwColor(const RgbColor& color);

    // ------------------------------------------------------------------------
    // Construct a RgbwColor using HslColor
    // ------------------------------------------------------------------------
    constexpr RgbwColor(const HslColor& color);

    // ------------------------------------------------------------------------
    // Construct a RgbwColor using HsbColor
    // ------------------------------------------------------------------------
    constexpr RgbwColor(const HsbColor& color);

    // ------------------------------------------------------------------------
    // Comparison operators
    // ------------------------------------------------------------------------
    constexpr bool operator==(const RgbwColor& other) const;

    constexpr bool operator!=(const RgbwColor& other) const;

    // ------------------------------------------------------------------------
    // Returns if the color is grey, all values are equal other than white
    // ------------------------------------------------------------------------
    constexpr bool IsMonotone() const;

    // ------------------------------------------------------------------------
    // Returns if the color components are all zero, the white component maybe 
    // anything
    // ------------------------------------------------------------------------
    constexpr bool IsColorLess() const;

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
    constexpr RgbwColor Dim(uint8_t ratio) const;

    // ------------------------------------------------------------------------
    // Brighten will return a new color that is blended to white with the given ratio
    // ratio - (0-255) where 255 will return the original color and 0 will return white
    // 
    // NOTE: This is a simple linear blend
    // ------------------------------------------------------------------------
    constexpr RgbwColor Brighten(uint8_t ratio) const;

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
    static constexpr RgbwColor LinearBlend(const RgbwColor& left, const RgbwColor& right, float progress);

    // ------------------------------------------------------------------------
    // BilinearBlend between four colors by the amount defined by 2d variable
    // c00 - upper left quadrant color
    // c01 - upper right quadrant color
    // c10 - lower left quadrant color
    // c11 - lower right quadrant color
    // x - unit value (0.0 - 1.0) that defines the blend progress in horizontal space
    // y - unit value (0.0 - 1.0) that defines the blend progress in vertical space
    // ------------------------------------------------------------------------
    static constexpr RgbwColor BilinearBlend(const RgbwColor& c00,
        const RgbwColor& c01, 
        const RgbwColor& c10, 
        const RgbwColor& c11, 
        float x, 
        float y);

    constexpr uint16_t CalcTotalTenthMilliAmpere(const SettingsObject& settings) const;

    // ------------------------------------------------------------------------
    // Red, Green, Blue, White color members (0-255) where 
    // (0,0,0,0) is black and (255,255,255, 0) and (0,0,0,255) is white
    // Note (255,255,255,255) is extreme bright white
    // ------------------------------------------------------------------------
    uint8_t R{};
    uint8_t G{};
    uint8_t B{};
    uint8_t W{};

private:
    inline static constexpr uint8_t _elementDim(uint8_t value, uint8_t ratio);
    inline static constexpr uint8_t _elementBrighten(uint8_t value, uint8_t ratio);
};

#include "RgbColor.h"
#include "HslColor.h"
#include "HsbColor.h"

constexpr RgbwColor::RgbwColor() = default;

constexpr RgbwColor::RgbwColor(uint8_t r, uint8_t g, uint8_t b, uint8_t w) :
    R{r}, G{g}, B{b}, W{w}
{
}

constexpr RgbwColor::RgbwColor(uint8_t brightness) :
    R{}, G{}, B{}, W{brightness}
{
}

constexpr RgbwColor::RgbwColor(const RgbwColor& color) = default;

constexpr RgbwColor::RgbwColor(const RgbColor& color) :
    R{color.R},
    G{color.G},
    B{color.B},
    W{}
{
}

constexpr RgbwColor::RgbwColor(const HslColor& color) :
    RgbwColor{RgbColor{color}}
{
}

constexpr RgbwColor::RgbwColor(const HsbColor& color) :
    RgbwColor{RgbColor{color}}
{
}

constexpr bool RgbwColor::operator==(const RgbwColor& other) const
{
    return (R == other.R && G == other.G && B == other.B && W == other.W);
}

constexpr bool RgbwColor::operator!=(const RgbwColor& other) const
{
    return !(*this == other);
}

constexpr bool RgbwColor::IsMonotone() const
{
    return (R == B && R == G);
}

constexpr bool RgbwColor::IsColorLess() const
{
    return (R == 0 && B == 0 && G == 0);
}

constexpr uint8_t RgbwColor::CalculateBrightness() const
{
    uint8_t colorB = (uint8_t)(((uint16_t)R + (uint16_t)G + (uint16_t)B) / 3);
    if (W > colorB)
    {
        return W;
    }
    else
    {
        return colorB;
    }
}

constexpr RgbwColor RgbwColor::Dim(uint8_t ratio) const
{
    // specifically avoids float math
    return RgbwColor{_elementDim(R, ratio), _elementDim(G, ratio), _elementDim(B, ratio), _elementDim(W, ratio)};
}

constexpr RgbwColor RgbwColor::Brighten(uint8_t ratio) const
{
    // specifically avoids float math
    return RgbwColor{_elementBrighten(R, ratio), _elementBrighten(G, ratio), _elementBrighten(B, ratio), _elementBrighten(W, ratio)};
}

constexpr void RgbwColor::Darken(uint8_t delta)
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

    if (W > delta)
    {
        W -= delta;
    }
    else
    {
        W = 0;
    }
}

constexpr void RgbwColor::Lighten(uint8_t delta)
{
    if (IsColorLess())
    {
        if (W < 255 - delta)
        {
            W += delta;
        }
        else
        {
            W = 255;
        }
    }
    else
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
}

constexpr RgbwColor RgbwColor::LinearBlend(const RgbwColor& left, const RgbwColor& right, float progress)
{
    return RgbwColor( left.R + ((right.R - left.R) * progress),
        left.G + ((right.G - left.G) * progress),
        left.B + ((right.B - left.B) * progress),
        left.W + ((right.W - left.W) * progress) );
}

constexpr RgbwColor RgbwColor::BilinearBlend(const RgbwColor& c00,
        const RgbwColor& c01,
        const RgbwColor& c10,
        const RgbwColor& c11,
        float x,
        float y)
{
    float v00 = (1.0f - x) * (1.0f - y);
    float v10 = x * (1.0f - y);
    float v01 = (1.0f - x) * y;
    float v11 = x * y;

    return RgbwColor(
        c00.R * v00 + c10.R * v10 + c01.R * v01 + c11.R * v11,
        c00.G * v00 + c10.G * v10 + c01.G * v01 + c11.G * v11,
        c00.B * v00 + c10.B * v10 + c01.B * v01 + c11.B * v11,
        c00.W * v00 + c10.W * v10 + c01.W * v01 + c11.W * v11 );
}

constexpr uint16_t RgbwColor::CalcTotalTenthMilliAmpere(const SettingsObject& settings) const
{
    auto total = 0;

    total += R * settings.RedTenthMilliAmpere / 255;
    total += G * settings.GreenTenthMilliAmpere / 255;
    total += B * settings.BlueTenthMilliAmpere / 255;
    total += W * settings.WhiteCurrent / 255;

    return total;
}

constexpr uint8_t RgbwColor::_elementDim(uint8_t value, uint8_t ratio)
{
    return (static_cast<uint16_t>(value) * (static_cast<uint16_t>(ratio) + 1)) >> 8;
}

constexpr uint8_t RgbwColor::_elementBrighten(uint8_t value, uint8_t ratio)
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
