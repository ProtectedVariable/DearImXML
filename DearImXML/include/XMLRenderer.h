#pragma once

#include <imgui.h>
#include <XMLDynamicBind.h>
#include <XMLEventHandler.h>
#include <XMLTree.h>

#include <unordered_map>

namespace ImXML {
class XMLRenderer {
   private:
    int sameline = 0;
    std::unordered_map<std::string, XMLDynamicBind> dynamicBinds;

    void renderMenu(XMLNode& node, XMLEventHandler& handler);

    void renderTree(XMLNode& node, XMLEventHandler& handler);

    bool onNodeBegin(XMLNode& node, XMLEventHandler& handler);

    void onNodeEnd(XMLNode& node, XMLEventHandler& handler);

    void traverse(XMLNode& root, XMLEventHandler& handler);

    XMLDynamicBind getDynamicBind(const XMLNode& node);

   public:
    XMLRenderer(/* args */);
    ~XMLRenderer();

    void render(XMLTree& tree, XMLEventHandler& handler);

    void addDynamicBind(const std::string& name, const XMLDynamicBind& bind);
};
}  // namespace ImXML