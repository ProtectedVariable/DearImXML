#ifndef XMLRENDERER_H
#define XMLRENDERER_H

#include <XMLTree.h>
#include <XMLEventHandler.h>
#include <ImGUI/imgui.h>

namespace ImXML {
	class XMLRenderer
	{
	private:
		bool sameline = false;

		void onNodeBegin(XMLNode& node, XMLEventHandler& handler) {
			handler.onNodeBegin(node);
			
			if(sameline) {
				ImGui::SameLine();
			}
			
			if(node.type == ImGuiEnum::BEGIN) {
				ImGui::Begin(node.args["name"].c_str());
			}

			if(node.type == ImGuiEnum::BUTTON) {
				if(ImGui::Button(node.args["label"].c_str())) {
					handler.onEvent(node);
				}
			}

			if(node.type == ImGuiEnum::TEXT) {
				ImGui::Text("%s", node.args["label"].c_str());
			}

			if(node.type == ImGuiEnum::SAMELINE) {
				sameline = true;
			}
		}

		void onNodeEnd(XMLNode& node, XMLEventHandler& handler) {
			if(node.type == ImGuiEnum::BEGIN) {
				ImGui::End();
			}
			
			if(node.type == ImGuiEnum::SAMELINE) {
				sameline = false;
			}
			handler.onNodeEnd(node);
		}

		void traverse(XMLNode& root, XMLEventHandler& handler) {
			onNodeBegin(root, handler);
			for(auto child : root.children) {
				traverse(*child, handler);
			}
			onNodeEnd(root, handler);
		}
	public:
		XMLRenderer(/* args */);
		~XMLRenderer();

		void render(XMLTree& tree, XMLEventHandler& handler) {
			XMLNode root = tree.getRoot();
			traverse(root, handler);
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
