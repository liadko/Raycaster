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
                if(visited[index] == False and image.getpixel(point)[3] == 255): 
                    visited[index] = True
                    valid_spots.append(point)
        
    return left, top, right, bottom


def main():
    # Open an image file
    image_path = "doom3.png"
    image = Image.open(image_path)

    # Get the image size
    #width, height = image.size
    #print("Image size:", width, "x", height, '\n')

    a = image.getpixel((18, 22))[3]
    
    
    
    
    x = 20
    while x < image.size[0]:
        if(image.getpixel((x, 570))[3] == 255):
            left, top, right, bottom = is_edging(image, x, 570)
            width, height = right - left, bottom - top
            
            print(f"{left}, {top}, {width}, {height}")
            x = right + 1
            
        x+=1
    
    
    
    # Save the modified image
    #output_path = "output.jpg"
    #image.save(output_path)

    #print("Image manipulation complete. Saved as", output_path)

if __name__ == '__main__':
    main()