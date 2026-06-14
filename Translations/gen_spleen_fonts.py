#!/usr/bin/env python3
"""
Convert Spleen BDF bitmap fonts into the column-major, 8px-strip byte format
used by IronOS' OLED driver (see OLED::drawArea / drawChar).

A glyph of W x H pixels (H a multiple of 8) is encoded as H/8 strips, each strip
being W bytes (one per column), the LSB of a byte being the top-most pixel of
that 8px strip. This matches the existing font06 (6x8) / font12 (12x16) tables.

This tool is intentionally standalone so the conversion can be sanity-checked by
rendering glyphs back to ASCII art (`--preview`) without needing hardware. A
later step uses `glyph_to_bytes()` to emit C tables.
"""
import argparse
import os
from typing import Optional

from bdflib import reader as bdfreader
from bdflib.model import Font

HERE = os.path.dirname(os.path.abspath(__file__))

# (filename, cell width, cell height, font ascent) for each Spleen size we use.
FONTS = {
    "8x16": ("spleen/spleen-8x16.bdf", 8, 16, 12),
    "12x24": ("spleen/spleen-12x24.bdf", 12, 24, 19),
}


def load_font(path: str) -> Font:
    with open(os.path.join(HERE, path), "rb") as f:
        return bdfreader.read_bdf(f)


def glyph_to_bytes(font: Font, ch: str, dst_w: int, dst_h: int, ascent: int) -> Optional[bytes]:
    """Encode a single character into IronOS column-major strip bytes."""
    try:
        glyph = font[ord(ch)]
    except KeyError:
        return None
    data = glyph.data
    src_left, src_bottom, src_w, src_h = glyph.get_bounding_box()

    # (x, y) are cell coordinates with origin at the top-left of the dst cell.
    def get_cell(x: int, y: int) -> bool:
        adj_x = x - src_left
        if adj_x < 0 or adj_x >= src_w:
            return False
        # Place the glyph using the font baseline: the baseline sits `ascent`
        # rows below the top of the cell, and the bbox bottom is `src_bottom`
        # above the baseline (negative for descenders).
        top_offset = ascent - src_h - src_bottom
        adj_y = y - top_offset
        if adj_y < 0 or adj_y >= src_h:
            return False
        # data is stored bottom row first; LSB is the right-most pixel.
        return bool(data[src_h - adj_y - 1] & (1 << (src_w - adj_x - 1)))

    bs = bytearray()
    for block in range(dst_h // 8):
        for c in range(dst_w):
            b = 0
            for r in range(8):
                if get_cell(c, r + 8 * block):
                    b |= 0x01 << r
            bs.append(b)
    return bytes(bs)


def render_ascii(bs: bytes, dst_w: int, dst_h: int) -> str:
    """Render encoded bytes back to ASCII art to verify the conversion."""
    lines = []
    for y in range(dst_h):
        block = y // 8
        bit = y % 8
        row = []
        for c in range(dst_w):
            byte = bs[block * dst_w + c]
            row.append("##" if (byte >> bit) & 1 else "..")
        lines.append("".join(row))
    return "\n".join(lines)


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--size", choices=sorted(FONTS), default="12x24")
    ap.add_argument("--preview", default="0123456789", help="characters to render as ASCII art")
    args = ap.parse_args()

    path, dst_w, dst_h, ascent = FONTS[args.size]
    font = load_font(path)
    print(f"# Spleen {args.size} (cell {dst_w}x{dst_h}, ascent {ascent}) -> {dst_w * (dst_h // 8)} bytes/glyph")
    for ch in args.preview:
        bs = glyph_to_bytes(font, ch, dst_w, dst_h, ascent)
        if bs is None:
            print(f"\n[{ch!r}] MISSING")
            continue
        print(f"\n[{ch!r}] ({len(bs)} bytes)")
        print(render_ascii(bs, dst_w, dst_h))


if __name__ == "__main__":
    main()
