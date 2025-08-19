#!/usr/bin/env python3
import sys

from pyfiglet import Figlet

# source .venv/bin/activate
# python3 ./utils/shell-banner-gen.py Hello, world!

FONTS = [
    "3-d",
    "5lineoblique",
    "alligator",
    "arrows",
    "avatar",
    "banner3-D",
    "basic",
    "big",
    "block",
    "chunky",
    "crawford",
    "cricket",
    "cursive",
    "cyberlarge",
    "doom",
    "dotmatrix",
    "epic",
]


def main():
    if len(sys.argv) < 2:
        print("Usage: python3 figlet_print.py <text>")
        sys.exit(1)

    text = " ".join(sys.argv[1:])

    for font_name in FONTS:
        fig = Figlet(font=font_name)
        print(f"Font: {font_name}\n")
        print(fig.renderText(text))
        print("=" * 80, "\n")


if __name__ == "__main__":
    main()
