#include "Input.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void ImageNode::execute() {
    std::string path = "";
    for (auto s : params) {
        if (s.first == "image") {
            path = s.second;
        }
    }
    Image sss;
    int width, height, channels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    sss.width = width;
    sss.height = height;
    sss.channels = channels;
    sss.data = data;
    output = sss;
    completed = true;
}