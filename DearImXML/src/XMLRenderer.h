#ifndef XMLRENDERER_H
#define XMLRENDERER_H

#include <XMLTree.h>
#include <XMLEventHandler.h>
#include <XMLDynamicBind.h>
#include <ImGUI/imgui.h>
#include <unordered_map>

namespace ImXML {
	class XMLRenderer
	{
	private:
		int sameline = 0;
		bool popup = false;
		std::unordered_map<std::string, XMLDynamicBind> dynamicBinds;

		void renderMenu(XMLNode& node, XMLEventHandler& handler) {
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

		void renderTree(XMLNode& node, XMLEventHandler& handler) {
			if(node.type == ImGuiEnum::TREENODE) {
				if(ImGui::TreeNode(node.args["label"].c_str())) {
					handler.onEvent(node);
					for(auto child : node.children) {
						renderTree(*child, handler);
					}
					ImGui::TreePop();
				}
			} else if(node.type == ImGuiEnum::TREE) {
				for(auto child : node.children) {
					renderTree(*child, handler);
				}
			} else {
				onNodeBegin(node, handler);
			}
		}

		void onNodeBegin(XMLNode& node, XMLEventHandler& handler, bool inPopup=false) {
			handler.onNodeBegin(node);
			if(!inPopup && popup) {
				return;
			}
			
			if(sameline == 1) {
				sameline++;
			} else if(sameline == 2) {
				ImGui::SameLine();
			}

			if(node.type == ImGuiEnum::COLUMN) {
				ImGui::TableNextColumn();
			}

			if(node.type == ImGuiEnum::ROW) {
				ImGui::TableNextRow(node.flags);
			}

			if(node.type == ImGuiEnum::TABLE) {
				ImGui::BeginTable(node.args["name"].c_str(), std::stoi(node.args["columns"]), node.flags);
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
				ImGui::Begin(node.args["name"].c_str(), nullptr, node.flags);
			}

			if(node.type == ImGuiEnum::GROUP) {
				ImGui::BeginGroup();
			}

			if(node.type == ImGuiEnum::POPUPCONTEXTWINDOW) {
				popup = true;
				if(ImGui::BeginPopupContextWindow()) {
					for(auto child : node.children) {
						traverse(*child, handler, true);
					}
					ImGui::EndPopup();
				}
			}

			if(node.type == ImGuiEnum::COLORPICKER3) {
				ImGui::ColorPicker3(node.args["label"].c_str(), (float*)dynamicBinds.at(node.args["dynamic"]).ptr);
			}
			if(node.type == ImGuiEnum::COLORPICKER4) {
				ImGui::ColorPicker4(node.args["label"].c_str(), (float*)dynamicBinds.at(node.args["dynamic"]).ptr);
			}
			if(node.type == ImGuiEnum::COLOREDIT3) {
				ImGui::ColorEdit3(node.args["label"].c_str(), (float*)dynamicBinds.at(node.args["dynamic"]).ptr);
			}
			if(node.type == ImGuiEnum::COLOREDIT4) {
				ImGui::ColorEdit4(node.args["label"].c_str(), (float*)dynamicBinds.at(node.args["dynamic"]).ptr);
			}

			if(node.type == ImGuiEnum::BUTTON) {
				if(ImGui::Button(node.args["label"].c_str())) {
					handler.onEvent(node);
				}
			}

			if(node.type == ImGuiEnum::TEXT) {
				ImGui::Text("%s", node.args["label"].c_str());
			}

			if(node.type == ImGuiEnum::SLIDERFLOAT) {
				ImGui::SliderFloat(node.args["label"].c_str(), (float*)dynamicBinds.at(node.args["dynamic"]).ptr, std::stof(node.args["min"]), std::stof(node.args["max"]));
			}

			if(node.type == ImGuiEnum::INPUTTEXT) {
				ImGui::InputText(node.args["label"].c_str(), (char*)dynamicBinds.at(node.args["dynamic"]).ptr, dynamicBinds.at(node.args["dynamic"]).size, node.flags);
			}

			if(node.type == ImGuiEnum::TREE) {
				renderTree(node, handler);
			}

			if(node.type == ImGuiEnum::SEPARATOR) {
				ImGui::Separator();
			}

			if(node.type == ImGuiEnum::SAMELINE) {
				sameline = 1;
			}
		}

		void onNodeEnd(XMLNode& node, XMLEventHandler& handler, bool inPopup=false) {
			if(node.type == ImGuiEnum::BEGIN) {
				ImGui::End();
			}
			if(node.type == ImGuiEnum::POPUPCONTEXTWINDOW) {
				popup = false;
			}
			if(node.type == ImGuiEnum::SAMELINE) {
				sameline = false;
			}
			if(node.type == ImGuiEnum::COLUMN) {
				ImGui::NextColumn();
			}
			if(node.type == ImGuiEnum::GROUP) {
				ImGui::EndGroup();
			}
			if(node.type == ImGuiEnum::TABLE) {
				ImGui::EndTable();
			}
			handler.onNodeEnd(node);
		}

		void traverse(XMLNode& root, XMLEventHandler& handler, bool inPopup=false) {
			onNodeBegin(root, handler, inPopup);
			for(auto child : root.children) {
				traverse(*child, handler, inPopup);
			}
			onNodeEnd(root, handler, inPopup);
		}
	public:
		XMLRenderer(/* args */);
		~XMLRenderer();

		void render(XMLTree& tree, XMLEventHandler& handler) {
			XMLNode root = tree.getRoot();
			traverse(root, handler);
		}

		void addDynamicBind(const std::string& name, const XMLDynamicBind& bind) {
			dynamicBinds.insert({name, bind});
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
