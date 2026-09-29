#!/usr/bin/env python3
from PIL import Image
import sys

assert len(sys.argv) == 3, 'Usage: bmp.py INPUT OUTPUT'

# Open the PNG image with transparency
img = Image.open(sys.argv[1]).convert('RGBA')

# Save as BMP (Pillow handles 32-bit RGBA BMP automatically if mode is RGBA)
img.save(sys.argv[2], format='BMP')
