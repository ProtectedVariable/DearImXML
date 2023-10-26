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
#include <algorithm>

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
				parsed |= flagnames.at(trim(flag));
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
			std::transform(name.begin(), name.end(), name.begin(), ::tolower);
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
			XMLNode* lastparent = nullptr;
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
		{ "inputtext", ImGuiEnum::INPUTTEXT },
		{ "treenode", ImGuiEnum::TREENODE },
		{ "tree", ImGuiEnum::TREE },
		{ "table", ImGuiEnum::TABLE },
		{ "row", ImGuiEnum::ROW },
		{ "column", ImGuiEnum::COLUMN },
		{ "group", ImGuiEnum::GROUP },
		{ "colorpicker3", ImGuiEnum::COLORPICKER3 },
		{ "colorpicker4", ImGuiEnum::COLORPICKER4 },
		{ "coloredit3", ImGuiEnum::COLOREDIT3 },
		{ "coloredit4", ImGuiEnum::COLOREDIT4 },
		{ "popupcontextwindow", ImGuiEnum::POPUPCONTEXTWINDOW },
		{ "separator", ImGuiEnum::SEPARATOR },

	};

	const std::unordered_map<std::string, int> XMLReader::flagnames = {
		//Window flags
		{ "ImGuiWindowFlags_None", ImGuiWindowFlags_None },
		{ "ImGuiWindowFlags_NoTitleBar", ImGuiWindowFlags_NoTitleBar },
		{ "ImGuiWindowFlags_NoResize", ImGuiWindowFlags_NoResize },
		{ "ImGuiWindowFlags_NoMove", ImGuiWindowFlags_NoMove },
		{ "ImGuiWindowFlags_NoScrollbar", ImGuiWindowFlags_NoScrollbar },
		{ "ImGuiWindowFlags_NoScrollWithMouse", ImGuiWindowFlags_NoScrollWithMouse },
		{ "ImGuiWindowFlags_NoCollapse", ImGuiWindowFlags_NoCollapse },
		{ "ImGuiWindowFlags_AlwaysAutoResize", ImGuiWindowFlags_AlwaysAutoResize },
		{ "ImGuiWindowFlags_NoBackground", ImGuiWindowFlags_NoBackground },
		{ "ImGuiWindowFlags_NoSavedSettings", ImGuiWindowFlags_NoSavedSettings },
		{ "ImGuiWindowFlags_NoMouseInputs", ImGuiWindowFlags_NoMouseInputs },
		{ "ImGuiWindowFlags_MenuBar", ImGuiWindowFlags_MenuBar },
		{ "ImGuiWindowFlags_HorizontalScrollbar", ImGuiWindowFlags_HorizontalScrollbar },
		{ "ImGuiWindowFlags_NoFocusOnAppearing", ImGuiWindowFlags_NoFocusOnAppearing },
		{ "ImGuiWindowFlags_NoBringToFrontOnFocus", ImGuiWindowFlags_NoBringToFrontOnFocus },
		{ "ImGuiWindowFlags_AlwaysVerticalScrollbar", ImGuiWindowFlags_AlwaysVerticalScrollbar },
		{ "ImGuiWindowFlags_AlwaysHorizontalScrollbar", ImGuiWindowFlags_AlwaysHorizontalScrollbar },
		{ "ImGuiWindowFlags_AlwaysUseWindowPadding", ImGuiWindowFlags_AlwaysUseWindowPadding },
		{ "ImGuiWindowFlags_NoNavInputs", ImGuiWindowFlags_NoNavInputs },
		{ "ImGuiWindowFlags_NoNavFocus", ImGuiWindowFlags_NoNavFocus },
		{ "ImGuiWindowFlags_UnsavedDocument", ImGuiWindowFlags_UnsavedDocument },
		{ "ImGuiWindowFlags_NoNav", ImGuiWindowFlags_NoNav },
		{ "ImGuiWindowFlags_NoDecoration", ImGuiWindowFlags_NoDecoration },
		{ "ImGuiWindowFlags_NoInputs", ImGuiWindowFlags_NoInputs },
		{ "ImGuiWindowFlags_NavFlattened", ImGuiWindowFlags_NavFlattened },
		{ "ImGuiWindowFlags_ChildWindow", ImGuiWindowFlags_ChildWindow },
		{ "ImGuiWindowFlags_Tooltip", ImGuiWindowFlags_Tooltip },
		{ "ImGuiWindowFlags_Popup", ImGuiWindowFlags_Popup },
		{ "ImGuiWindowFlags_Modal", ImGuiWindowFlags_Modal },
		{ "ImGuiWindowFlags_ChildMenu", ImGuiWindowFlags_ChildMenu },

		//InputText flags
		{ "ImGuiInputTextFlags_None", ImGuiInputTextFlags_None },
		{ "ImGuiInputTextFlags_CharsDecimal", ImGuiInputTextFlags_CharsDecimal },
		{ "ImGuiInputTextFlags_CharsHexadecimal", ImGuiInputTextFlags_CharsHexadecimal },
		{ "ImGuiInputTextFlags_CharsUppercase", ImGuiInputTextFlags_CharsUppercase },
		{ "ImGuiInputTextFlags_CharsNoBlank", ImGuiInputTextFlags_CharsNoBlank },
		{ "ImGuiInputTextFlags_AutoSelectAll", ImGuiInputTextFlags_AutoSelectAll },
		{ "ImGuiInputTextFlags_EnterReturnsTrue", ImGuiInputTextFlags_EnterReturnsTrue },
		{ "ImGuiInputTextFlags_CallbackCompletion", ImGuiInputTextFlags_CallbackCompletion },
		{ "ImGuiInputTextFlags_CallbackHistory", ImGuiInputTextFlags_CallbackHistory },
		{ "ImGuiInputTextFlags_CallbackAlways", ImGuiInputTextFlags_CallbackAlways },
		{ "ImGuiInputTextFlags_CallbackCharFilter", ImGuiInputTextFlags_CallbackCharFilter },
		{ "ImGuiInputTextFlags_AllowTabInput", ImGuiInputTextFlags_AllowTabInput },
		{ "ImGuiInputTextFlags_CtrlEnterForNewLine", ImGuiInputTextFlags_CtrlEnterForNewLine },
		{ "ImGuiInputTextFlags_NoHorizontalScroll", ImGuiInputTextFlags_NoHorizontalScroll },
		{ "ImGuiInputTextFlags_AlwaysOverwrite", ImGuiInputTextFlags_AlwaysOverwrite },
		{ "ImGuiInputTextFlags_ReadOnly", ImGuiInputTextFlags_ReadOnly },
		{ "ImGuiInputTextFlags_Password", ImGuiInputTextFlags_Password },
		{ "ImGuiInputTextFlags_NoUndoRedo", ImGuiInputTextFlags_NoUndoRedo },
		{ "ImGuiInputTextFlags_CharsScientific", ImGuiInputTextFlags_CharsScientific },
		{ "ImGuiInputTextFlags_CallbackResize", ImGuiInputTextFlags_CallbackResize },
		{ "ImGuiInputTextFlags_CallbackEdit", ImGuiInputTextFlags_CallbackEdit },
		{ "ImGuiInputTextFlags_Multiline", ImGuiInputTextFlags_Multiline },
		{ "ImGuiInputTextFlags_NoMarkEdited", ImGuiInputTextFlags_NoMarkEdited },

		//TreeNode Flags
		{ "ImGuiTreeNodeFlags_None", ImGuiTreeNodeFlags_None },
		{ "ImGuiTreeNodeFlags_Selected", ImGuiTreeNodeFlags_Selected },
		{ "ImGuiTreeNodeFlags_Framed", ImGuiTreeNodeFlags_Framed },
		{ "ImGuiTreeNodeFlags_AllowItemOverlap", ImGuiTreeNodeFlags_AllowItemOverlap },
		{ "ImGuiTreeNodeFlags_NoTreePushOnOpen", ImGuiTreeNodeFlags_NoTreePushOnOpen },
		{ "ImGuiTreeNodeFlags_NoAutoOpenOnLog", ImGuiTreeNodeFlags_NoAutoOpenOnLog },
		{ "ImGuiTreeNodeFlags_DefaultOpen", ImGuiTreeNodeFlags_DefaultOpen },
		{ "ImGuiTreeNodeFlags_OpenOnDoubleClick", ImGuiTreeNodeFlags_OpenOnDoubleClick },
		{ "ImGuiTreeNodeFlags_OpenOnArrow", ImGuiTreeNodeFlags_OpenOnArrow },
		{ "ImGuiTreeNodeFlags_Leaf", ImGuiTreeNodeFlags_Leaf },
		{ "ImGuiTreeNodeFlags_Bullet", ImGuiTreeNodeFlags_Bullet },
		{ "ImGuiTreeNodeFlags_FramePadding", ImGuiTreeNodeFlags_FramePadding },
		{ "ImGuiTreeNodeFlags_SpanAvailWidth", ImGuiTreeNodeFlags_SpanAvailWidth },
		{ "ImGuiTreeNodeFlags_SpanFullWidth", ImGuiTreeNodeFlags_SpanFullWidth },
		{ "ImGuiTreeNodeFlags_NavLeftJumpsBackHere", ImGuiTreeNodeFlags_NavLeftJumpsBackHere },
		{ "ImGuiTreeNodeFlags_CollapsingHeader", ImGuiTreeNodeFlags_CollapsingHeader },

		//enum ImGuiPopupFlags_
		{ "ImGuiPopupFlags_None", ImGuiPopupFlags_None },
		{ "ImGuiPopupFlags_MouseButtonLeft", ImGuiPopupFlags_MouseButtonLeft },
		{ "ImGuiPopupFlags_MouseButtonRight", ImGuiPopupFlags_MouseButtonRight },
		{ "ImGuiPopupFlags_MouseButtonMiddle", ImGuiPopupFlags_MouseButtonMiddle },
		{ "ImGuiPopupFlags_MouseButtonMask_", ImGuiPopupFlags_MouseButtonMask_ },
		{ "ImGuiPopupFlags_MouseButtonDefault_", ImGuiPopupFlags_MouseButtonDefault_ },
		{ "ImGuiPopupFlags_NoOpenOverExistingPopup", ImGuiPopupFlags_NoOpenOverExistingPopup },
		{ "ImGuiPopupFlags_NoOpenOverItems", ImGuiPopupFlags_NoOpenOverItems },
		{ "ImGuiPopupFlags_AnyPopupId", ImGuiPopupFlags_AnyPopupId },
		{ "ImGuiPopupFlags_AnyPopupLevel", ImGuiPopupFlags_AnyPopupLevel },
		{ "ImGuiPopupFlags_AnyPopup", ImGuiPopupFlags_AnyPopup },
		//enum ImGuiSelectableFlags_
		{ "ImGuiSelectableFlags_None", ImGuiSelectableFlags_None },
		{ "ImGuiSelectableFlags_DontClosePopups", ImGuiSelectableFlags_DontClosePopups },
		{ "ImGuiSelectableFlags_SpanAllColumns", ImGuiSelectableFlags_SpanAllColumns },
		{ "ImGuiSelectableFlags_AllowDoubleClick", ImGuiSelectableFlags_AllowDoubleClick },
		{ "ImGuiSelectableFlags_Disabled", ImGuiSelectableFlags_Disabled },
		{ "ImGuiSelectableFlags_AllowItemOverlap", ImGuiSelectableFlags_AllowItemOverlap },
		//enum ImGuiComboFlags_
		{ "ImGuiComboFlags_None", ImGuiComboFlags_None },
		{ "ImGuiComboFlags_PopupAlignLeft", ImGuiComboFlags_PopupAlignLeft },
		{ "ImGuiComboFlags_HeightSmall", ImGuiComboFlags_HeightSmall },
		{ "ImGuiComboFlags_HeightRegular", ImGuiComboFlags_HeightRegular },
		{ "ImGuiComboFlags_HeightLarge", ImGuiComboFlags_HeightLarge },
		{ "ImGuiComboFlags_HeightLargest", ImGuiComboFlags_HeightLargest },
		{ "ImGuiComboFlags_NoArrowButton", ImGuiComboFlags_NoArrowButton },
		{ "ImGuiComboFlags_NoPreview", ImGuiComboFlags_NoPreview },
		{ "ImGuiComboFlags_HeightMask_", ImGuiComboFlags_HeightMask_ },
		//enum ImGuiTabBarFlags_
		{ "ImGuiTabBarFlags_None", ImGuiTabBarFlags_None },
		{ "ImGuiTabBarFlags_Reorderable", ImGuiTabBarFlags_Reorderable },
		{ "ImGuiTabBarFlags_AutoSelectNewTabs", ImGuiTabBarFlags_AutoSelectNewTabs },
		{ "ImGuiTabBarFlags_TabListPopupButton", ImGuiTabBarFlags_TabListPopupButton },
		{ "ImGuiTabBarFlags_NoCloseWithMiddleMouseButton", ImGuiTabBarFlags_NoCloseWithMiddleMouseButton },
		{ "ImGuiTabBarFlags_NoTabListScrollingButtons", ImGuiTabBarFlags_NoTabListScrollingButtons },
		{ "ImGuiTabBarFlags_NoTooltip", ImGuiTabBarFlags_NoTooltip },
		{ "ImGuiTabBarFlags_FittingPolicyResizeDown", ImGuiTabBarFlags_FittingPolicyResizeDown },
		{ "ImGuiTabBarFlags_FittingPolicyScroll", ImGuiTabBarFlags_FittingPolicyScroll },
		{ "ImGuiTabBarFlags_FittingPolicyMask_", ImGuiTabBarFlags_FittingPolicyMask_ },
		{ "ImGuiTabBarFlags_FittingPolicyDefault_", ImGuiTabBarFlags_FittingPolicyDefault_ },
		//enum ImGuiTabItemFlags_
		{ "ImGuiTabItemFlags_None", ImGuiTabItemFlags_None },
		{ "ImGuiTabItemFlags_UnsavedDocument", ImGuiTabItemFlags_UnsavedDocument },
		{ "ImGuiTabItemFlags_SetSelected", ImGuiTabItemFlags_SetSelected },
		{ "ImGuiTabItemFlags_NoCloseWithMiddleMouseButton", ImGuiTabItemFlags_NoCloseWithMiddleMouseButton },
		{ "ImGuiTabItemFlags_NoPushId", ImGuiTabItemFlags_NoPushId },
		{ "ImGuiTabItemFlags_NoTooltip", ImGuiTabItemFlags_NoTooltip },
		{ "ImGuiTabItemFlags_NoReorder", ImGuiTabItemFlags_NoReorder },
		{ "ImGuiTabItemFlags_Leading", ImGuiTabItemFlags_Leading },
		{ "ImGuiTabItemFlags_Trailing", ImGuiTabItemFlags_Trailing },
		//enum ImGuiFocusedFlags_
		{ "ImGuiFocusedFlags_None", ImGuiFocusedFlags_None },
		{ "ImGuiFocusedFlags_ChildWindows", ImGuiFocusedFlags_ChildWindows },
		{ "ImGuiFocusedFlags_RootWindow", ImGuiFocusedFlags_RootWindow },
		{ "ImGuiFocusedFlags_AnyWindow", ImGuiFocusedFlags_AnyWindow },
		{ "ImGuiFocusedFlags_RootAndChildWindows", ImGuiFocusedFlags_RootAndChildWindows },
		//enum ImGuiHoveredFlags_
		{ "ImGuiHoveredFlags_None", ImGuiHoveredFlags_None },
		{ "ImGuiHoveredFlags_ChildWindows", ImGuiHoveredFlags_ChildWindows },
		{ "ImGuiHoveredFlags_RootWindow", ImGuiHoveredFlags_RootWindow },
		{ "ImGuiHoveredFlags_AnyWindow", ImGuiHoveredFlags_AnyWindow },
		{ "ImGuiHoveredFlags_AllowWhenBlockedByPopup", ImGuiHoveredFlags_AllowWhenBlockedByPopup },
		{ "ImGuiHoveredFlags_AllowWhenBlockedByActiveItem", ImGuiHoveredFlags_AllowWhenBlockedByActiveItem },
		{ "ImGuiHoveredFlags_AllowWhenOverlapped", ImGuiHoveredFlags_AllowWhenOverlapped },
		{ "ImGuiHoveredFlags_AllowWhenDisabled", ImGuiHoveredFlags_AllowWhenDisabled },
		{ "ImGuiHoveredFlags_RectOnly", ImGuiHoveredFlags_RectOnly },
		{ "ImGuiHoveredFlags_RootAndChildWindows", ImGuiHoveredFlags_RootAndChildWindows },
		//enum ImGuiDragDropFlags_
		{ "ImGuiDragDropFlags_None", ImGuiDragDropFlags_None },
		{ "ImGuiDragDropFlags_SourceNoPreviewTooltip", ImGuiDragDropFlags_SourceNoPreviewTooltip },
		{ "ImGuiDragDropFlags_SourceNoDisableHover", ImGuiDragDropFlags_SourceNoDisableHover },
		{ "ImGuiDragDropFlags_SourceNoHoldToOpenOthers", ImGuiDragDropFlags_SourceNoHoldToOpenOthers },
		{ "ImGuiDragDropFlags_SourceAllowNullID", ImGuiDragDropFlags_SourceAllowNullID },
		{ "ImGuiDragDropFlags_SourceExtern", ImGuiDragDropFlags_SourceExtern },
		{ "ImGuiDragDropFlags_SourceAutoExpirePayload", ImGuiDragDropFlags_SourceAutoExpirePayload },
		{ "ImGuiDragDropFlags_AcceptBeforeDelivery", ImGuiDragDropFlags_AcceptBeforeDelivery },
		{ "ImGuiDragDropFlags_AcceptNoDrawDefaultRect", ImGuiDragDropFlags_AcceptNoDrawDefaultRect },
		{ "ImGuiDragDropFlags_AcceptNoPreviewTooltip", ImGuiDragDropFlags_AcceptNoPreviewTooltip },
		{ "ImGuiDragDropFlags_AcceptPeekOnly", ImGuiDragDropFlags_AcceptPeekOnly },
        //enum ImGuiTableFlags_
        {"ImGuiTableFlags_None", ImGuiTableFlags_None},
        {"ImGuiTableFlags_Resizable", ImGuiTableFlags_Resizable},
        {"ImGuiTableFlags_Reorderable", ImGuiTableFlags_Reorderable},
        {"ImGuiTableFlags_Hideable", ImGuiTableFlags_Hideable},
        {"ImGuiTableFlags_Sortable", ImGuiTableFlags_Sortable},
        {"ImGuiTableFlags_NoSavedSettings", ImGuiTableFlags_NoSavedSettings},
        {"ImGuiTableFlags_ContextMenuInBody", ImGuiTableFlags_ContextMenuInBody},
        {"ImGuiTableFlags_RowBg", ImGuiTableFlags_RowBg},
        {"ImGuiTableFlags_BordersInnerH", ImGuiTableFlags_BordersInnerH},
        {"ImGuiTableFlags_BordersOuterH", ImGuiTableFlags_BordersOuterH},
        {"ImGuiTableFlags_BordersInnerV", ImGuiTableFlags_BordersInnerV},
        {"ImGuiTableFlags_BordersOuterV", ImGuiTableFlags_BordersOuterV},
        {"ImGuiTableFlags_BordersH", ImGuiTableFlags_BordersH},
        {"ImGuiTableFlags_BordersV", ImGuiTableFlags_BordersV},
        {"ImGuiTableFlags_BordersInner", ImGuiTableFlags_BordersInner},
        {"ImGuiTableFlags_BordersOuter", ImGuiTableFlags_BordersOuter},
        {"ImGuiTableFlags_Borders", ImGuiTableFlags_Borders},
        {"ImGuiTableFlags_NoBordersInBody", ImGuiTableFlags_NoBordersInBody},
        {"ImGuiTableFlags_NoBordersInBodyUntilResize", ImGuiTableFlags_NoBordersInBodyUntilResize},
        {"ImGuiTableFlags_SizingFixedFit", ImGuiTableFlags_SizingFixedFit},
        {"ImGuiTableFlags_SizingFixedSame", ImGuiTableFlags_SizingFixedSame},
        {"ImGuiTableFlags_SizingStretchProp", ImGuiTableFlags_SizingStretchProp},
        {"ImGuiTableFlags_SizingStretchSame", ImGuiTableFlags_SizingStretchSame},
        {"ImGuiTableFlags_NoHostExtendX", ImGuiTableFlags_NoHostExtendX},
        {"ImGuiTableFlags_NoHostExtendY", ImGuiTableFlags_NoHostExtendY},
        {"ImGuiTableFlags_NoKeepColumnsVisible", ImGuiTableFlags_NoKeepColumnsVisible},
        {"ImGuiTableFlags_PreciseWidths", ImGuiTableFlags_PreciseWidths},
        {"ImGuiTableFlags_NoClip", ImGuiTableFlags_NoClip},
        {"ImGuiTableFlags_PadOuterX", ImGuiTableFlags_PadOuterX},
        {"ImGuiTableFlags_NoPadOuterX", ImGuiTableFlags_NoPadOuterX},
        {"ImGuiTableFlags_NoPadInnerX", ImGuiTableFlags_NoPadInnerX},
        {"ImGuiTableFlags_ScrollX", ImGuiTableFlags_ScrollX},
        {"ImGuiTableFlags_ScrollY", ImGuiTableFlags_ScrollY},
        {"ImGuiTableFlags_SortMulti", ImGuiTableFlags_SortMulti},
        {"ImGuiTableFlags_SortTristate", ImGuiTableFlags_SortTristate},
        {"ImGuiTableFlags_HighlightHoveredColumn", ImGuiTableFlags_HighlightHoveredColumn},
        {"ImGuiTableFlags_SizingMask_", ImGuiTableFlags_SizingMask_},
        //enum ImGuiTableColumnFlags_
        {"ImGuiTableColumnFlags_None", ImGuiTableColumnFlags_None},
        {"ImGuiTableColumnFlags_Disabled", ImGuiTableColumnFlags_Disabled},
        {"ImGuiTableColumnFlags_DefaultHide", ImGuiTableColumnFlags_DefaultHide},
        {"ImGuiTableColumnFlags_DefaultSort", ImGuiTableColumnFlags_DefaultSort},
        {"ImGuiTableColumnFlags_WidthStretch", ImGuiTableColumnFlags_WidthStretch},
        {"ImGuiTableColumnFlags_WidthFixed", ImGuiTableColumnFlags_WidthFixed},
        {"ImGuiTableColumnFlags_NoResize", ImGuiTableColumnFlags_NoResize},
        {"ImGuiTableColumnFlags_NoReorder", ImGuiTableColumnFlags_NoReorder},
        {"ImGuiTableColumnFlags_NoHide", ImGuiTableColumnFlags_NoHide},
        {"ImGuiTableColumnFlags_NoClip", ImGuiTableColumnFlags_NoClip},
        {"ImGuiTableColumnFlags_NoSort", ImGuiTableColumnFlags_NoSort},
        {"ImGuiTableColumnFlags_NoSortAscending", ImGuiTableColumnFlags_NoSortAscending},
        {"ImGuiTableColumnFlags_NoSortDescending", ImGuiTableColumnFlags_NoSortDescending},
        {"ImGuiTableColumnFlags_NoHeaderLabel", ImGuiTableColumnFlags_NoHeaderLabel},
        {"ImGuiTableColumnFlags_NoHeaderWidth", ImGuiTableColumnFlags_NoHeaderWidth},
        {"ImGuiTableColumnFlags_PreferSortAscending", ImGuiTableColumnFlags_PreferSortAscending},
        {"ImGuiTableColumnFlags_PreferSortDescending", ImGuiTableColumnFlags_PreferSortDescending},
        {"ImGuiTableColumnFlags_IndentEnable", ImGuiTableColumnFlags_IndentEnable},
        {"ImGuiTableColumnFlags_IndentDisable", ImGuiTableColumnFlags_IndentDisable},
        {"ImGuiTableColumnFlags_AngledHeader", ImGuiTableColumnFlags_AngledHeader},
        {"ImGuiTableColumnFlags_IsEnabled", ImGuiTableColumnFlags_IsEnabled},
        {"ImGuiTableColumnFlags_IsVisible", ImGuiTableColumnFlags_IsVisible},
        {"ImGuiTableColumnFlags_IsSorted", ImGuiTableColumnFlags_IsSorted},
        {"ImGuiTableColumnFlags_IsHovered", ImGuiTableColumnFlags_IsHovered},
        {"ImGuiTableColumnFlags_WidthMask_", ImGuiTableColumnFlags_WidthMask_},
        {"ImGuiTableColumnFlags_IndentMask_", ImGuiTableColumnFlags_IndentMask_},
        {"ImGuiTableColumnFlags_StatusMask_", ImGuiTableColumnFlags_StatusMask_},
        {"ImGuiTableColumnFlags_NoDirectResize_", ImGuiTableColumnFlags_NoDirectResize_},
        //enum ImGuiTableRowFlags_
        {"ImGuiTableRowFlags_None", ImGuiTableRowFlags_None},
        {"ImGuiTableRowFlags_Headers", ImGuiTableRowFlags_Headers}
	};
}


#endif /* XMLREADER_H */
