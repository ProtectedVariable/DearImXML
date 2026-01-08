#pragma once

#include <imgui.h>
#include <ImGuiEnum.h>
#include <XMLTree.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <stack>
#include <string>
#include <unordered_map>

namespace ImXML {
class XMLReader {
   private:
    /* data */
    static constexpr const char* ws = " \t\n\r\f\v";
    static const std::unordered_map<std::string, ImGuiEnum> tagnames;
    static const std::unordered_map<std::string, int> flagnames;

    // trim from end of string (right)
    inline std::string& rtrim(std::string& s, const char* t = ws);

    // trim from beginning of string (left)
    inline std::string& ltrim(std::string& s, const char* t = ws);

    // trim from both ends of string (right then left)
    inline std::string& trim(std::string& s, const char* t = ws);

    std::vector<std::string> tokenize(const std::string& line, const std::string& delimiters);

    int parseFlags(std::string& flagstr);

    std::string toLower(const std::string& str);

    XMLNode* stringToNode(std::string& str);

    XMLTree traverse(std::vector<std::string>& tags);

   public:
    XMLReader();
    ~XMLReader();

    XMLTree read(std::string filename);
};
}  // namespace ImXML