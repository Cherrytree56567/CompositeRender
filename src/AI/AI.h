#pragma once
#include <iostream>
#include <cpu_provider_factory.h>
#include <onnxruntime_cxx_api.h>
#include "../Node/Node.h"
#include "../globals.h"

/*
 * Upscale Node
 * 
 * Dependencies = Image img (op)
 * Params = int scale
*/
struct UpscaleNode : Node {
    virtual void execute() override;
};