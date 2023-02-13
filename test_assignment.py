from PIL import Image
import subprocess

base = "make; ./part1 image.ppm transformed.ppm;"
subprocess.run(base, shell=True)

im = Image.open("transformed.ppm")
im.save("transformed.jpeg")