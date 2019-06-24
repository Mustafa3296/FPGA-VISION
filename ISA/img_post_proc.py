#!/usr/bin/env python3

import sys
from PIL import Image

def post_proc(pix_matches):
    src_img = Image.open("Checkerboard_200x200.jpg")

    pixs = src_img.load()
    #pix_matches = bytearray(40000) # 200x200 source image
    
    '''for i in range(100):
        for j in range(100):
            idx = i * 200 + j
            idx*= 4
            pix_matches[idx] = 1'''


    rows, cols = src_img.size
    #print(rows,cols)
    count = 0
    for i in range(rows):
        for j in range(cols):
            idx = i * cols + j
            idx*= 4 #4 bytes per pixel / binary candidacy
            #print(pix_matches[idx])
            if pix_matches[idx] == b'\x01': 
                #count+= 1
                #print(count)
                #print(i,j)
                z = i - 100
                if z < 0:
                    z = 0
                while z < (i):
                    y = j - 100
                    if y < 0:
                        y = 0
                    while y < (j):
                            #print(z,y)
                            #print("red", i,j)
                            pixs[z,y] = (255,0,0) 
                            y+=1
                    #print(z)
                    z+=1
    src_img.save("TEMPED_SRC.png")

#print(pix_matches)

