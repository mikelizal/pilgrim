#!/usr/bin/env python

from PIL import Image
from sys import argv

filename=argv[1]
x=int(argv[2])
y=int(argv[3])
dx=int(argv[4])
dy=int(argv[5])

print('file: %s'%filename)

imagen=Image.open(filename)
imagen.crop((x,y,x+dx,y+dy)).show()





