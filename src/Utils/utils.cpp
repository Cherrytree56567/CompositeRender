#include "utils.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

std::string getFileExtension(const std::string& filename) {
    size_t dotPos = filename.find_last_of('.');
    if (dotPos == std::string::npos) {
        return "";
    }
    std::string ext = filename.substr(dotPos + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext;
}

bool saveImage(const std::string& filename, unsigned char* data, int width, int height, int channels) {
    std::string ext = getFileExtension(filename);

    if (ext == "png") {
        return stbi_write_png(filename.c_str(), width, height, channels, data, width * channels) != 0;
    } else if (ext == "jpg" || ext == "jpeg") {
        int quality = 90;
        return stbi_write_jpg(filename.c_str(), width, height, channels, data, quality) != 0;
    } else if (ext == "bmp") {
        return stbi_write_bmp(filename.c_str(), width, height, channels, data) != 0;
    } else if (ext == "tga") {
        return stbi_write_tga(filename.c_str(), width, height, channels, data) != 0;
    } else {
        std::cout << "[Composite::Render] Error: Unsupported file extension: " << ext << ".\n";
        return false;
    }
}

int64_t parseID(const char* idStr) {
    return idStr ? std::stoll(idStr) : -1;
}

