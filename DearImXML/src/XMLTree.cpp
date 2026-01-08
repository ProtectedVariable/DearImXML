#include "XMLTree.h"

namespace ImXML {
XMLTree::XMLTree(XMLNode root) : root(root) {
}

XMLTree::~XMLTree() {
}

template<>
std::string XMLNode::arg(const std::string& key) {
    if (args.contains(key)) {
        return args[key];
    } else {
        return "";
    }
}

template<>
int XMLNode::arg(const std::string& key) {
    if (args.contains(key)) {
        return std::stoi(arg<std::string>(key));
    } else {
        return 0;
    }
}

template<>
float XMLNode::arg(const std::string& key) {
    if (args.contains(key)) {
        return std::stof(arg<std::string>(key));
    } else {
        return 0;
    }
}

template<>
double XMLNode::arg(const std::string& key) {
    if (args.contains(key)) {
        return std::stod(arg<std::string>(key));
    } else {
        return 0;
    }
}

template<>
bool XMLNode::arg(const std::string& key) {
    if (args.contains(key)) {
        return arg<std::string>(key) == "true";
    } else {
        return false;
    }
}

}  // namespace ImXML