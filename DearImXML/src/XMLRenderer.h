#ifndef XMLRENDERER_H
#define XMLRENDERER_H

#include <XMLTree.h>
#include <XMLEventHandler.h>
#include <ImGUI/imgui.h>

namespace ImXML {
	class XMLRenderer
	{
	private:
		int sameline = 0;

		void renderMenu(XMLNode& node, XMLEventHandler& handler)  {
			if(node.type == ImGuiEnum::MENU) {
				if(ImGui::BeginMenu(node.args["label"].c_str())) {
					for(auto child : node.children) {
						if(child->type == ImGuiEnum::MENUITEM) {
							if(ImGui::MenuItem(child->args["label"].c_str())) {
								handler.onEvent(*child);
							}
						} else if(child->type == ImGuiEnum::MENU) {
							renderMenu(*child, handler);
						}
					}
					ImGui::EndMenu();
				}
			}
		}

		void onNodeBegin(XMLNode& node, XMLEventHandler& handler) {
			handler.onNodeBegin(node);
			
			if(sameline == 1) {
				sameline++;
			} else if(sameline == 2) {
				ImGui::SameLine();
			}

			if(node.type == ImGuiEnum::MENUBAR) {
				if(ImGui::BeginMenuBar()) {
					for(auto child : node.children) {
						renderMenu(*child, handler);
					}
					ImGui::EndMenuBar();
				}
			}
			
			if(node.type == ImGuiEnum::BEGIN) {
				auto c = std::string("");
				ImGui::Begin(node.args["name"].c_str(), nullptr, node.flags);
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
				sameline = 1;
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
