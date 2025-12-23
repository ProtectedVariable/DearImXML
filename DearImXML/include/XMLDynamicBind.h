#pragma once

namespace ImXML {

enum XMLDynamicBindType {
    Float,
    Int,
    Chars,
};

struct XMLDynamicBind {
    void* ptr;
    unsigned int size;
    XMLDynamicBindType type;
};
}  // namespace ImXML