#include "runtime/widescreen.h"

#include <cstring>

namespace widescreen
{
    float mapX(float x, Anchor anchor, float primCentre)
    {
        switch (anchor)
        {
        case Anchor::Full:
            return x;
        case Anchor::Centre:
            return x * kSqueeze;
        case Anchor::Left:
            return (x + 1.0f) * kSqueeze - 1.0f;
        case Anchor::Right:
            return 1.0f - (1.0f - x) * kSqueeze;
        case Anchor::InPlace:
            return primCentre + (x - primCentre) * kSqueeze;
        }
        return x;
    }

    float gameAspectFactor(bool wide, bool progressive)
    {
        // set_screen_wide_and_progressive: wide and interlaced 0x3FAAAAAA
        // (lui 0x3FAA; ori 0xAAAA), 4:3 and progressive 0.75, otherwise 1.
        uint32_t bits = 0x3F800000u;
        if (wide && !progressive)
            bits = 0x3FAAAAAAu;
        else if (!wide && progressive)
            bits = 0x3F400000u;
        float f;
        std::memcpy(&f, &bits, sizeof(f));
        return f;
    }

    uint32_t targetWidth(float renderScale, bool wide)
    {
        const float width = 512.0f * renderScale * (wide ? 4.0f / 3.0f : 1.0f);
        return static_cast<uint32_t>(width + 0.5f);
    }
}
