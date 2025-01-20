#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "Node/Node.h"

extern std::string outputFileName;
extern std::unordered_map<int64_t, std::shared_ptr<Node>> nodes;