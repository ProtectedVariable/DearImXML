#include "XMLRenderer.h"

namespace ImXML {

void XMLRenderer::renderMenu(XMLNode& node, XMLEventHandler& handler) {
    if (node.type == ImGuiEnum::MENU) {
        if (ImGui::BeginMenu(node.args["label"].c_str())) {
            for (auto child : node.children) {
                if (child->type == ImGuiEnum::MENUITEM) {
                    if (ImGui::MenuItem(child->args["label"].c_str())) {
                        handler.onEvent(*child);
                    }
                } else if (child->type == ImGuiEnum::MENU) {
                    renderMenu(*child, handler);
                }
            }
            ImGui::EndMenu();
        }
    }
}

void XMLRenderer::renderTree(XMLNode& node, XMLEventHandler& handler) {
    if (node.type == ImGuiEnum::TREENODE) {
        if (ImGui::TreeNode(node.args["label"].c_str())) {
            handler.onEvent(node);
            for (auto child : node.children) {
                renderTree(*child, handler);
            }
            ImGui::TreePop();
        }
    } else if (node.type == ImGuiEnum::TREE) {
        for (auto child : node.children) {
            renderTree(*child, handler);
        }
    } else {
        onNodeBegin(node, handler);
    }
}

bool XMLRenderer::onNodeBegin(XMLNode& node, XMLEventHandler& handler) {
    handler.onNodeBegin(node);
    if (sameline == 1) {
        sameline++;
    } else if (sameline == 2) {
        ImGui::SameLine();
    }
    //TODO: Make a switch statement instead duh
    if (node.type == ImGuiEnum::COLUMN) {
        ImGui::TableNextColumn();
    }

    if (node.type == ImGuiEnum::BEGINPOPUPMODAL) {
        if (!ImGui::BeginPopupModal(node.args["name"].c_str(), nullptr, node.flags)) {
            return false;
        }
    }

    if (node.type == ImGuiEnum::HEADER) {
        ImGui::TableHeadersRow();
    }

    if (node.type == ImGuiEnum::ROW) {
        ImGui::TableNextRow(node.flags);
    }

    if (node.type == ImGuiEnum::TABLE) {
        if (!ImGui::BeginTable(node.args["name"].c_str(), std::stoi(node.args["columns"]), node.flags)) {
            return false;
        }
    }

    if (node.type == ImGuiEnum::SETUPCOLUMN) {
        ImGui::TableSetupColumn(node.args["label"].c_str(), node.flags, std::stof(node.args["width"]));
    }

    if (node.type == ImGuiEnum::MENUBAR) {
        if (!ImGui::BeginMenuBar()) {
            return false;
        }
    }

    if (node.type == ImGuiEnum::BEGIN) {
        if (!ImGui::Begin(node.args["name"].c_str(), nullptr, node.flags)) {
            return false;
        }
    }

    if (node.type == ImGuiEnum::GROUP) {
        ImGui::BeginGroup();
    }

    if (node.type == ImGuiEnum::POPUPCONTEXTWINDOW) {
        if (!ImGui::BeginPopupContextWindow()) {
            return false;
        }
    }

    auto bind = getDynamicBind(node);
    if (node.type == ImGuiEnum::COLORPICKER3) {
        if (ImGui::ColorPicker3(node.args["label"].c_str(), (float*)bind.ptr)) {
            handler.onEvent(node);
        }
    }
    if (node.type == ImGuiEnum::COLORPICKER4) {
        if (ImGui::ColorPicker4(node.args["label"].c_str(), (float*)bind.ptr)) {
            handler.onEvent(node);
        }
    }
    if (node.type == ImGuiEnum::COLOREDIT3) {
        if (ImGui::ColorEdit3(node.args["label"].c_str(), (float*)bind.ptr)) {
            handler.onEvent(node);
        }
    }
    if (node.type == ImGuiEnum::COLOREDIT4) {
        if (ImGui::ColorEdit4(node.args["label"].c_str(), (float*)bind.ptr)) {
            handler.onEvent(node);
        }
    }

    if (node.type == ImGuiEnum::BUTTON) {
        if (ImGui::Button(node.args["label"].c_str())) {
            handler.onEvent(node);
        }
    }

    if (node.type == ImGuiEnum::TEXT) {
        ImGui::Text("%s", node.args["label"].c_str());
    }

    if (node.type == ImGuiEnum::SLIDERFLOAT) {
        if (ImGui::SliderFloat(node.args["label"].c_str(), (float*)bind.ptr, std::stof(node.args["min"]), std::stof(node.args["max"]))) {
            handler.onEvent(node);
        }
    }

    if (node.type == ImGuiEnum::INPUTTEXT) {
        bool evt = false;
        if (node.args.contains("hint")) {
            evt = ImGui::InputTextWithHint(node.args["label"].c_str(), node.args["hint"].c_str(), (char*) bind.ptr, bind.size, node.flags);
        } else {
            evt = ImGui::InputText(node.args["label"].c_str(), (char*) bind.ptr, bind.size, node.flags);
        }
        if (evt) {
            handler.onEvent(node);
        }
    }

    if (node.type == ImGuiEnum::TREE) {
        renderTree(node, handler);
    }

    if (node.type == ImGuiEnum::SEPARATOR) {
        ImGui::Separator();
    }

    if (node.type == ImGuiEnum::SAMELINE) {
        sameline = 1;
    }

    if (node.type == ImGuiEnum::COMBO) {
        if (!ImGui::BeginCombo(node.arg<std::string>("label").c_str(), node.arg<std::string>("preview_value").c_str(), node.flags)) {
            return false;
        }
    }

    if (node.type == ImGuiEnum::CHECKBOX) {
        if (ImGui::Checkbox(node.arg<std::string>("label").c_str(), (bool*) bind.ptr))
            handler.onEvent(node);
    }

    if (node.type == ImGuiEnum::CHILD) {
        //TODO: FIXME
        if (!ImGui::BeginChild(node.arg<std::string>("label").c_str())) {
            return false;
        }
    }

    return true;
}

void XMLRenderer::onNodeEnd(XMLNode& node, XMLEventHandler& handler) {
    //TODO: Make a switch statement instead duh
    if (node.type == ImGuiEnum::BEGINPOPUPMODAL) {
        ImGui::EndPopup();
    }
    if (node.type == ImGuiEnum::POPUPCONTEXTWINDOW) {
        ImGui::EndPopup();
    }
    if (node.type == ImGuiEnum::COMBO) {
        ImGui::EndCombo();
    }
    if (node.type == ImGuiEnum::CHILD) {
        ImGui::EndChild();
    }
    if (node.type == ImGuiEnum::BEGIN) {
        ImGui::End();
    }

    if (node.type == ImGuiEnum::SAMELINE) {
        sameline = false; //FIXME: WTF am I doing here
    }
    if (node.type == ImGuiEnum::COLUMN) {
        ImGui::NextColumn();
    }
    if (node.type == ImGuiEnum::GROUP) {
        ImGui::EndGroup();
    }
    if (node.type == ImGuiEnum::TABLE) {
        ImGui::EndTable();
    }
    handler.onNodeEnd(node);
}

void XMLRenderer::traverse(XMLNode& root, XMLEventHandler& handler) {
    if (onNodeBegin(root, handler)) {
        for (auto child : root.children) {
            traverse(*child, handler);
        }
        onNodeEnd(root, handler);
    }
}

XMLDynamicBind XMLRenderer::getDynamicBind(const XMLNode& node) {
    if (node.args.contains("dynamic")) {
        if (dynamicBinds.contains(node.args.at("dynamic"))) {
            return dynamicBinds.at(node.args.at("dynamic"));
        }
    }
    return {nullptr, 0, Float};
}

void XMLRenderer::render(XMLTree& tree, XMLEventHandler& handler) {
    XMLNode root = tree.getRoot();
    traverse(root, handler);
}

void XMLRenderer::addDynamicBind(const std::string& name, const XMLDynamicBind& bind) {
    dynamicBinds.insert({name, bind});
}

XMLRenderer::XMLRenderer() {
}

XMLRenderer::~XMLRenderer() {
}

}  // namespace ImXML
