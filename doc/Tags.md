DearImXML — XML tags → ImGui mapping
================================================================

Layout and Formatting 
----------------------------------------------------------------
| Tag name | Required Arguments | Optional Arguments | ImGui Call / Behavior |
|---|---|---|---|
| Sameline | (none) | (none) | `ImGui::Sameline();` |
| Separator | (none) | (none) | `ImGui::Separator();` |
| SeparatorText | label (string) | (none) | `ImGui::SeparatorText(<label>);` |
| SetupColumn |  label (string) | flags (ImGuiTableColumnFlags_),  width (float) | `ImGui::TableSetupColumn(<label>, <flags>, <width>);` |
| Header |  (none) | (none) | `ImGui::TableHeadersRow();` |
| Row |  (none) | flags (ImGuiTableRowFlags_) | `ImGui::TableNextRow(<flags>);` |
| Column | (none) | (none) | `ImGui::TableNextColumn();` |
| Group | (none) | (none) | `ImGui::BeginGroup();` |
| Text | label (none) | (none) | `ImGui::Text("%s", <label>);` |
| Placeholder | (none) | (none) | Nothing, can be used to then call any piece of code using `onNodeBegin()`  |


Containers
----------------------------------------------------------------
| Tag name | Required Arguments | Optional Arguments | ImGui Call / Behavior |
|---|---|---|---|
| BeginPopupModal | name (string) | flags (ImGuiPopupFlags_) | `ImGui::BeginPopupModal(<name>, nullptr, <flags>);` |
| Table | name (string), columns (int) | flags (ImGuiTableFlags_) | `ImGui::BeginTable(<name>, <columns>, <flag>, <size>);` |
| Menubar | (none) | (none) | `ImGui::BeginMenuBar();` |
| MainMenubar | (none) | (none) | `ImGui::BeginMainMenuBar();` |
| Menu | label (string) | (none) | `ImGui::BeginMenu(<label>);` |
| Begin | name (string) | flags (ImGuiWindowFlags_) | `ImGui::Begin(<name>, nullptr, <flags>);` |
| Child | label (string) | flags (ImGuiWindowFlags_), childflags (ImGuiChildFlags_) | `ImGui::BeginChild(<label>, <size>, <childflags>, <flags>);` |
| Combo | label (string) | preview_value (string), flags (ImGuiComboFlags_) | `ImGui::BeginCombo(<label>, <preview_value>, <flags>);` |
| TreeNode | label (string) | flags (ImGuiTreeNodeFlags_) | `ImGui::TreeNodeEx(<label>, <flags>);` |
| TabBar | id (string) | flags (ImGuiTabBarFlags_) | `ImGui::BeginTabBar(<id>, <flags>);` |
| TabItem | label (string) | flags (ImGuiTabBarFlags_) | `ImGui::BeginTabItem(<label>, nullptr, <flags>);` |


Interactive Widgets
----------------------------------------------------------------
| Tag name | Required Arguments | Optional Arguments | ImGui Call |
|---|---|---|---|
| MenuItem | label (string) | shortcut (string) | `ImGui::MenuItem(<label>, <shortcut>) `|
| colorpicker3 | label (string), dynamic (string) | flags (ImGuiColorEditFlags_) | `ImGui::ColorPicker3(<label>, (float*)(<dynamic>).ptr, <flags>)`|
| colorpicker4 | label (string), dynamic (string) | flags (ImGuiColorEditFlags_) | `ImGui::ColorPicker4(<label>, (float*)(<dynamic>).ptr, <flags>)`|
| coloredit3 | label (string), dynamic (string) | flags (ImGuiColorEditFlags_) | `ImGui::ColorEdit3(<label>, (float*)(<dynamic>).ptr, <flags>)` |
| coloredit4 | label (string), dynamic (string) | flags (ImGuiColorEditFlags_) | `ImGui::ColorEdit4(<label>, (float*)(<dynamic>).ptr, <flags>)` |
| button | label (string) | (none) | `ImGui::Button(<label>, <size>)` |
| sliderfloat | label (string), dynamic (string) | min (float), max (float) | `ImGui::SliderFloat(<label>, (float*)(<dynamic>).ptr, min, max)` |
| inputfloat | label (string), dynamic (string) | step (float), step_fast (float), format (string), flags (ImGuiInputTextFlags_) | `ImGui::InputFloat(<label>, (float*) (<dynamic>).ptr, <step>, <step_fast>, <format>, <flags>);` |
| checkbox | label (string), dynamic (string) | (none) | `ImGui::Checkbox(<label>, (float*)(<dynamic>).ptr);` |
| selectable | label (string) | selected (bool), flags (ImGuiSelectableFlags_) | `ImGui::Selectable(<label>, <selected>, <flags>, <size>);` |
| inputtext | label (string), dynamic (string) | hint (string), flags (ImGuiInputTextFlags_) | `ImGui::InputText(<label>, (char*)(<dynamic>).ptr, <dynamic>.size, <flags>)` or `ImGui::InputTextWithHint(<label>, <hint>, (char*)(<dynamic>).ptr, <dynamic>.size, <flags>)` |
| inputtextmultiline | label (string), dynamic (string) | flags (ImGuiInputTextFlags_) | `ImGui::InputTextMultiline(<label>, (char*)(<dynamic>).ptr, <dynamic>.size, <size>, <flags>)` |

Flags and special attributes
----------------------------------------------------------------
- flags attribute: used on some tags (notably <begin flags="..."> and <inputtext flags="...">). The reader parses a flags string with tokens separated by '|' or ',' (e.g. flags="ImGuiWindowFlags_MenuBar|ImGuiWindowFlags_NoCollapse") and converts each token via a flagnames map into the integer flag bitmask which is passed to ImGui calls where supported.
- dynamic attribute: a string key matching a dynamic bind previously registered via XMLRenderer::addDynamicBind (the example code registers "float0", "str0" etc.). The renderer uses dynamicBinds.at(name).ptr and dynamicBinds.at(name).size for reading/writing widget state.
  - For SliderFloat/ColorPicker/ColorEdit you must bind to float arrays of the appropriate size.
  - For InputText you must bind to a char buffer with a size provided in the dynamic bind.
