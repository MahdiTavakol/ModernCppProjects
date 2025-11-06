#!/usr/bin/env python3
"""
Usage:
    python make_animation.py <folder> <first> <stride> <last> <output>

Example:
    python make_animation.py temp 0 5 200 animation.mp4
"""

import sys
from pathlib import Path
import imageio.v3 as iio

def main():
    if len(sys.argv) < 6:
        print("Usage: python make_animation.py <folder> <first> <stride> <last> <output>")
        sys.exit(1)

    folder = Path(sys.argv[1])
    first = int(sys.argv[2])
    stride = int(sys.argv[3])
    last = int(sys.argv[4])
    output = Path(sys.argv[5])

    fps = 30  # adjust if needed

    if not folder.exists():
        print(f"Error: folder '{folder}' not found.")
        sys.exit(1)

    frame_paths = []
    for i in range(first, last + 1, stride):
        fname = folder / f"frame-{i}.svg"
        if fname.exists():
            frame_paths.append(fname)
        else:
            print(f"Warning: {fname} not found, skipping...")

    if not frame_paths:
        print("No frames found. Exiting.")
        sys.exit(1)

    print(f"Reading {len(frame_paths)} SVG files...")
    frames = []
    for idx, path in enumerate(frame_paths, 1):
        try:
            img = iio.imread(path)  # imageio handles SVG via ffmpeg/librsvg if available
            frames.append(img)
            if idx % 20 == 0 or idx == len(frame_paths):
                print(f"  Loaded {idx}/{len(frame_paths)}")
        except Exception as e:
            print(f"Error reading {path}: {e}")

    if output.suffix.lower() == ".mp4":
        print(f"Writing MP4 video {output} at {fps} fps...")
        iio.imwrite(output, frames, fps=fps, codec="libx264", quality=8)
    elif output.suffix.lower() == ".gif":
        print(f"Writing GIF {output} at {fps} fps...")
        duration = 1.0 / fps
        iio.imwrite(output, frames, duration=duration, loop=0)
    else:
        print("Output must end with .mp4 or .gif")
        sys.exit(1)

    print("Done.")

if __name__ == "__main__":
    main()