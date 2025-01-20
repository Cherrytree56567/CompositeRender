#include "Node.h"

Image Image::resize(int newWidth, int newHeight) const {
    Image resized(newWidth, newHeight, channels, new unsigned char[newWidth * newHeight * channels]());
    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            int srcX = x * width / newWidth;
            int srcY = y * height / newHeight;

            for (int c = 0; c < channels; ++c) {
                resized.data[(y * newWidth + x) * channels + c] = data[(srcY * width + srcX) * channels + c];
            }
        }
    }
    return resized;
}

void Image::fillColor(unsigned char r, unsigned char g, unsigned char b) {
    data = new unsigned char[width * height * channels]();
    for (int i = 0; i < width * height; ++i) {
        data[i * channels + 0] = r;
        data[i * channels + 1] = g;
        data[i * channels + 2] = b;
        if (channels == 4) {
            data[i * channels + 3] = 255;
        }
    }
}