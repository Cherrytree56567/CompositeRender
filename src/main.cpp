#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <cmath>
#include <sstream>
#include <tinyxml2.h>
#include "globals.h"
#include "Utils/utils.h"
#include "Input/Input.h"
#include "Color/Color.h"
#include "Output/Output.h"
#include "AI/AI.h"

/*
 * Global Variables
*/
std::string outputFileName = "output.png";
std::unordered_map<int64_t, std::shared_ptr<Node>> nodes;

/*
 * Parses the XML into an unordered_map
 * 
 *                   |  ID   ||        Node*        |
 * std::unordered_map<int64_t, std::shared_ptr<Node>>
*/
std::unordered_map<int64_t, std::shared_ptr<Node>> parseXML(const std::string& fileName) {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(fileName.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cout << "[Composite::Render] Error: Failed to load file.\n";
        return {};
    }

    std::unordered_map<int64_t, std::shared_ptr<Node>> nodes;

    tinyxml2::XMLElement* root = doc.FirstChildElement("Composite");
    if (!root) return {};
    tinyxml2::XMLElement* nodesElement = root->FirstChildElement("Nodes");
    if (!nodesElement) return {};

    for (tinyxml2::XMLElement* elem = nodesElement->FirstChildElement(); elem != nullptr; elem = elem->NextSiblingElement()) {
        std::string nodeName = elem->Name();

        std::shared_ptr<Node> node;
        if (nodeName == "Image") {
            node = std::make_shared<ImageNode>();
        } else if (nodeName == "Composite") {
            node = std::make_shared<CompositeNode>();
        } else if (nodeName == "ColorBalance") {
            node = std::make_shared<ColorBalanceNode>();
        } else if (nodeName == "ColorCorrection") {
            node = std::make_shared<ColorCorrectionNode>();
        } else if (nodeName == "AlphaOver") {
            node = std::make_shared<AlphaOverNode>();
        } else if (nodeName == "Exposure") {
            node = std::make_shared<ExposureNode>();
        } else if (nodeName == "Gamma") {
            node = std::make_shared<GammaNode>();
        } else if (nodeName == "Upscale") {
            node = std::make_shared<UpscaleNode>();
        } else if (nodeName == "RemoveBackground") {
            node = std::make_shared<BackgroundRemoverNode>();
        } else if (nodeName == "ImageGenerator") {
            node = std::make_shared<ImageGenNode>();
        } else if (nodeName == "HDR") {
            node = std::make_shared<HDRNode>();
        } else {
            continue;
        }

        node->name = nodeName;
        const char* idStr = elem->Attribute("id");
        node->id = parseID(idStr);

        if (nodeName == "Composite") {
            tinyxml2::XMLElement* compositeElem = elem;
            const char* activeStr = compositeElem->Attribute("active");
            if (activeStr) {
                std::string activeValue = activeStr;
                std::shared_ptr<CompositeNode> compositeNode = std::dynamic_pointer_cast<CompositeNode>(node);
                if (activeValue == "true") {
                    compositeNode->active = true;
                } else {
                    compositeNode->active = false;
                }
            }
        }

        for (tinyxml2::XMLElement* inElem = elem->FirstChildElement("in"); inElem != nullptr; inElem = inElem->NextSiblingElement("in")) {
            const char* type = inElem->Attribute("type");
            const char* valStr = inElem->Attribute("val");
            if (type && valStr) {
                node->dependencies.emplace_back(type, parseID(valStr));
            }
        }

        for (tinyxml2::XMLElement* paramElem = elem->FirstChildElement("param"); paramElem != nullptr; paramElem = paramElem->NextSiblingElement("param")) {
            const char* name = paramElem->Attribute("name");
            const char* val = paramElem->Attribute("val");
            if (name && val) {
                node->params.emplace_back(name, val);
            }
        }
        nodes[node->id] = node;
    }
    return nodes;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cout << "[Composite::Render] Critical Error: Please provide an .xml file for parsing and an output file.\n";
        std::cout << "[Composite::Render] \n";
        std::cout << "[Composite::Render] Format:\n";
        std::cout << "[Composite::Render] CompositeRenderer.exe <xml File> <Output File>\n";
        return -1;
    }
    
    nodes = parseXML(argv[1]);
    outputFileName = argv[2];

    /*
     * Execute the Active Composite Node.
    */
    for (const auto& pair : nodes) {
        const auto& node = pair.second;

        if (auto compositeNode = std::dynamic_pointer_cast<CompositeNode>(node)) {
            if (compositeNode->active) {
                compositeNode->execute();
            }
        }
    }
}