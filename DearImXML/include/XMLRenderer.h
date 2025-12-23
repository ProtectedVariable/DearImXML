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
    bool popup = false;
    std::unordered_map<std::string, XMLDynamicBind> dynamicBinds;

    void renderMenu(XMLNode& node, XMLEventHandler& handler);

    void renderTree(XMLNode& node, XMLEventHandler& handler);

    void onNodeBegin(XMLNode& node, XMLEventHandler& handler, bool inPopup = false);

    void onNodeEnd(XMLNode& node, XMLEventHandler& handler, bool inPopup = false);

    void traverse(XMLNode& root, XMLEventHandler& handler, bool inPopup = false);

   public:
    XMLRenderer(/* args */);
    ~XMLRenderer();

    void render(XMLTree& tree, XMLEventHandler& handler);

    void addDynamicBind(const std::string& name, const XMLDynamicBind& bind);
};
}  // namespace ImXML