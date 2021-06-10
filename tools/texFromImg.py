#! /usr/bin/python3

# this is a simple script that will take a picture (any format)
# as input and create a .tex (my own texture-format) file

# .tex will contain:
# header:
#   2bytes width 2bytes height
#   byte that stores which color-format (RGB 16bit 32bit etc)
# Pixel info:
#   depending on the color format the bytes describing each pixel
#   from left to right top to bottom (like reading this comment)

from sys import argv            # to use arguments
from os import path             # to validate existing file
from PIL import Image           # to load and get data from images

# argv[1]: path/to/file (relative to the directory of execution ".")
# argv[2]: output name

# checking for correct arguments

# are there 3 arguments?
if len(argv) != 3:
    print("ERROR: Wrong number of arguments")
    exit()

# checking if file exists
if path.isfile(argv[1]) is False:
    print("ERROR: File", argv[1], "doesnt exist")
    exit()

# checking if output-name already exists
if path.isfile(argv[2]) is True:
    print(argv[2], "already exists. Overwrite? y/n")

    userInput = ""

    while userInput.lower() != "y" and userInput.lower() != "n":
        userInput = input()

    if userInput.lower() == "n":
        exit()


image = Image.open(argv[1])         # opens image-file
rgb_im = image.convert("RGBA")      # converts image to rgb-file
texture = open(argv[2], "wb")       # creates output-file with given name

# printing what script will do...
print("Converting", argv[1])
print("Format: 32-Bit Color (RGBA)")
print("Dimensions:", image.size)

# saves dimensions in four bytes
texture.write((image.size[0]).to_bytes(2,byteorder="little"))
texture.write((image.size[1]).to_bytes(2,byteorder="little"))

# go through all pixels and write them to the file
for y in range(image.size[1]):
    for x in range(image.size[0]):
        r, g, b, a = rgb_im.getpixel((x, y))
        #print((x,y), r, g, b, a)
        texture.write((r).to_bytes(1,byteorder="big"))
        texture.write((g).to_bytes(1,byteorder="big"))
        texture.write((b).to_bytes(1,byteorder="big"))
        texture.write((a).to_bytes(1,byteorder="big"))

# closing all opened data
rgb_im.close()
image.close()
texture.close()
