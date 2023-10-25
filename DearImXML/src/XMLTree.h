#ifndef DEARIMXML_XMLTREE_H
#define DEARIMXML_XMLTREE_H

#include <ImGuiEnum.h>
#include <unordered_map>
#include <string>
#include <vector>

namespace ImXML {

	struct XMLNode
	{
		ImGuiEnum type;
		std::unordered_map<std::string, std::string> args;
		int flags = 0;
		std::vector<XMLNode*> children;
	};
	

	class XMLTree
	{
	private:
		XMLNode root;
	public:
		XMLTree(XMLNode root);
		~XMLTree();

		XMLNode& getRoot() {
			return root;
		}
	};
	
	XMLTree::XMLTree(XMLNode root) : root(root)
	{
	}
	
	XMLTree::~XMLTree()
	{
	}
	
}

#endif /* DEARIMXML_XMLTREE_H */
