#ifndef HEAD_H
#define HEAD_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <string>

#pragma pack(push, 1)
struct BMPHeader {
    uint16_t fileType;
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offsetData;
};

struct DIBHeader {
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitCount;
    uint32_t compression;
    uint32_t sizeImage;
    int32_t xPixelsPerMeter;
    int32_t yPixelsPerMeter;
    uint32_t colorsUsed;
    uint32_t colorsImportant;
};
#pragma pack(pop)

class BMPImage {
public:
    BMPImage(const std::string& filePath) {
        loadBMP(filePath);
    }

    void convertToGray();
    void saveBMP(const std::string& filePath);
    void displayImageDetails();
    void adjustColor(float redFactor, float greenFactor, float blueFactor);
    void applyBlur(int radius);
    void adjustContrast(float factor);
    void applySharpen();
    void adjustBrightness(int value);
    void convertTo8Bit();
    void resize(int newWidth, int newHeight);

private:
    struct Pixel {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
    };

    BMPHeader header;
    DIBHeader dibHeader;
    std::vector<Pixel> pixels;

    void loadBMP(const std::string& filePath);
};

#endif // HEAD_H
