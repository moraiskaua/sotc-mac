#include "runtime/widescreen.h"

#include <algorithm>
#include <cmath>
#include <cstring>

namespace widescreen
{
    float screenAspect(int width, int height)
    {
        if (width <= 0 || height <= 0)
            return 16.0f / 9.0f;
        return std::clamp(static_cast<float>(width) / static_cast<float>(height), kNarrowest, kWidest);
    }

    float squeeze(float aspect) { return kNarrowest / aspect; }

    float mapX(float x, Anchor anchor, float primCentre, float squeeze)
    {
        switch (anchor)
        {
        case Anchor::Full:
            return x;
        case Anchor::Centre:
            return x * squeeze;
        case Anchor::Left:
            return (x + 1.0f) * squeeze - 1.0f;
        case Anchor::Right:
            return 1.0f - (1.0f - x) * squeeze;
        case Anchor::InPlace:
            return primCentre + (x - primCentre) * squeeze;
        }
        return x;
    }

    float gameAspectFactor(float aspect, bool progressive)
    {
        // set_screen_wide_and_progressive's table, bit for bit, at the two
        // aspects the game knows: 16:9 interlaced 0x3FAAAAAA (lui 0x3FAA; ori
        // 0xAAAA), 16:9 progressive 1, 4:3 interlaced 1, 4:3 progressive 0.75.
        // Any other width: aspect / (4/3), times 0.75 progressive.
        uint32_t bits = 0;
        if (std::fabs(aspect - 16.0f / 9.0f) < 1e-4f)
            bits = progressive ? 0x3F800000u : 0x3FAAAAAAu;
        else if (std::fabs(aspect - kNarrowest) < 1e-4f)
            bits = progressive ? 0x3F400000u : 0x3F800000u;
        else
            return aspect / kNarrowest * (progressive ? 0.75f : 1.0f);
        float f;
        std::memcpy(&f, &bits, sizeof(f));
        return f;
    }

    uint32_t targetWidth(float renderScale, float aspect)
    {
        return static_cast<uint32_t>(512.0f * renderScale * (aspect / kNarrowest) + 0.5f);
    }
}
