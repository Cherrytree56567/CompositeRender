#include "Color.h"

void ColorBalanceNode::execute() {
    Vec3 lift = {0.0f, 0.0f, 0.0f};
    Vec3 gain = {0.0f, 0.0f, 0.0f};
    Vec3 gamma = {0.0f, 0.0f, 0.0f};
    float alpha = 0.5;

    for (auto s : dependencies) {
        if (s.first == "image" && !nodes[s.second]->completed) {
            nodes[s.second]->execute();
        }
    }

    for (auto s : params) {
        if (s.first == "lift") {
            lift = stringToVec3(s.second);
        } else if (s.first == "gain") {
            gain = stringToVec3(s.second);
        } else if (s.first == "gamma") {
            gamma = stringToVec3(s.second);
        } else if (s.first == "factor") {
            alpha = std::stof(s.second);
        }
        std::cout << s.second;
    }

    Image image = nodes[dependencies[0].second]->output;
    Image imageOld = {0,0,0,nodes[dependencies[0].second]->output.data};

    for (int y = 1; y < image.height; ++y) {
        for (int x = 1; x < image.width; ++x) {
            int index = (y * image.width + x) * image.channels;

            float rNew = static_cast<float>(image.data[index]);
            float gNew = static_cast<float>(image.data[index + 1]);
            float bNew = static_cast<float>(image.data[index + 2]);

            float rOld = static_cast<float>(imageOld.data[index]);
            float gOld = static_cast<float>(imageOld.data[index + 1]);
            float bOld = static_cast<float>(imageOld.data[index + 2]);

            rNew = ((1 - alpha) * rOld) + (alpha * rNew);
            gNew = ((1 - alpha) * gOld) + (alpha * gNew);
            bNew = ((1 - alpha) * bOld) + (alpha * bNew);

            rNew /= 255.0f;
            gNew /= 255.0f;
            bNew /= 255.0f;

            rNew = (rNew + lift.x) * gain.x;
            gNew = (gNew + lift.y) * gain.y;
            bNew = (bNew + lift.z) * gain.z;

            rNew = std::pow(rNew, gamma.x);
            gNew = std::pow(gNew, gamma.y);
            bNew = std::pow(bNew, gamma.z);

            rNew = std::clamp(rNew, 0.0f, 1.0f);
            gNew = std::clamp(gNew, 0.0f, 1.0f);
            bNew = std::clamp(bNew, 0.0f, 1.0f);

            rNew *= 255.0f;
            gNew *= 255.0f;
            bNew *= 255.0f;

            image.data[index] = static_cast<unsigned char>(rNew);
            image.data[index + 1] = static_cast<unsigned char>(gNew);
            image.data[index + 2] = static_cast<unsigned char>(bNew);
        }
    }
    output = image;
    completed = true;
}

void ColorCorrectionNode::execute() {
    float brightness = 0.0f;
    float contrast = 0.0f;
    float saturation = 0.0f;
    float alpha = 0.5f;
    for (auto s : dependencies) {
        if (s.first == "image" && !nodes[s.second]->completed) {
            nodes[s.second]->execute();
        }
    }
    for (auto s : params) {
        if (s.first == "brightness") {
            brightness = std::stof(s.second);
        } else if (s.first == "contrast") {
            contrast = std::stof(s.second);
        } else if (s.first == "saturation") {
            saturation = std::stof(s.second);
        } else if (s.first == "factor") {
            alpha = std::stof(s.second);
        }
        std::cout << s.second;
    }

    Image image = nodes[dependencies[0].second]->output;
    Image imageOld = {0,0,0,nodes[dependencies[0].second]->output.data};

    if (image.channels < 3) {
        std::cout << "[Composite::Render] Error: Image must have at least 3 channels.\n";
        return;
    }

    const float lumR = 0.2126f;
    const float lumG = 0.7152f;
    const float lumB = 0.0722f;

    const int imageSize = image.width * image.height * image.channels;

    for (int i = 0; i < imageSize; i += image.channels) {
        float r = image.data[i];
        float g = image.data[i + 1];
        float b = image.data[i + 2];

        r *= brightness;
        g *= brightness;
        b *= brightness;

        r = (r - 128.0f) * contrast + 128.0f;
        g = (g - 128.0f) * contrast + 128.0f;
        b = (b - 128.0f) * contrast + 128.0f;

        float luminance = r * lumR + g * lumG + b * lumB;

        r = luminance + (r - luminance) * saturation;
        g = luminance + (g - luminance) * saturation;
        b = luminance + (b - luminance) * saturation;

        image.data[i] = std::clamp(static_cast<int>(r), 0, 255);
        image.data[i + 1] = std::clamp(static_cast<int>(g), 0, 255);
        image.data[i + 2] = std::clamp(static_cast<int>(b), 0, 255);

        if (image.channels == 4) {
            image.data[i + 3] = image.data[i + 3];
        }
    }
    output = image;
    completed = true;
}

