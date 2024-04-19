#Sprite sheet maker

from PIL import Image
import time


def is_edging(image:Image, x, y)->int:
    
    right, left, top, bottom = x, x, y, y
    
    valid_spots = [(x, y)]
    
    visited = [False]*image.size[0]*image.size[1] 
    
    
    while len(valid_spots):
        current = valid_spots.pop()
        
        if current[0] > right: right = current[0]
        elif current[0] < left: left = current[0]
        if current[1] < top: top = current[1]
        elif current[1] > bottom: bottom = current[1]
        
        for i in range(-1, 2):
            for j in range(-1, 2):
                if(i == j): continue
                
                point = (current[0]+i, current[1]+j)
                index = point[0] + point[1] * image.size[0]
                if(0 < index >= len(visited)): continue
                if(visited[index] == False and image.getpixel(point)[3] == 255): 
                    visited[index] = True
                    valid_spots.append(point)
        
    return left, top, right, bottom


def main():
    # Open an image file
    image_path = "doom3.png"
    image = Image.open(image_path)

    
    BOXSIZE = 280
    spritesheet = Image.new("RGBA", (8*BOXSIZE, 9*BOXSIZE))

    guide = Image.new("RGBA", (8*BOXSIZE, 9*BOXSIZE))
    for x in range(guide.width):
        for y in range(guide.height):
            if x % BOXSIZE == 0 or y % BOXSIZE == 0:
                guide.putpixel((x, y), (255, 0, 0, 255))
                if(x + 140 < guide.width):
                    guide.putpixel((x+140, y), (0, 0, 200, 200))
    
    guide.save("guide.png")
    # Get the image size
    #width, height = image.size
    #print("Image size:", width, "x", height, '\n')

    current_sprite = Image.new("RGBA", (8*BOXSIZE, 8*BOXSIZE))
    start_x = 30
    y  = [60, 180, 330, 450, 575, 690, 800, 890]
    for i in range(8):
        print("{ ", end='')
        n = 0
        x = start_x
        while x < image.size[0]:
            if(image.getpixel((x, y[i]))[3] == 255):
                left, top, right, bottom = is_edging(image, x, y[i])
                width, height = right - left, bottom - top
                
                #print('{', f"{left}, {top}, {width}, {height}", '}, ', end='')
                rect = image.crop((left, top, right, bottom))
                
                x_offset = (BOXSIZE - width)//2
                y_offset = BOXSIZE - height
                
                spritesheet.paste(rect, (n * BOXSIZE + x_offset, i * BOXSIZE + y_offset))
                current_sprite.paste(rect, (n * BOXSIZE + x_offset, i * BOXSIZE + y_offset))
                current_sprite.save(f"sprites/sprite_{i}_{n}.png")
                current_sprite = Image.new("RGBA", (8*BOXSIZE, 8*BOXSIZE))
                
                print(width, "x",height, " ")
                x = right + 1
                n+=1
                
            
            x+=1
        print("},")
    
    
    spritesheet.save("spritesheet.png")
    guide.save("guide.png")
    
    # Save the modified image
    #output_path = "output.jpg"
    #image.save(output_path)

    #print("Image manipulation complete. Saved as", output_path)

if __name__ == '__main__':
    main()