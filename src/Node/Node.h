#pragma once
#include <cstdint>
#include <string>
#include <vector>

struct Image {
    int width, height, channels;
    unsigned char* data;
    Image() : width(0), height(0), channels(0), data(nullptr) {}
    Image(int w, int h, int c, unsigned char* d) : width(w), height(h), channels(c), data(d) {}
    void fillColor(unsigned char r, unsigned char g, unsigned char b);
    Image resize(int newWidth, int newHeight) const;
};

struct Node {
    int64_t id;
    std::string name;
    std::vector<std::pair<std::string, int64_t>> dependencies;
    std::vector<std::pair<std::string, std::string>> params;
    Image output;
    bool completed = false;
    virtual void execute() {}
};