void AlphaOverNode::execute() {
    Image top;
    Image bottom;
    float factor;
    for (auto s : dependencies) {
        if (s.first == "image" && !nodes[s.second]->completed) {
            nodes[s.second]->execute();
            if (top.width == 0) {
                top = nodes[s.second]->output;
            } else if (bottom.width == 0) {
                bottom = nodes[s.second]->output;
            } else {
                std::cout << "[Composite::Render] Warning: Too many Image Dependencies.\n";
            }
        }
    }
    for (auto s : params) {
        if (s.first == "color" && top.width != 0) {
            std::istringstream colorStream(s.second);
            std::vector<int> colorValues;
            std::string value;
            while (std::getline(colorStream, value, ',')) {
                float colorValue = std::stof(value);
                int intColorValue = static_cast<int>(colorValue * 255);
                colorValues.push_back(intColorValue);
            }

            Image colorImg(512, 512, 4, 0);
            colorImg.fillColor(colorValues[0], colorValues[1], colorValues[2]);
            top = colorImg;
        } else if (s.first == "color" && bottom.width != 0) {
            std::istringstream colorStream(s.second);
            std::vector<int> colorValues;
            std::string value;
            while (std::getline(colorStream, value, ',')) {
                float colorValue = std::stof(value);
                int intColorValue = static_cast<int>(colorValue * 255);
                colorValues.push_back(intColorValue);
            }

            Image colorImg(512, 512, 4, 0);
            colorImg.fillColor(colorValues[0], colorValues[1], colorValues[2]);
            bottom = colorImg;
        } else if (s.first == "factor") {
            factor = std::stof(s.second);
        }
    }

    if (bottom.width != top.width || bottom.height != top.height) {
        bottom = bottom.resize(top.width, top.height);
    }

    std::unique_ptr<unsigned char[]> data(new unsigned char[top.width * top.height * top.channels]);

    for (int i = 0; i < top.width * top.height; ++i) {
        for (int c = 0; c < top.channels; ++c) {
            unsigned char topVal = top.data[i * top.channels + c];
            unsigned char bottomVal = bottom.data[i * bottom.channels + c];
            data[i * top.channels + c] = static_cast<unsigned char>(factor * topVal + (1.0f - factor) * bottomVal);
        }
    }

    output = Image(top.width, top.height, top.channels, data.release());
    completed = true;
}

void ExposureNode::execute() {
    Image img;
    float exposure;
    for (auto s : dependencies) {
        if (s.first == "image" && !nodes[s.second]->completed) {
            nodes[s.second]->execute();
            img = nodes[s.second]->output;
        }
    }
    for (auto s : params) {
        if (s.first == "exposure") {
            exposure = std::stof(s.second);
        }
        if (s.first == "color") {
            std::istringstream colorStream(s.second);
            std::vector<int> colorValues;
            std::string value;
            while (std::getline(colorStream, value, ',')) {
                float colorValue = std::stof(value);
                int intColorValue = static_cast<int>(colorValue * 255);
                colorValues.push_back(intColorValue);
            }

            Image colorImg(512, 512, 4, 0);
            colorImg.fillColor(colorValues[0], colorValues[1], colorValues[2]);
            img = colorImg;
        }
    }

    if (img.channels < 3) {
        std::cout << "[Composite::Render] Error: Image must have at least 3 channels.\n";
        return;
    }

    for (int i = 0; i < img.width * img.height * img.channels; i += img.channels) {
        for (int j = 0; j < 3; ++j) {
            float topVal = img.data[i + j] / 255.0f;
            float exposedVal = 1.0f - std::exp(-topVal * exposure);
            img.data[i + j] = static_cast<unsigned char>(exposedVal * 255);
        }

        if (img.channels == 4) {
            img.data[i + 3] = img.data[i + 3];
        }
    }
    output = img;
    completed = true;
}

void GammaNode::execute() {
    Image img;
    float gamma;
    for (auto s : dependencies) {
        if (s.first == "image" && !nodes[s.second]->completed) {
            nodes[s.second]->execute();
            img = nodes[s.second]->output;
        }
    }
    for (auto s : params) {
        if (s.first == "gamma") {
            gamma = std::stof(s.second);
        }
        if (s.first == "color") {
            std::istringstream colorStream(s.second);
            std::vector<int> colorValues;
            std::string value;
            while (std::getline(colorStream, value, ',')) {
                float colorValue = std::stof(value);
                int intColorValue = static_cast<int>(colorValue * 255);
                colorValues.push_back(intColorValue);
            }

            Image colorImg(512, 512, 4, 0);
            colorImg.fillColor(colorValues[0], colorValues[1], colorValues[2]);
            img = colorImg;
        }
    }

    if (img.channels < 3) {
        std::cout << "[Composite::Render] Error: Image must have at least 3 channels.\n";
        return;
    }

    for (size_t i = 0; i < img.width * img.height * img.channels; i += img.channels) {
        for (size_t j = 0; j < 3; ++j) {
            float topVal = img.data[i + j] / 255.0f;
            float exposedVal = std::pow(topVal, gamma);
            img.data[i + j] = static_cast<unsigned char>(exposedVal * 255);
        }

        if (img.channels == 4) {
            img.data[i + 3] = img.data[i + 3];
        }
    }
    output = img;
    completed = true;
}