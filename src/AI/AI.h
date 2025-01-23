#pragma once
#include <iostream>
#include <stb_image_write.h>
#include <stb_image.h>
#include <Python.h>
#include <cstdlib>
#include <filesystem>
#include "../Node/Node.h"
#include "../globals.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

/*
 * Upscale Node
 * 
 * Dependencies = Image img (op)
 * Params = int scale
*/
struct UpscaleNode : Node {
    virtual void execute() override;
};