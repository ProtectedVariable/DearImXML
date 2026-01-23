#include "XMLRenderer.h"

#include <stdexcept>

namespace ImXML {

bool XMLRenderer::onNodeBegin(XMLNode& node, XMLEventHandler& handler) {
    handler.onNodeBegin(node);

    bool shouldRenderChildren = true;
    bool eventTriggered = false;

    switch (node.type) {
        // Layout & Formatting
        case ImGuiEnum::COLUMN:
            ImGui::TableNextColumn();
            break;

        case ImGuiEnum::SAMELINE:
            ImGui::SameLine();
            break;

        case ImGuiEnum::SEPARATOR:
            ImGui::Separator();
            break;

        case ImGuiEnum::SEPARATORTEXT:
            ImGui::SeparatorText(node.arg<std::string>("label").c_str());
            break;

        case ImGuiEnum::HEADER:
            ImGui::TableHeadersRow();
            break;

        case ImGuiEnum::ROW:
            ImGui::TableNextRow(node.flags);
            break;

        case ImGuiEnum::SETUPCOLUMN:
            ImGui::TableSetupColumn(node.arg<std::string>("label").c_str(), node.flags, std::stof(node.args["width"]));
            break;

        case ImGuiEnum::TEXT:
            ImGui::Text("%s", node.arg<std::string>("label").c_str());
            break;

        case ImGuiEnum::GROUP:
            ImGui::BeginGroup();
            break;

        // Containers (Return false if collapsed/closed)
        case ImGuiEnum::BEGINPOPUPMODAL:
            shouldRenderChildren = ImGui::BeginPopupModal(node.args["name"].c_str(), nullptr, node.flags);
            break;

        case ImGuiEnum::TABLE:
            shouldRenderChildren = ImGui::BeginTable(node.args["name"].c_str(), std::stoi(node.args["columns"]), node.flags);
            break;

        case ImGuiEnum::MENUBAR:
            shouldRenderChildren = ImGui::BeginMenuBar();
            break;

        case ImGuiEnum::MAINMENUBAR:
            shouldRenderChildren = ImGui::BeginMainMenuBar();
            break;

        case ImGuiEnum::BEGIN:
            shouldRenderChildren = ImGui::Begin(node.args["name"].c_str(), nullptr, node.flags);
            break;

        case ImGuiEnum::POPUPCONTEXTWINDOW:
            shouldRenderChildren = ImGui::BeginPopupContextWindow();
            break;

        case ImGuiEnum::CHILD:
            shouldRenderChildren = ImGui::BeginChild(node.arg<std::string>("label").c_str(), {0, 0}, node.childflags, node.flags);
            break;

        case ImGuiEnum::COMBO:
            shouldRenderChildren =
                ImGui::BeginCombo(node.arg<std::string>("label").c_str(), node.arg<std::string>("preview_value").c_str(), node.flags);
            break;

        case ImGuiEnum::MENU:
            shouldRenderChildren = ImGui::BeginMenu(node.arg<std::string>("label").c_str());
            break;

        case ImGuiEnum::TREENODE:
            shouldRenderChildren = ImGui::TreeNodeEx(node.arg<std::string>("label").c_str(), node.flags);
            break;

        case ImGuiEnum::TABBAR:
            shouldRenderChildren = ImGui::BeginTabBar(node.arg<std::string>("id").c_str(), node.flags);
            break;
        case ImGuiEnum::TABITEM:
            shouldRenderChildren = ImGui::BeginTabItem(node.arg<std::string>("label").c_str(), nullptr, node.flags);
            break;

        // Interactive Widgets
        case ImGuiEnum::MENUITEM:
            eventTriggered = ImGui::MenuItem(node.arg<std::string>("label").c_str(), node.arg<std::string>("shortcut").c_str());
            break;

        case ImGuiEnum::COLORPICKER3:
            eventTriggered = ImGui::ColorPicker3(node.arg<std::string>("label").c_str(), (float*) getDynamicBind(node).ptr, node.flags);
            break;

        case ImGuiEnum::COLORPICKER4:
            eventTriggered = ImGui::ColorPicker4(node.arg<std::string>("label").c_str(), (float*) getDynamicBind(node).ptr, node.flags);
            break;

        case ImGuiEnum::COLOREDIT3:
            eventTriggered = ImGui::ColorEdit3(node.arg<std::string>("label").c_str(), (float*) getDynamicBind(node).ptr, node.flags);
            break;

        case ImGuiEnum::COLOREDIT4:
            eventTriggered = ImGui::ColorEdit4(node.arg<std::string>("label").c_str(), (float*) getDynamicBind(node).ptr, node.flags);
            break;

        case ImGuiEnum::BUTTON:
            eventTriggered = ImGui::Button(node.arg<std::string>("label").c_str());
            break;

        case ImGuiEnum::SLIDERFLOAT:
            eventTriggered = ImGui::SliderFloat(node.arg<std::string>("label").c_str(), (float*) getDynamicBind(node).ptr, node.arg<float>("min"),
                                                node.arg<float>("max"));
            break;

        case ImGuiEnum::CHECKBOX:
            eventTriggered = ImGui::Checkbox(node.arg<std::string>("label").c_str(), (bool*) getDynamicBind(node).ptr);
            break;

        case ImGuiEnum::SELECTABLE:
            eventTriggered = ImGui::Selectable(node.arg<std::string>("label").c_str(), node.arg<bool>("selected"), node.flags);
            break;

        case ImGuiEnum::INPUTTEXT: {
            auto bind = getDynamicBind(node);
            if (node.args.contains("hint")) {
                eventTriggered = ImGui::InputTextWithHint(node.arg<std::string>("label").c_str(), node.arg<std::string>("hint").c_str(),
                                                          (char*) bind.ptr, bind.size, node.flags);
            } else {
                eventTriggered = ImGui::InputText(node.arg<std::string>("label").c_str(), (char*) bind.ptr, bind.size, node.flags);
            }
            break;
        }

        case ImGuiEnum::INPUTTEXTMULTILINE:
            auto bind = getDynamicBind(node);
            eventTriggered = ImGui::InputTextMultiline(node.arg<std::string>("label").c_str(), (char*) bind.ptr, bind.size, ImVec2(0, 0), node.flags);
            break;

        case ImGuiEnum::INPUTFLOAT: {
            auto bind = getDynamicBind(node);
            std::string fmt = node.args.contains("format") ? node.args["format"] : "%.3f";
            eventTriggered = ImGui::InputFloat(node.arg<std::string>("label").c_str(), (float*) bind.ptr, node.arg<float>("step"),
                                               node.arg<float>("step_fast"), fmt.c_str(), node.flags);

            break;
        }

        case ImGuiEnum::PLACEHOLDER:
            break;

        default:
            throw std::invalid_argument("Unhandled node type");
    }

    if (eventTriggered) {
        handler.onEvent(node);
    }

    return shouldRenderChildren;
}

void XMLRenderer::onNodeEnd(XMLNode& node, XMLEventHandler& handler) {
    switch (node.type) {
        case ImGuiEnum::BEGINPOPUPMODAL:
        case ImGuiEnum::POPUPCONTEXTWINDOW:
            ImGui::EndPopup();
            break;

        case ImGuiEnum::COMBO:
            ImGui::EndCombo();
            break;

        case ImGuiEnum::CHILD:
            ImGui::EndChild();
            break;

        case ImGuiEnum::BEGIN:
            ImGui::End();
            break;

        case ImGuiEnum::MENUBAR:
            ImGui::EndMenuBar();
            break;

        case ImGuiEnum::MAINMENUBAR:
            ImGui::EndMainMenuBar();
            break;

        case ImGuiEnum::MENU:
            ImGui::EndMenu();
            break;

        case ImGuiEnum::TREENODE:
            ImGui::TreePop();
            break;

        case ImGuiEnum::COLUMN:
            ImGui::NextColumn();
            break;

        case ImGuiEnum::GROUP:
            ImGui::EndGroup();
            break;

        case ImGuiEnum::TABLE:
            ImGui::EndTable();
            break;

        case ImGuiEnum::TABBAR:
            ImGui::EndTabBar();
            break;
        case ImGuiEnum::TABITEM:
            ImGui::EndTabItem();
            break;

        default:
            break;
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
