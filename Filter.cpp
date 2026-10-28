#include "Filter.hpp"

#include <algorithm> // std::min, std::swap
#include <cmath>     // std::round
#include <cstdint>

// Grayscale 

void Filter::grayscale(Image& image) {
    for (auto& row : image.pixels()) {
        for (auto& pixel : row) {
            const auto avg = static_cast<uint8_t>(
                std::round((pixel.rgbtRed + pixel.rgbtGreen + pixel.rgbtBlue) / 3.0));

            pixel.rgbtBlue  = avg;
            pixel.rgbtGreen = avg;
            pixel.rgbtRed   = avg;
        }
    }
}

// Sepia 

void Filter::sepia(Image& image) {
    for (auto& row : image.pixels()) {
        for (auto& pixel : row) {
            const int origRed   = pixel.rgbtRed;
            const int origGreen = pixel.rgbtGreen;
            const int origBlue  = pixel.rgbtBlue;

            pixel.rgbtRed = static_cast<uint8_t>(
                std::min(255, static_cast<int>(
                    std::round(0.393 * origRed + 0.769 * origGreen + 0.189 * origBlue))));

            pixel.rgbtGreen = static_cast<uint8_t>(
                std::min(255, static_cast<int>(
                    std::round(0.349 * origRed + 0.686 * origGreen + 0.168 * origBlue))));

            pixel.rgbtBlue = static_cast<uint8_t>(
                std::min(255, static_cast<int>(
                    std::round(0.272 * origRed + 0.534 * origGreen + 0.131 * origBlue))));
        }
    }
}

// Reflect (Two-Pointer Swap)

void Filter::reflect(Image& image) {
    const int32_t w = image.width();

    for (auto& row : image.pixels()) {
        // Two pointers inner from outer edge till centre.
        for (int32_t left = 0, right = w - 1; left < right; ++left, --right) {
            std::swap(row[static_cast<std::size_t>(left)],
                      row[static_cast<std::size_t>(right)]);
        }
    }
}
