#ifndef BMP_HPP
#define BMP_HPP

//                14 + 40 = 54 header bytes

#include <cstdint>

#pragma pack(push, 1)


struct BITMAPFILEHEADER {
    uint16_t bfType;       
    uint32_t bfSize;       
    uint16_t bfReserved1; 
    uint16_t bfReserved2;  
    uint32_t bfOffBits;    
};


struct BITMAPINFOHEADER {
    uint32_t biSize;          // Size of this header (40).
    int32_t  biWidth;         // Image width in pixels.
    int32_t  biHeight;        // Image height in pixels (positive = bottom-up).
    uint16_t biPlanes;        // Number of colour planes (must be 1).
    uint16_t biBitCount;      // Bits per pixel (24 for this project).
    uint32_t biCompression;   // Compression type (0 = BI_RGB, uncompressed).
    uint32_t biSizeImage;     // Size of the raw pixel data (may be 0 for BI_RGB).
    int32_t  biXPelsPerMeter; // Horizontal resolution (pixels per metre).
    int32_t  biYPelsPerMeter; // Vertical resolution (pixels per metre).
    uint32_t biClrUsed;       // Number of colours in the palette (0 = default).
    uint32_t biClrImportant;  // Number of important colours (0 = all).
};


struct RGBTRIPLE {
    uint8_t rgbtBlue;
    uint8_t rgbtGreen;
    uint8_t rgbtRed;
};

#pragma pack(pop)

#endif 
