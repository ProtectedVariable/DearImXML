#include <XMLReader.h>

namespace ImXML {

	const std::unordered_map<std::string, ImGuiEnum> XMLReader::tagnames = {
		{ "begin", ImGuiEnum::BEGIN },
		{ "button" , ImGuiEnum::BUTTON }
	};

}
