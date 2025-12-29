#pragma once

#include <ImGuiEnum.h>

#include <string>
#include <unordered_map>
#include <vector>

namespace ImXML {

struct XMLColumnSetup {
    std::string label;
    int flags;
    float init_width_or_weight;
};

struct XMLNode {
    ImGuiEnum type;
    std::unordered_map<std::string, std::string> args;
    int flags = 0;
    std::vector<XMLNode*> children;
    //Empty for non table nodes
    std::vector<XMLColumnSetup> tables_column_setups;
};

class XMLTree {
   private:
    XMLNode root;

   public:
    XMLTree(XMLNode root);
    ~XMLTree();

    XMLNode& getRoot() { return root; }
};
}  // namespace ImXML