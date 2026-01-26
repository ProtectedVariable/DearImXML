#pragma once

#include <XMLTree.h>

namespace ImXML {
class XMLEventHandler {
   public:
    XMLEventHandler() = default;
    virtual ~XMLEventHandler() = default;

    virtual void onNodeBegin(XMLNode& node) = 0;
    virtual void onNodeEnd(XMLNode& node) = 0;
    virtual void onEvent(XMLNode& node) = 0;
};
}  // namespace ImXML
