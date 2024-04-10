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

    
    BOXSIZE = 120
    spritesheet = Image.new("RGBA", (9*BOXSIZE, 9*BOXSIZE))


    # Get the image size
    #width, height = image.size
    #print("Image size:", width, "x", height, '\n')

    
    directions_x = [30, 125, 200, 287, 360, 442, 543, 640]
    starts_y = [33, 33, 33, 33, 33, 110, 85, 83]
    for i in range(8):
        print("{ ", end='')
        n = 0
        y = starts_y[i]
        while y < image.size[1]:
            if(image.getpixel((directions_x[i], y))[3] == 255):
                left, top, right, bottom = is_edging(image, directions_x[i], y)
                width, height = right - left, bottom - top
                
                #print('{', f"{left}, {top}, {width}, {height}", '}, ', end='')
                rect = image.crop((left, top, right, bottom))
                
                x_offset = (BOXSIZE - width)//2
                y_offset = BOXSIZE - height
                
                spritesheet.paste(rect, (i * BOXSIZE + x_offset + 7, n * BOXSIZE + y_offset))
                
                print(width, "x",height, " ")
                y = bottom + 1
                n+=1
                
                if(n == 6): break
            
            y+=1
        print("},")
    
    
    spritesheet.save("spritesheet.png")
    
    # Save the modified image
    #output_path = "output.jpg"
    #image.save(output_path)

    #print("Image manipulation complete. Saved as", output_path)

if __name__ == '__main__':
    main()