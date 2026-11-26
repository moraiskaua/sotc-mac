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
            t.Equals(mapX(-1.0f, Anchor::Full, 0.0f, 0.75f), -1.0f, "left edge");
            t.Equals(mapX(0.3f, Anchor::Full, 0.0f, 0.75f), 0.3f, "inside");
        });

        tc.Run("centre: squeezed by 3/4 about the middle", [](TestCase &t)
               {
            t.Equals(mapX(1.0f, Anchor::Centre, 0.0f, 0.75f), 0.75f, "right edge");
            t.Equals(mapX(-1.0f, Anchor::Centre, 0.0f, 0.75f), -0.75f, "left edge");
            t.Equals(mapX(0.0f, Anchor::Centre, 0.0f, 0.75f), 0.0f, "middle");
        });

        tc.Run("left anchor: the left edge stays", [](TestCase &t)
               {
            t.Equals(mapX(-1.0f, Anchor::Left, 0.0f, 0.75f), -1.0f, "left edge");
            t.Equals(mapX(1.0f, Anchor::Left, 0.0f, 0.75f), 0.5f, "right edge");
        });

        tc.Run("right anchor: the right edge stays", [](TestCase &t)
               {
            t.Equals(mapX(1.0f, Anchor::Right, 0.0f, 0.75f), 1.0f, "right edge");
            t.Equals(mapX(-1.0f, Anchor::Right, 0.0f, 0.75f), -0.5f, "left edge");
        });

        tc.Run("in place: squeezed about the primitive's own centre", [](TestCase &t)
               {
            t.IsTrue(std::fabs(mapX(0.6f, Anchor::InPlace, 0.5f, 0.75f) - 0.575f) < 1e-6f, "beside the centre");
            t.Equals(mapX(0.5f, Anchor::InPlace, 0.5f, 0.75f), 0.5f, "the centre itself");
        });

        tc.Run("the game's aspect factor (set_screen_wide_and_progressive)", [](TestCase &t)
               {
            const float wide = widescreen::gameAspectFactor(16.0f / 9.0f, false);
            uint32_t bits = 0;
            std::memcpy(&bits, &wide, 4);
            // At 16:9 the game's own constant (lui 0x3FAA; ori 0xAAAA), not 4.0f / 3.0f (0x3FAAAAAB).
            t.Equals(bits, 0x3FAAAAAAu, "16:9, interlaced: 0x3FAAAAAA");
            t.Equals(widescreen::gameAspectFactor(16.0f / 9.0f, true), 1.0f, "16:9, progressive");
            t.Equals(widescreen::gameAspectFactor(4.0f / 3.0f, false), 1.0f, "4:3, interlaced");
            t.Equals(widescreen::gameAspectFactor(4.0f / 3.0f, true), 0.75f, "4:3, progressive");
            t.IsTrue(std::fabs(widescreen::gameAspectFactor(16.0f / 10.0f, false) - 1.2f) < 1e-6f, "16:10: wider than 4:3 by 1.2");
        });

        tc.Run("the screen's aspect, clamped to 4:3 .. 21:9", [](TestCase &t)
               {
            t.IsTrue(std::fabs(widescreen::screenAspect(1920, 1018) - 1920.0f / 1018.0f) < 1e-6f, "a maximised window, as it is");
            t.Equals(widescreen::screenAspect(1024, 1024), 4.0f / 3.0f, "taller than 4:3: 4:3");
            t.Equals(widescreen::screenAspect(5120, 1440), 21.0f / 9.0f, "wider than 21:9: 21:9");
            t.Equals(widescreen::screenAspect(0, 0), 16.0f / 9.0f, "no size yet: 16:9");
        });

        tc.Run("the 2D squeeze: a 4:3 picture's width in the wide one", [](TestCase &t)
               {
            t.Equals(widescreen::squeeze(16.0f / 9.0f), 0.75f, "16:9");
            t.Equals(widescreen::squeeze(4.0f / 3.0f), 1.0f, "4:3: none");
            t.Equals(mapX(1.0f, Anchor::Centre, 0.0f, 0.5f), 0.5f, "centre at a 0.5 squeeze");
        });

        tc.Run("render target width: as wide as the screen's aspect", [](TestCase &t)
               {
            t.Equals(widescreen::targetWidth(3.0f, 4.0f / 3.0f), 1536u, "4:3 at 3x");
            t.Equals(widescreen::targetWidth(3.0f, 16.0f / 9.0f), 2048u, "16:9 at 3x");
            t.Equals(widescreen::targetWidth(1.0f, 16.0f / 9.0f), 683u, "16:9 at 1x");
            t.Equals(widescreen::targetWidth(2.0f, 16.0f / 9.0f), 1365u, "16:9 at 2x");
            t.Equals(widescreen::targetWidth(4.0f, 16.0f / 9.0f), 2731u, "16:9 at 4x");
            t.Equals(widescreen::targetWidth(3.0f, 16.0f / 10.0f), 1843u, "16:10 at 3x");
        });

        tc.Run("caller table: an address range, low inclusive, high exclusive", [](TestCase &t)
               {
            const widescreen::Entry table[] = {
                {0x0108A000u, 0x0108A700u, Anchor::Full, false},
                {0x013F6B20u, 0x013F8958u, Anchor::Centre, true},
            };
            const widescreen::Entry *e = widescreen::entryFor(0x0108A000u, table, 2);
            t.IsTrue(e == &table[0], "the low end is in");
            t.IsTrue(widescreen::entryFor(0x0108A700u, table, 2) == nullptr, "the high end is out");
            t.IsTrue(widescreen::entryFor(0x013F7178u, table, 2) == &table[1], "a gauge's call");
        });

        tc.Run("unknown caller: no entry (drawn as Centre)", [](TestCase &t)
               {
            const widescreen::Entry table[] = {{0x0108A000u, 0x0108A700u, Anchor::Full, false}};
            t.IsTrue(widescreen::entryFor(0x01234567u, table, 1) == nullptr, "not in the table");
        });

        tc.Run("edge anchor: by the third the element sits in", [](TestCase &t)
               {
            t.IsTrue(widescreen::edgeFor(-0.8f) == Anchor::Left, "left third");
            t.IsTrue(widescreen::edgeFor(0.8f) == Anchor::Right, "right third");
            t.IsTrue(widescreen::edgeFor(0.1f) == Anchor::Centre, "middle");
        });

        tc.Run("full width: a triangle from edge to edge, not one beyond the screen", [](TestCase &t)
               {
            t.IsTrue(widescreen::spansWidth(-1.0f, 1.0f), "edge to edge");
            t.IsTrue(widescreen::spansWidth(-1.001f, 0.998f), "edge to edge, rounded");
            t.IsTrue(!widescreen::spansWidth(-0.9f, 1.0f), "short of the left edge");
            t.IsTrue(!widescreen::spansWidth(-2.7f, 2.5f), "the map, wider than the screen");
        }); });
}
