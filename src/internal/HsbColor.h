/*-------------------------------------------------------------------------
HsbColor provides a color object that can be directly consumed by NeoPixelBus

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

struct RgbColor;

// ------------------------------------------------------------------------
// HsbColor represents a color object that is represented by Hue, Saturation, Brightness
// component values.  It contains helpful color routines to manipulate the
// color.
// ------------------------------------------------------------------------
struct HsbColor
{
    // ------------------------------------------------------------------------
    // Construct a HsbColor that will have its values set in latter operations
    // ------------------------------------------------------------------------
    constexpr HsbColor();

    // ------------------------------------------------------------------------
    // Construct a HsbColor using H, S, B values (0.0 - 1.0)
    // ------------------------------------------------------------------------
    constexpr HsbColor(float h, float s, float b);

    // ------------------------------------------------------------------------
    // Construct a HsbColor using HsbColor
    // ------------------------------------------------------------------------
    constexpr HsbColor(const HsbColor& color);

    // ------------------------------------------------------------------------
    // Construct a HsbColor using RgbColor
    // ------------------------------------------------------------------------
    constexpr HsbColor(const RgbColor& color);

    // ------------------------------------------------------------------------
    // LinearBlend between two colors by the amount defined by progress variable
    // left - the color to start the blend at
    // right - the color to end the blend at
    // progress - (0.0 - 1.0) value where 0.0 will return left and 1.0 will return right
    //     and a value between will blend the color weighted linearly between them
    // ------------------------------------------------------------------------
    template <typename T_NEOHUEBLEND>
    static constexpr HsbColor LinearBlend(const HsbColor& left,
        const HsbColor& right,
        float progress);

    // ------------------------------------------------------------------------
    // BilinearBlend between four colors by the amount defined by 2d variable
    // c00 - upper left quadrant color
    // c01 - upper right quadrant color
    // c10 - lower left quadrant color
    // c11 - lower right quadrant color
    // x - unit value (0.0 - 1.0) that defines the blend progress in horizontal space
    // y - unit value (0.0 - 1.0) that defines the blend progress in vertical space
    // ------------------------------------------------------------------------
    template <typename T_NEOHUEBLEND>
    static constexpr HsbColor BilinearBlend(const HsbColor& c00,
        const HsbColor& c01,
        const HsbColor& c10,
        const HsbColor& c11,
        float x,
        float y);

    // ------------------------------------------------------------------------
    // Hue, Saturation, Brightness color members
    // ------------------------------------------------------------------------

    float H{};
    float S{};
    float B{};

private:
    static constexpr HsbColor convertToHsbColor(const RgbColor& color);
};

#include "RgbColor.h"

constexpr HsbColor::HsbColor() = default;

constexpr HsbColor::HsbColor(float h, float s, float b) :
    H{h}, S{s}, B{b}
{
}

constexpr HsbColor::HsbColor(const HsbColor& color) = default;

constexpr HsbColor::HsbColor(const RgbColor& color) :
    HsbColor{convertToHsbColor(color)}
{
}

//template <typename T_NEOHUEBLEND>
//static constexpr HsbColor HsbColor::LinearBlend<T_NEOHUEBLEND>(const HsbColor& left,
//    const HsbColor& right,
//    float progress)
//{
//    return HsbColor(T_NEOHUEBLEND::HueBlend(left.H, right.H, progress),
//        left.S + ((right.S - left.S) * progress),
//        left.B + ((right.B - left.B) * progress));
//}

//template <typename T_NEOHUEBLEND>
//static constexpr HsbColor HsbColor::BilinearBlend<T_NEOHUEBLEND>(const HsbColor& c00,
//    const HsbColor& c01,
//    const HsbColor& c10,
//    const HsbColor& c11,
//    float x,
//    float y)
//{
//    float v00 = (1.0f - x) * (1.0f - y);
//    float v10 = x * (1.0f - y);
//    float v01 = (1.0f - x) * y;
//    float v11 = x * y;

//    return HsbColor(
//        T_NEOHUEBLEND::HueBlend(
//            T_NEOHUEBLEND::HueBlend(c00.H, c10.H, x),
//            T_NEOHUEBLEND::HueBlend(c01.H, c11.H, x),
//            y),
//        c00.S * v00 + c10.S * v10 + c01.S * v01 + c11.S * v11,
//        c00.B * v00 + c10.B * v10 + c01.B * v01 + c11.B * v11);
//}

constexpr HsbColor HsbColor::convertToHsbColor(const RgbColor& color)
{
    // convert colors to float between (0.0 - 1.0)
    float r = color.R / 255.0f;
    float g = color.G / 255.0f;
    float b = color.B / 255.0f;

    float max = (r > g && r > b) ? r : (g > b) ? g : b;
    float min = (r < g && r < b) ? r : (g < b) ? g : b;

    float d = max - min;

    HsbColor hsb{};
    hsb.H = 0.f;
    hsb.B = max;
    hsb.S = (hsb.B == 0.0f) ? 0 : (d / hsb.B);

    if (d != 0.0f)
    {
        if (r == max)
        {
            hsb.H = (g - b) / d + (g < b ? 6.0f : 0.0f);
        }
        else if (g == max)
        {
            hsb.H = (b - r) / d + 2.0f;
        }
        else
        {
            hsb.H = (r - g) / d + 4.0f;
        }
        hsb.H /= 6.0f;
    }

    return hsb;
}
