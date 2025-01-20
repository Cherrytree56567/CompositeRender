#pragma once
#include <string>
#include <algorithm>
#include <iostream>
#include <unordered_map>

std::string getFileExtension(const std::string& filename);

bool saveImage(const std::string& filename, unsigned char* data, int width, int height, int channels);

int64_t parseID(const char* idStr);