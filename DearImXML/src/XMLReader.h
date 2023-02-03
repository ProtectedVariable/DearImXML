#ifndef XMLREADER_H
#define XMLREADER_H

#include <string>
#include <XMLTree.h>
#include <ImGuiEnum.h>
#include <fstream>
#include <iostream>
#include <regex>
#include <stack>
#include <unordered_map>
#include <ImGUI/imgui.h>

namespace ImXML {
	class XMLReader
	{
	private:
		/* data */
		static constexpr const char* ws = " \t\n\r\f\v";
		static const std::unordered_map<std::string, ImGuiEnum> tagnames;
		static const std::unordered_map<std::string, int> flagnames;

		// trim from end of string (right)
		inline std::string& rtrim(std::string& s, const char* t = ws) {
			s.erase(s.find_last_not_of(t) + 1);
			return s;
		}

		// trim from beginning of string (left)
		inline std::string& ltrim(std::string& s, const char* t = ws) {
			s.erase(0, s.find_first_not_of(t));
			return s;
		}

		// trim from both ends of string (right then left)
		inline std::string& trim(std::string& s, const char* t = ws) {
			return ltrim(rtrim(s, t), t);
		}

		std::vector<std::string> tokenize(const std::string& line, const std::string& delimiters) {
			std::vector<std::string> tokens;
			int last = 0;
			int i = 0;
			for(char c : line) {
				bool match = false;
				for(char d : delimiters) {
					if(c == d) {
						match = true;
						break;
					}
				}
				if(match) {
					tokens.push_back(line.substr(last, i-last));
					last = i+1;
				}
				i++;
			}
			tokens.push_back(line.substr(last, i-last));
			return tokens;
		}

		int parseFlags(std::string& flagstr) {
			auto flags = tokenize(flagstr, "|,");
			int parsed = 0;
			for(auto flag : flags) {
				parsed |= flagnames.at(flag);
			}
			return parsed;
		}

		XMLNode* stringToNode(std::string& str) {
			std::regex tagname = std::regex(R"(<\/?\w+)");
			std::smatch m;
			std::regex_search(str, m, tagname);
			auto prefix = m.str(0);
			auto name = prefix.substr(1, prefix.length()-1);
			XMLNode* node = new XMLNode;
			if(tagnames.find(name) == tagnames.end()) {
				throw std::runtime_error("Unknown tag " + name);
			}
			node->type = tagnames.at(name);

			std::regex arguments = std::regex(R"(\w+=\"[^"]+\")");
			std::string::const_iterator searchStart(str.cbegin());

			while(std::regex_search(searchStart, str.cend(), m, arguments)) {
				for(auto x : m) {
					auto argstr = x.str();
					auto argtoken = tokenize(argstr, "=");
					node->args.insert({argtoken[0], argtoken[1].substr(1, argtoken[1].length()-2)});
					//Special case, flags must be converted
					if(argtoken[0] == "flags") {
						node->flags = parseFlags(node->args["flags"]);
					}
				}
				searchStart = m.suffix().first;
			}
			return node;
		}

		XMLTree traverse(std::vector<std::string>& tags) {
			std::stack<XMLNode*> parents;
			XMLNode* lastparent;
			for(auto tagstr : tags) {
				if(tagstr.at(1) == '/') {
					lastparent = parents.top();
					parents.pop();
				} else {
					XMLNode* node = stringToNode(tagstr);
					if(parents.size() > 0) {
						parents.top()->children.push_back(node);
					}
					//not a inline tag
					if(tagstr.at(tagstr.length()-2) != '/') {
						parents.push(node);
					}
				}
			}
			return XMLTree(*lastparent);
		}
	public:
		XMLReader();
		~XMLReader();

		XMLTree read(std::string filename);
	};
	
	XMLReader::XMLReader()
	{
	}
	
	XMLReader::~XMLReader()
	{
	}

	XMLTree XMLReader::read(std::string filename) {
		std::ifstream file(filename);
		std::string str;
		std::vector<std::string> taglines;
		while (std::getline(file, str))
		{
			std::string trimmed = trim(str);
			if(trimmed.length() <= 0) {
				continue;
			}
			if(trimmed.at(0) != '<') {
				throw std::runtime_error("Invalid XML line");
			}

			std::regex tagregex = std::regex(R"(^<\s*[^\?][ -~]+\/?>)");
			std::smatch m;
			if(std::regex_search(trimmed, m, tagregex)) {
				taglines.push_back(m.str(0));
			}

		}

		return traverse(taglines);
	}

	const std::unordered_map<std::string, ImGuiEnum> XMLReader::tagnames = {
		{ "begin", ImGuiEnum::BEGIN },
		{ "button" , ImGuiEnum::BUTTON },
		{ "placeholder", ImGuiEnum::PLACEHOLDER },
		{ "text", ImGuiEnum::TEXT },
		{ "sameline", ImGuiEnum::SAMELINE },
		{ "menubar", ImGuiEnum::MENUBAR },
		{ "menu", ImGuiEnum::MENU },
		{ "menuitem", ImGuiEnum::MENUITEM },
		{ "sliderfloat", ImGuiEnum::SLIDERFLOAT },
	};

	const std::unordered_map<std::string, int> XMLReader::flagnames = {
		{ "ImGuiWindowFlags_MenuBar", ImGuiWindowFlags_MenuBar },
		{ "ImGuiWindowFlags_NoCollapse", ImGuiWindowFlags_NoCollapse },
		{ "ImGuiWindowFlags_NoTitleBar", ImGuiWindowFlags_NoTitleBar },
	};
}


#endif /* XMLREADER_H */
