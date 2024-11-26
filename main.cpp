// 26.11.24
// Author : A.Goktug
// File : main.cpp

#include "head.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

int main()
{
    std::string inputFilePath;
    std::string outputFilePath;
    std::string copyFolderPath = "COPY";

    std::cout << "The software is provided \"AS IS\", without warranty of any kind, express or\n";
    std::cout << "implied, including but not limited to the warranties of merchantability,\n";
    std::cout << "fitness for a particular purpose and noninfringement. In no event shall the\n";
    std::cout << "authors or copyright holders be liable for any claim, damages or other\n";
    std::cout << "liability, whether in an action of contract, tort or otherwise, arising from,\n";
    std::cout << "out of or in connection with the software or the use or other dealings in the\n";
    std::cout << "software.\n\n";


    std::cout << "Author:A.Goktug:\n";

    std::cout << "Enter the path of the BMP file: ";
    std::getline(std::cin, inputFilePath);


    if (!fs::exists(inputFilePath))
    {
        std::cerr << "The specified file does not exist!" << std::endl;
        return 1;
    }


    if (!fs::exists(copyFolderPath))
    {
        fs::create_directory(copyFolderPath);
    }


    std::string fileName = fs::path(inputFilePath).filename().string();
    std::string copyFilePath = copyFolderPath + "/" + fileName;


    try
    {
        fs::copy(inputFilePath, copyFilePath, fs::copy_options::overwrite_existing);
        std::cout << "Original file copied to " << copyFilePath << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error copying file: " << e.what() << std::endl;
        return 1;
    }


    BMPImage bmpImage(inputFilePath);
    bmpImage.displayImageDetails();

    int choice;
    std::cout << "Select the type of editing:\n"
    << "1. Basic Editing (Grayscale)\n"
    << "2. Detailed Editing (Color Adjustment)\n"
    << "3. Blur\n"
    << "4. Adjust Contrast\n"
    << "5. Apply Sharpen\n"
    << "6. Adjust Brightness\n"
    << "7. Convert to 8-Bit Picture\n"
    << "8. Resize\n"
    << "Choice: ";
    std::cin >> choice;


    if (choice == 1) {
        bmpImage.convertToGray();
    } else if (choice == 2) {
        float redFactor, greenFactor, blueFactor;
        std::cout << "Enter the red brightness factor (a value between 0.0 and 2.0, 1.0 is the original): ";
        std::cin >> redFactor;
        std::cout << "Enter the green brightness factor (a value between 0.0 and 2.0, 1.0 is the original): ";
        std::cin >> greenFactor;
        std::cout << "Enter the blue brightness factor (a value between 0.0 and 2.0, 1.0 is the original): ";
        std::cin >> blueFactor;

        bmpImage.adjustColor(redFactor, greenFactor, blueFactor);
    } else if (choice == 3) {
        int blurValue;
        std::cout << "Enter a value for blur: ";
        std::cin >> blurValue;

        if (blurValue < 0) {
            std::cerr << "Invalid blur value!" << std::endl;
            return 1;
        }

        bmpImage.applyBlur(blurValue);
    } else if (choice == 4) {
        float contrastFactor;
        std::cout << "Enter the contrast adjustment factor (0.0 to 2.0, 1.0 is original): ";
        std::cin >> contrastFactor;
        bmpImage.adjustContrast(contrastFactor);
    } else if (choice == 5) {
        bmpImage.applySharpen();
    } else if (choice == 6) {
        int brightnessValue;
        std::cout << "Enter the brightness adjustment value (-255 to 255, 0 is original): ";
        std::cin >> brightnessValue;
        bmpImage.adjustBrightness(brightnessValue);
    }
    else if (choice == 7) {
        bmpImage.convertTo8Bit();
    }
    else if (choice == 8) {
        int width, height;
        std::cout << "Enter new width: ";
        std::cin >> width;
        std::cout << "Enter new height: ";
        std::cin >> height;


        bmpImage.resize(width, height);
    }
    else
    {
        std::cerr << "Invalid choice!" << std::endl;
        return 1;
    }


    std::cout << "Enter the file path to save the edited image: ";
    std::cin.ignore();
    std::getline(std::cin, outputFilePath);

    bmpImage.saveBMP(outputFilePath);

    std::cout << "Process completed, output file: " << outputFilePath << std::endl;

    return 0;
}
