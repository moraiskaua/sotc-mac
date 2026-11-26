# Widescreen (worklist #20)

Mac only. Graphics "Widescreen" ("Tela larga", `SOTC_WIDESCREEN=1`, saved in
`settings.cfg`, off by default, changes live).

## What it does

The game has a 16:9 mode of its own: option 2 (`_optionWorkChk(2)`, the word at
`0x11DF750`), read by `setAspect` (0x14195D8), which calls
`set_screen_wide_and_progressive` (0x1164468). That writes an aspect factor at
`0x118FE64` (16:9 interlaced `0x3FAAAAAA`, 16:9 progressive 1, 4:3 interlaced 1,
4:3 progressive 0.75); `get_aspect_ratio` (0x1164500) multiplies the camera's
aspect by it every frame, so the projection (camera context +0x40) and the
culling widen together. The game keeps drawing into its 512 x 448 frame,
anamorphic, for a wide TV to stretch.

With the option on, `syncWidescreen` (game/overrides/sotc_native.cpp) writes, at
each frame's change:

- option 2 = 1 (off: 0; the option wins over the game's own menu);
- the factor for the window's aspect, 4:3 to 21:9 (`widescreen::screenAspect`),
  as the game writes it at 16:9 and 4:3, `aspect / (4/3)` otherwise
  (`widescreen::gameAspectFactor`), so a maximised window or a full screen has
  no bars;
- the native renderer's target `512 x scale x aspect / (4/3)` wide
  (`widescreen::targetWidth`), shown at that aspect (`drawHostFrame`).

Values are written only when they differ: off with a 4:3 save changes no byte.
F11 or Alt+Enter: borderless full screen. When the option turns on in a window
narrower than 16:9 (the default one is the PS2's frame), the window is made
16:9 at the same height.

## The 2D

The game leaves its 2D (menus, text, HUD) for the TV to stretch, so it comes out
a third too wide. Every 2D draw the game makes through `gsReg` (0x1C5B00) is
squeezed back by `4/3 / aspect` in x (`widescreen::mapX`), by an anchor:

| Anchor | x on the wide screen | Used for |
| --- | --- | --- |
| Full | unchanged | fades, cinema bars, any triangle spanning the whole width (subtitle band, title backdrop) |
| Centre | squeezed about the middle | menus, title, pause, map, subtitles, text windows, logos |
| Left / Right | squeezed, the left / right edge kept | the HUD gauges, the loading indicator |
| InPlace | squeezed about its own centre | (kept for 2D placed at a scene point; none found) |

2D that does not go through `gsReg` (the immediate gl* layer: heat haze strips,
particle quads; the screen effects' copies) is left alone: it is placed by the
game's own wide projection already.

Each corner of a sprite is written by a different `gsReg` call
(`spriteDrawSprite`: 0x10A3C68, 0x10A3D00, 0x10A3D94, 0x10A3E2C), so the anchor
comes from the caller's function (an address range), never from one return
address. The HUD gauges' anchor is picked once per gauge from where it is drawn
(left third: Left, right third: Right, otherwise Centre) and kept while the
gauge stays on screen, so a growing or sliding gauge never jumps side.

## Census (26/11/2026)

`SOTC_GSREG_CENSUS=1` prints, every 120 frames, `[ws-census] ra tex n x y`: the
box (2D NDC, -1..1) each (gsReg caller, texture) pair drew. Route: title, New
Game, intro skipped, the shrine on foot, pause (map), R1.

| Caller (return addresses) | What | Box | Anchor |
| --- | --- | --- | --- |
| 0x108A024, 0x108A060 | fadeScreen family | full screen | Full |
| 0x108A380, 0x108A3A4, 0x108A524, 0x108A530 | fadeScreen / drawDemoBar | full screen | Full |
| 0x108A67C .. 0x108A6DC | cinema bars (drawDemoBar) | full width, top and bottom | Full |
| 0x10A3C68 .. 0x10A40D4 | sprite family: title logo and menu, the corner text, the map (beyond the screen, +-2.7), the pause box and hints | various | Centre |
| 0x134B638, 0x134B698 | subtitle band | full width | Full (by its span) |
| 0x13FDD40 | text windows (hints) | bottom middle | Centre |
| 0x14034EC .. 0x140352C | title backdrop | full screen | Full (by its span) |
| 0x1407728 | title (a point) | middle | Centre |
| 0x1C2FFC .. 0x1C3080 | loading indicator | bottom right | Right |
| 0x13F6B20, 0x13F7178, 0x13F75D0 | HUD gauges (from docs/native_renderer_map.md; not on screen on this route) | | Left / Right by where drawn |

## Limits

- Vita: off.
- The emulated picture (F9, `SOTC_NATIVE=0`) is shown at the wide aspect with
  its 2D stretched, as a wide TV showed the PS2.
- Narrower than 4:3: bars (the game does not narrow its view).
