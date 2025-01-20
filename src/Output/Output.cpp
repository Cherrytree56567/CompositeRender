#include "Output.h"

void CompositeNode::execute() {
    for (auto s : dependencies) {
        if (!nodes[s.second]->completed) {
            nodes[s.second]->execute();
        }
    }
    if (!saveImage(outputFileName, nodes[dependencies[0].second]->output.data, nodes[dependencies[0].second]->output.width, nodes[dependencies[0].second]->output.height, nodes[dependencies[0].second]->output.channels)) {
        std::cout << "[Composite::Render] Error: Failed to save image.\n";
    }
}