// 26.11.24
// Author : A.Goktug
// File : bmpops.cpp

#include "head.h"
#include <map>
#include <algorithm>
#include <cmath>

void BMPImage::convertToGray()
{
    for (auto& pixel : pixels)
    {
        uint8_t gray = static_cast<uint8_t>(0.299 * pixel.red + 0.587 * pixel.green + 0.114 * pixel.blue);
        pixel.red = pixel.green = pixel.blue = gray;
    }
}


void BMPImage::saveBMP(const std::string& filePath)
{
    std::ofstream out(filePath, std::ios::binary);
    out.write(reinterpret_cast<const char*>(&header), sizeof(header));
    out.write(reinterpret_cast<const char*>(&dibHeader), sizeof(dibHeader));
    out.write(reinterpret_cast<const char*>(pixels.data()), pixels.size() * sizeof(Pixel));
    out.close();
}

void BMPImage::displayImageDetails()
{
    std::cout << "Width: " << dibHeader.width << ", Height: " << dibHeader.height << std::endl;
}

void BMPImage::loadBMP(const std::string& filePath)
{
    std::ifstream in(filePath, std::ios::binary);
    if (!in)
    {
        std::cerr << "Dosya açılamadı!" << std::endl;
        return;
    }

    in.read(reinterpret_cast<char*>(&header), sizeof(header));
    in.read(reinterpret_cast<char*>(&dibHeader), sizeof(dibHeader));

    in.seekg(header.offsetData, std::ios::beg);

    pixels.resize(dibHeader.width * dibHeader.height);
    in.read(reinterpret_cast<char*>(pixels.data()), pixels.size() * sizeof(Pixel));

    in.close();
}
void BMPImage::adjustColor(float redFactor, float greenFactor, float blueFactor)
{
    for (auto& pixel : pixels)
    {
        pixel.red = std::min(static_cast<int>(pixel.red * redFactor), 255);
        pixel.green = std::min(static_cast<int>(pixel.green * greenFactor), 255);
        pixel.blue = std::min(static_cast<int>(pixel.blue * blueFactor), 255);
    }
}


void BMPImage::applyBlur(int radius)
{
    
    if (radius < 0) {
        std::cerr << "Radius must be non-negative!" << std::endl;
        return;
    }

    
    std::vector<Pixel> blurredPixels = pixels;

    
    for (int y = 0; y < dibHeader.height; ++y)
    {
        for (int x = 0; x < dibHeader.width; ++x)
        {
            int redSum = 0, greenSum = 0, blueSum = 0;
            int count = 0;

            
            for (int dy = -radius; dy <= radius; ++dy)
            {
                for (int dx = -radius; dx <= radius; ++dx)
                {
                    int nx = x + dx;
                    int ny = y + dy;

                    
                    if (nx >= 0 && ny >= 0 && nx < dibHeader.width && ny < dibHeader.height)
                    {
                        int idx = ny * dibHeader.width + nx;
                        redSum += pixels[idx].red;
                        greenSum += pixels[idx].green;
                        blueSum += pixels[idx].blue;
                        count++;
                    }
                }
            }

            
            int idx = y * dibHeader.width + x;
            blurredPixels[idx].red = std::min(255, std::max(0, redSum / count));
            blurredPixels[idx].green = std::min(255, std::max(0, greenSum / count));
            blurredPixels[idx].blue = std::min(255, std::max(0, blueSum / count));
        }
    }

    
    pixels = blurredPixels;
}



void BMPImage::adjustContrast(float factor)
{
    
    uint64_t totalRed = 0, totalGreen = 0, totalBlue = 0;
    for (const auto& pixel : pixels) {
        totalRed += pixel.red;
        totalGreen += pixel.green;
        totalBlue += pixel.blue;
    }

    int totalPixels = pixels.size();
    uint8_t avgRed = totalRed / totalPixels;
    uint8_t avgGreen = totalGreen / totalPixels;
    uint8_t avgBlue = totalBlue / totalPixels;

    
    for (auto& pixel : pixels) {
        pixel.red = std::min(255, std::max(0, static_cast<int>(avgRed + factor * (pixel.red - avgRed))));
        pixel.green = std::min(255, std::max(0, static_cast<int>(avgGreen + factor * (pixel.green - avgGreen))));
        pixel.blue = std::min(255, std::max(0, static_cast<int>(avgBlue + factor * (pixel.blue - avgBlue))));
    }
}




