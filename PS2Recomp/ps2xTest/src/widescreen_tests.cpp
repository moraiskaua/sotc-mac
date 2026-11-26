#include "MiniTest.h"
#include "runtime/widescreen.h"

#include <cstdint>
#include <cmath>
#include <cstring>

// Widescreen 16:9 (worklist #20): where a 2D vertex goes on the wide screen,
// the game's own aspect factor and the wider render target, checked without
// a window. x is the native renderer's 2D NDC (-1 left, 1 right, 0 centre).
void register_widescreen_tests()
{
    MiniTest::Case("Widescreen", [](TestCase &tc)
                   {
        using widescreen::Anchor;
        using widescreen::mapX;

        tc.Run("full width: unchanged", [](TestCase &t)
               {
            t.Equals(mapX(-1.0f, Anchor::Full, 0.0f), -1.0f, "left edge");
            t.Equals(mapX(0.3f, Anchor::Full, 0.0f), 0.3f, "inside");
        });

        tc.Run("centre: squeezed by 3/4 about the middle", [](TestCase &t)
               {
            t.Equals(mapX(1.0f, Anchor::Centre, 0.0f), 0.75f, "right edge");
            t.Equals(mapX(-1.0f, Anchor::Centre, 0.0f), -0.75f, "left edge");
            t.Equals(mapX(0.0f, Anchor::Centre, 0.0f), 0.0f, "middle");
        });

        tc.Run("left anchor: the left edge stays", [](TestCase &t)
               {
            t.Equals(mapX(-1.0f, Anchor::Left, 0.0f), -1.0f, "left edge");
            t.Equals(mapX(1.0f, Anchor::Left, 0.0f), 0.5f, "right edge");
        });

        tc.Run("right anchor: the right edge stays", [](TestCase &t)
               {
            t.Equals(mapX(1.0f, Anchor::Right, 0.0f), 1.0f, "right edge");
            t.Equals(mapX(-1.0f, Anchor::Right, 0.0f), -0.5f, "left edge");
        });

        tc.Run("in place: squeezed about the primitive's own centre", [](TestCase &t)
               {
            t.IsTrue(std::fabs(mapX(0.6f, Anchor::InPlace, 0.5f) - 0.575f) < 1e-6f, "beside the centre");
            t.Equals(mapX(0.5f, Anchor::InPlace, 0.5f), 0.5f, "the centre itself");
        });

        tc.Run("the game's aspect factor (set_screen_wide_and_progressive)", [](TestCase &t)
               {
            const float wide = widescreen::gameAspectFactor(true, false);
            uint32_t bits = 0;
            std::memcpy(&bits, &wide, 4);
            // The game's own constant (lui 0x3FAA; ori 0xAAAA), not 4.0f / 3.0f (0x3FAAAAAB).
            t.Equals(bits, 0x3FAAAAAAu, "wide, interlaced: 0x3FAAAAAA");
            t.Equals(widescreen::gameAspectFactor(true, true), 1.0f, "wide, progressive");
            t.Equals(widescreen::gameAspectFactor(false, false), 1.0f, "4:3, interlaced");
            t.Equals(widescreen::gameAspectFactor(false, true), 0.75f, "4:3, progressive");
        });

        tc.Run("render target width: 4/3 wider when wide", [](TestCase &t)
               {
            t.Equals(widescreen::targetWidth(3.0f, false), 1536u, "4:3 at 3x");
            t.Equals(widescreen::targetWidth(3.0f, true), 2048u, "16:9 at 3x");
            t.Equals(widescreen::targetWidth(1.0f, true), 683u, "16:9 at 1x");
            t.Equals(widescreen::targetWidth(2.0f, true), 1365u, "16:9 at 2x");
            t.Equals(widescreen::targetWidth(4.0f, true), 2731u, "16:9 at 4x");
        }); });
}
