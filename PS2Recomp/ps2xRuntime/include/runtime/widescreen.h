#pragma once

#include <cstdint>

// Widescreen 16:9 (worklist #20, docs/widescreen.md; Mac only).
//
// The game has a 16:9 mode of its own (option 2, set_screen_wide_and_progressive):
// it widens its projection and its culling and keeps drawing into the same
// 512 x 448 frame, anamorphic, for a wide TV to stretch. The native renderer
// draws that frame 4/3 wider and shows it at 16:9; the 2D (which the game
// leaves for the TV to stretch) is squeezed back to its shape here, each
// element by how it is anchored on the wide screen.
namespace widescreen
{
    // Where a 2D element sits on the 16:9 screen.
    enum class Anchor : uint8_t
    {
        Full,    // the whole width (fades, cinema bars, screen layers): unchanged
        Centre,  // squeezed about the screen's middle (menus, text, subtitles, logos)
        Left,    // squeezed, its left edge kept (HUD on the left)
        Right,   // squeezed, its right edge kept (HUD on the right)
        InPlace, // squeezed about its own centre (2D placed at a point of the scene)
    };

    // A 4:3 picture's width in the 16:9 one.
    constexpr float kSqueeze = 0.75f;

    // x: the 2D NDC (-1 left, 0 centre, 1 right) the game drew at; primCentre:
    // the primitive's own centre (InPlace only). The x on the wide screen.
    float mapX(float x, Anchor anchor, float primCentre);

    // The factor set_screen_wide_and_progressive (0x1164468) writes at
    // 0x118FE64 for (wide, progressive), bit for bit.
    float gameAspectFactor(bool wide, bool progressive);

    // The native renderer's target width: 512 x scale, 4/3 of it when wide.
    uint32_t targetWidth(float renderScale, bool wide);
}
