#pragma once
#include "../Node/Node.h"

/*
 * Image Node
 * 
 * Dependencies = NULL
 * Params = Path image
*/
struct ImageNode : Node {
    virtual void execute() override;
};