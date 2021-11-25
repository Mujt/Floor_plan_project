from PIL import Image, ImageDraw

import colorsys
import random

def get_n_hls_colors(num):
    hls_colors = []
    i = 0
    step = 360.0 / num
    while i < 360:
        h = i
        s = 90 + random.random() * 10
        l = 50 + random.random() * 10
        _hlsc = [h / 360.0, l / 100.0, s / 100.0]
        hls_colors.append(_hlsc)
        i += step
    return hls_colors

def ncolors(num):
    rgb_colors = []
    if num < 1:
        return rgb_colors
    hls_colors = get_n_hls_colors(num)
    for hlsc in hls_colors:
        _r, _g, _b = colorsys.hls_to_rgb(hlsc[0], hlsc[1], hlsc[2])
        r, g, b = [int(x * 255.0) for x in (_r, _g, _b)]
        rgb_colors.append((r, g, b))

    return rgb_colors

def circuit_see2():
    file = open("C:\\Users\\mujt1\\Desktop\\EDA理论\\Floor_plan_EDA_project\\out.txt", "r")
    im01 = Image.open("C:\\Users\\mujt1\\Desktop\\EDA理论\\Floor_plan_EDA_project\\circuit.jpg")
    draw = ImageDraw.Draw(im01)

    row = file.readlines()
    l = []
    for line in row:
        str = list(line.split(" "))
        l.append(str)
    print(l)
    bili = 8
    maps = []
    texts = []
    for hang in l:
        ll = []
        if len(hang) == 5:
            x = (int(hang[1])+1)/bili
            y = (int(hang[2])+1)/bili
            width = (int(hang[3])-2)/bili
            height = (int(hang[4])-2)/bili
            ll.append(x)
            ll.append(y)
            ll.append((x + width))
            ll.append((y + height))
            maps.append(ll)
            texts.append((x+width/4,y+height/4))
        elif len(hang) == 3:
            x = (int(hang[1])-2)/bili
            y = (int(hang[2])-2)/bili
            ll.append(x)
            ll.append(y)
            ll.append(x + 1)
            ll.append(y + 1)
            maps.append(ll)
            #texts.append((x+1, y+1))
    print(maps)
    print(texts)

    colors = ncolors(len(maps))
    for i in range(len(maps)):
        draw.rectangle(maps[i], fill=colors[i])
        #draw.text(texts[i],i.__str__(),fill='#272727')

    for i in range(len(texts)):
        draw.text(texts[i],i.__str__(),fill='#272727')

    im01.show()

circuit_see2()