#include <XMLReader.h>

namespace DearImXML {

	const std::unordered_map<std::string, ImGuiEnum> XMLReader::tagnames = {
		{ "begin", ImGuiEnum::BEGIN },
		{ "button" , ImGuiEnum::BUTTON }
	};

}
