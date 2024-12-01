from PIL import Image, ImageDraw, ImageFont, ImageFile
import os


ImageFile.LOAD_TRUNCATED_IMAGES = True

def add_text_to_image(image_path, output_path, text, position, font_size=40, text_color=(255, 0, 0)):
    image = Image.open(image_path)
    draw = ImageDraw.Draw(image)

    try:
        font = ImageFont.truetype("arial.ttf", font_size)
    except IOError:
        print("Font not found, using default font.")
        font = ImageFont.load_default()

    draw.text(position, text, fill=text_color, font=font)
    image.save(output_path)

    print(f"Text added to image and saved as {output_path}")

def main():
    image_path = input("Enter the path of the BMP file: ")

    if not os.path.exists(image_path):
        print("The specified file does not exist!")
        return

    text = input("Enter the text you want to add to the image: ")

    try:
        x = int(input("Enter the X coordinate for the text: "))
        y = int(input("Enter the Y coordinate for the text: "))
    except ValueError:
        print("Invalid input for coordinates. Please enter integer values.")
        return

    try:
        font_size = int(input("Enter the font size: "))
    except ValueError:
        print("Invalid font size. Using default size 40.")
        font_size = 40

    try:
        r = int(input("Enter the red component of the text color (0-255): "))
        g = int(input("Enter the green component of the text color (0-255): "))
        b = int(input("Enter the blue component of the text color (0-255): "))
        text_color = (r, g, b)
    except ValueError:
        print("Invalid input for color. Using default color red (255, 0, 0).")
        text_color = (255, 0, 0)

    output_path = input("Enter the path to save the edited image: ")

    add_text_to_image(image_path, output_path, text, (x, y), font_size, text_color)

if __name__ == "__main__":
    main()
