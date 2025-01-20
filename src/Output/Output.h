#pragma once
#include "../globals.h"
#include "../Node/Node.h"
#include "../Utils/utils.h"

/*
 * Composite Node
 * 
 * Dependencies = Image img
 * Params = NULL
*/
struct CompositeNode : Node {
    virtual void execute() override;
    bool active;
};