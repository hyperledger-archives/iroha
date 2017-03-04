from PIL import Image

block_len = 14
matrix_len = 13

CHAR_COL = 1
CHAR_ROW = 15

text = [
"""
o.....ooooooo
o.....o......
o.....o......
ooooooooooooo
......o.....o
......o.....o
......oooo..o
......o..o..o
......o..o..o
ooooooo..o..o
.........o..o
.........o..o
oooooooooo..o
""",
"""
o.ooooooooooo
o...........o
o...........o
o...........o
o...........o
o...........o
ooooooooooo.o
o...........o
o...........o
o...........o
o...........o
o...........o
o.ooooooooooo
""",
"""
o.ooo.ooooooo
o...o..o....o
o...o..o....o
o...o..o..o.o
o...o..o..o.o
ooo.o..o..o.o
o...o..o..o.o
o...o..o..o.o
o...o..o..o.o
o...o..o..o.o
o.ooo..o....o
.......o....o
ooooooooooooo
""",
"""
o.ooo.o..o..o
o...o.ooooooo
o...o....o...
o...o.ooooooo
o...o........
o...o.ooooooo
ooo.o.......o
o...o.ooooooo
o...o.......o
o...o.ooooooo
o...o.......o
o...o..oooo.o
o.ooo.......o
""",
"""
ooooooooooooo
......o......
o.ooooooooo.o
o.....o.....o
ooooo.o.ooooo
o.....o.....o
o.ooooooooo.o
o.....o.....o
...o..o......
...o..o......
...o..o......
....ooo.......
ooooo..oooooo
""",







"""
......o......
ooooooooooooo
o...........o
o..ooooooo..o
o...........o
o...........o
o..o.....o..o
o..ooooo.o..o
o..o.....o..o
o..o.....o..o
o..o.....o..o
o..o.ooooo..o
o...........o
""",
"""
ooooooooooooo
.............
oooooooo.oooo
...o.....o...
oo.o.oooooooo
.o.........o.
.ooooooooo.o.
.o.........o.
.o.ooooooooo.
............o
o...o...ooooo
ooooo...o....
....ooooooooo
""",
"""
ooooooo......
.o...o.......
.o...o..ooooo
.o...o.......
oo.oooo......
.o...o..ooooo
.o...o.......
.o...o.......
.o...o..ooooo
.o...o..o....
.o...o..o....
.o...o..o....
.o...o..ooooo
""",
"""
.........o...
oooooooo.oooo
o........o...
ooo.oooo.oooo
o...o....o...
o...o....oooo
o.oo..ooo....
o.......o....
o...o...ooooo
o...o...o....
o...o...o....
o...o...o....
o...o....oooo
""",
"""
o.o.o..o...o.
o.o.o.oo.o.oo
o.o.o.o..o..o
ooooo.ooooooo
..o...o..o..o
ooooo.o.o.o.o
o.o.o........
o.o.o.ooooooo
o.o.o.....o..
o.o.o.ooo.ooo
o.o.o.o.o.o..
o.o.o.o.o.ooo
o.o.o.o.o.o.o
""",
"""
o.o.o..o...o.
o.o.o.oo.oooo
o.o.o..o...o.
ooooo.ooooooo
..o....o...o.
ooooo.oo.oooo
o.o.o....o...
o.o.o..ooooo.
o.o.o....o...
o.o.o..ooooo.
o.o.o..o.o.o.
o.o.o.oo.o.oo
o.o.o..o...o.
"""
]

#　平成年月日株式会社ああ武宮誠東山奈央岡田隆天地会津會光皇萌以呂波水樹素子東京
char_color = ( 220, 221, 221)

img = Image.new("RGB", ( block_len*4 + 14*13 * CHAR_ROW, block_len*4 + 14*13 * CHAR_COL), ( 67, 67, 69))

def write_img( img, offset_row, offset_col, text):
    for x in range(0,matrix_len):
        for y in range(0,matrix_len):
            if text.split()[y][x] == "o":
                for px in range(block_len):
                    for py in range(block_len):
                        img.putpixel(
                           (block_len*x+px + offset_row,
                            block_len*y+py + offset_col),
                            char_color
                        )

count = 0
for v in text:
    write_img( img, block_len*4 + (block_len*2 + block_len * matrix_len) * count,  block_len*2, v)
    count += 1

img.save("new.png","PNG")
