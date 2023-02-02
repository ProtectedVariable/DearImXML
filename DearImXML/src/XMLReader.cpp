#include <XMLReader.h>

namespace ImXML {

	const std::unordered_map<std::string, ImGuiEnum> XMLReader::tagnames = {
		{ "begin", ImGuiEnum::BEGIN },
		{ "button" , ImGuiEnum::BUTTON },
		{ "placeholder", ImGuiEnum::PLACEHOLDER },
		{ "text", ImGuiEnum::TEXT },
		{ "sameline", ImGuiEnum::SAMELINE },
	};

}
