#include "Image.hpp"

#include <fstream>
#include <stdexcept>



void Image::read(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Could not open input file: " + filename);
    }

    // Read image 
    file.read(reinterpret_cast<char*>(&fileHeader_), sizeof(fileHeader_));
    file.read(reinterpret_cast<char*>(&infoHeader_), sizeof(infoHeader_));

    if (!file) {
        throw std::runtime_error("Error reading BMP headers from: " + filename);
    }

    // format checkig 
    if (fileHeader_.bfType != 0x4D42) {
        throw std::runtime_error("Not a BMP file (bad magic number): " + filename);
    }
    if (infoHeader_.biBitCount != 24) {
        throw std::runtime_error("Only 24-bit BMPs are supported: " + filename);
    }
    if (infoHeader_.biCompression != 0) {
        throw std::runtime_error("Only uncompressed BMPs are supported: " + filename);
    }

    // what are dimension of the image we input 
    const int32_t w = width();
    const int32_t h = height();
    const bool    topDown = (infoHeader_.biHeight < 0);
    const int     padding = paddingBytes();

    // using vector to make DMA
    pixels_.assign(static_cast<std::size_t>(h),
                   std::vector<RGBTRIPLE>(static_cast<std::size_t>(w)));

    for (int32_t row = 0; row < h; ++row) {
        // by defaut BMP store bottom - up and we want to store in top -> down (so we make change here )
        const int32_t targetRow = topDown ? row : (h - 1 - row);

        file.read(reinterpret_cast<char*>(
                      pixels_[static_cast<std::size_t>(targetRow)].data()),
                  w * static_cast<std::streamsize>(sizeof(RGBTRIPLE)));

       
        file.seekg(padding, std::ios::cur);
    }

    if (!file) {
        throw std::runtime_error("Error reading pixel data from: " + filename);
    }

    
    infoHeader_.biHeight = h;
}

void Image::write(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Could not open output file: " + filename);
    }

    // Writing file 
    file.write(reinterpret_cast<const char*>(&fileHeader_), sizeof(fileHeader_));
    file.write(reinterpret_cast<const char*>(&infoHeader_), sizeof(infoHeader_));

    // Write pixel data top-down memory → bottom-up bmp 
    const int32_t w       = width();
    const int32_t h       = height();
    const int     padding = paddingBytes();
    const uint8_t pad     = 0x00;

    for (int32_t row = h - 1; row >= 0; --row) {
        file.write(reinterpret_cast<const char*>(
                       pixels_[static_cast<std::size_t>(row)].data()),
                   w * static_cast<std::streamsize>(sizeof(RGBTRIPLE)));

        for (int p = 0; p < padding; ++p) {
            file.write(reinterpret_cast<const char*>(&pad), 1);
        }
    }

    if (!file) {
        throw std::runtime_error("Error writing to output file: " + filename);
    }
}


std::vector<std::vector<RGBTRIPLE>> Image::getPixelsCopy() const {
    return pixels_;   //  deep copy of entire 2D vector
}

void Image::setPixels(const std::vector<std::vector<RGBTRIPLE>>& newPixels) {
    const auto h = static_cast<std::size_t>(height());
    const auto w = static_cast<std::size_t>(width());

    if (newPixels.size() != h) {
        throw std::invalid_argument(
            "setPixels: row count mismatch (expected " + std::to_string(h) +
            ", got " + std::to_string(newPixels.size()) + ")");
    }
    for (std::size_t r = 0; r < h; ++r) {
        if (newPixels[r].size() != w) {
            throw std::invalid_argument(
                "setPixels: column count mismatch at row " + std::to_string(r));
        }
    }

    pixels_ = newPixels;
}
