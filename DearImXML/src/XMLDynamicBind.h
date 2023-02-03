#ifndef DEARIMXML_XMLDYNAMICBIND_H
#define DEARIMXML_XMLDYNAMICBIND_H

namespace ImXML {

	enum XMLDynamicBindType {
		Float, Int, Chars,
	};

	struct XMLDynamicBind
	{
		void* ptr;
		unsigned int size;
		XMLDynamicBindType type;
	};
	
	
}

#endif /* DEARIMXML_XMLDYNAMICBIND_H */
