#pragma once

namespace ImXML {

enum XMLDynamicBindType {
    Float,
    Int,
    Bool,
    Chars,
};

struct XMLDynamicBind {
    void* ptr;
    unsigned int size;
    XMLDynamicBindType type;
};
}  // namespace ImXML