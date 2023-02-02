#ifndef DEARIMXML_XMLEVENTHANDLER_H
#define DEARIMXML_XMLEVENTHANDLER_H

#include <XMLTree.h>

namespace ImXML {
	class XMLEventHandler
	{
	public:
		XMLEventHandler(/* args */);
		~XMLEventHandler();

		virtual void onNodeBegin(XMLNode& node) = 0;
		virtual void onNodeEnd(XMLNode& node) = 0;
		virtual void onEvent(XMLNode& node) = 0;

	};
	
	XMLEventHandler::XMLEventHandler(/* args */)
	{
	}
	
	XMLEventHandler::~XMLEventHandler()
	{
	}
	
}

#endif /* DEARIMXML_XMLEVENTHANDLER_H */