void BMPImage::applySharpen()
{
    
    std::vector<std::vector<int>> kernel = {
        {-1, -1, -1},
        {-1,  9, -1},
        {-1, -1, -1}
    };

    
    std::vector<Pixel> sharpenedPixels = pixels;

    
    for (int y = 1; y < dibHeader.height - 1; ++y)
    {
        for (int x = 1; x < dibHeader.width - 1; ++x)
        {
            int redSum = 0, greenSum = 0, blueSum = 0;

            
            for (int dy = -1; dy <= 1; ++dy)
            {
                for (int dx = -1; dx <= 1; ++dx)
                {
                    int nx = x + dx;
                    int ny = y + dy;

                    int idx = ny * dibHeader.width + nx;
                    redSum += pixels[idx].red * kernel[dy + 1][dx + 1];
                    greenSum += pixels[idx].green * kernel[dy + 1][dx + 1];
                    blueSum += pixels[idx].blue * kernel[dy + 1][dx + 1];
                }
            }

            
            int idx = y * dibHeader.width + x;
            sharpenedPixels[idx].red = std::min(255, std::max(0, redSum));
            sharpenedPixels[idx].green = std::min(255, std::max(0, greenSum));
            sharpenedPixels[idx].blue = std::min(255, std::max(0, blueSum));
        }
    }

    pixels = sharpenedPixels;
}



void BMPImage::adjustBrightness(int value)
{
    for (auto& pixel : pixels) {
        pixel.red = std::min(255, std::max(0, pixel.red + value));
        pixel.green = std::min(255, std::max(0, pixel.green + value));
        pixel.blue = std::min(255, std::max(0, pixel.blue + value));
    }
}




void BMPImage::convertTo8Bit() {
    
    std::vector<Pixel> palette(256);

    
    for (int i = 0; i < 256; ++i) {
        palette[i].red = palette[i].green = palette[i].blue = i;
    }

    
    std::vector<uint8_t> indexedPixels(pixels.size());
    for (size_t i = 0; i < pixels.size(); ++i) {
        const Pixel& pixel = pixels[i];

        
        int bestIndex = 0;
        int minDistance = 255 * 255 * 3; 

        for (int j = 0; j < 256; ++j) {
            const Pixel& palettePixel = palette[j];
            int distance = std::pow(palettePixel.red - pixel.red, 2) +
            std::pow(palettePixel.green - pixel.green, 2) +
            std::pow(palettePixel.blue - pixel.blue, 2);

            if (distance < minDistance) {
                minDistance = distance;
                bestIndex = j;
            }
        }

        indexedPixels[i] = bestIndex;  
    }

    
    dibHeader.bitCount = 8;
    dibHeader.colorsUsed = 256;
    dibHeader.sizeImage = pixels.size();

    
    header.offsetData = sizeof(header) + sizeof(dibHeader) + (256 * sizeof(Pixel));

    
    std::ofstream out("output_8bit.bmp", std::ios::binary);
    out.write(reinterpret_cast<const char*>(&header), sizeof(header));
    out.write(reinterpret_cast<const char*>(&dibHeader), sizeof(dibHeader));

    
    out.write(reinterpret_cast<const char*>(palette.data()), 256 * sizeof(Pixel));

    
    out.write(reinterpret_cast<const char*>(indexedPixels.data()), indexedPixels.size());
    out.close();

    std::cout << "Image has been converted to 8-bit." << std::endl;
}


void BMPImage::resize(int newWidth, int newHeight) {
    std::vector<Pixel> resizedPixels(newWidth * newHeight);

    float xRatio = static_cast<float>(dibHeader.width) / newWidth;
    float yRatio = static_cast<float>(dibHeader.height) / newHeight;

    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            int originalX = std::min(static_cast<int>(x * xRatio), dibHeader.width - 1);
            int originalY = std::min(static_cast<int>(y * yRatio), dibHeader.height - 1);

            int originalIdx = originalY * dibHeader.width + originalX;
            int newIdx = y * newWidth + x;

            resizedPixels[newIdx] = pixels[originalIdx];
        }
    }

    dibHeader.width = newWidth;
    dibHeader.height = newHeight;
    pixels = resizedPixels;

    std::cout << "Image has been resized to " << newWidth << "x" << newHeight << "." << std::endl;
}


