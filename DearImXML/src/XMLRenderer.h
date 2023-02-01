#ifndef XMLRENDERER_H
#define XMLRENDERER_H

#include <XMLTree.h>
#include <ImGUI/imgui.h>

namespace DearImXML {
	class XMLRenderer
	{
	private:
		void onNodeBegin(XMLNode& node) {
			if(node.type == ImGuiEnum::BEGIN) {
				ImGui::Begin(node.args["name"].c_str());
			}

			if(node.type == ImGuiEnum::BUTTON) {
				ImGui::Button(node.args["label"].c_str());
			}
		}

		void onNodeEnd(XMLNode& node) {
			if(node.type == ImGuiEnum::BEGIN) {
				ImGui::End();
			}
		}

		void traverse(XMLNode& root) {
			onNodeBegin(root);
			for(auto child : root.children) {
				traverse(*child);
			}
			onNodeEnd(root);
		}
	public:
		XMLRenderer(/* args */);
		~XMLRenderer();

		void render(XMLTree& tree) {
			XMLNode root = tree.getRoot();
			traverse(root);
		}
	};
	
	XMLRenderer::XMLRenderer(/* args */)
	{
	}
	
	XMLRenderer::~XMLRenderer()
	{
	}
	
}

#endif /* XMLRENDERER_H */
