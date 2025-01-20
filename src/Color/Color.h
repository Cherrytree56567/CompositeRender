#pragma once
#include <iostream>
#include "../Node/Node.h"
#include "../Vec3/Vec3.h"
#include "../globals.h"

/*
 * Color Balance Node
 * 
 * Dependencies = Image img
 * Params = Vec3 lift, Vec3 gain, Vec3 gamma
*/
struct ColorBalanceNode : Node {
    virtual void execute() override;
};

/*
 * Color Correction Node
 * 
 * Dependencies = Image img
 * Params = float brightness, float contrast, float saturation
*/
struct ColorCorrectionNode : Node {
    virtual void execute() override;
};

/*
 * Alpha Over Node
 * 
 * Dependencies = Image top (op), Image bottom (op)
 * Params = Color top (op), Color bottom (op)
*/
struct AlphaOverNode : Node {
    virtual void execute() override;
};

/*
 * Exposure Node
 * 
 * Dependencies = Image img (op)
 * Params = Color img (op), float exposure
*/
struct ExposureNode : Node {
    virtual void execute() override;
};

/*
 * Gamma Node
 * 
 * Dependencies = Image img (op)
 * Params = Color img (op), float gamma
*/
struct GammaNode : Node {
    virtual void execute() override;
};