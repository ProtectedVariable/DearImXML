#pragma once

#include <ImGuiEnum.h>
#include <imgui.h>

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
    int childflags = 0;  //only if node is "child"
    ImVec2 size{0, 0};
    bool hide = false;
    std::vector<XMLNode*> children;
    //Empty for non table nodes
    std::vector<XMLColumnSetup> tables_column_setups;

    template<typename T>
    T arg(const std::string& key);
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