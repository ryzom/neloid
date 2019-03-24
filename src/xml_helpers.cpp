#include "xml_helpers.h"

#include <string>

#include <nel/misc/types_nl.h>
#include <nel/misc/debug.h>
#include <nel/misc/algo.h>

#include <nel/misc/i_xml.h>

#include <nel/misc/rgba.h>
#include <nel/misc/vector.h>

std::string GetXmlPropertyString(xmlNodePtr node, std::string propName) {
	bool res;
	std::string propVal;

	res=NLMISC::CIXml::getPropertyString(propVal, node, propName.c_str());
	if(!res)
		throw NLMISC::Exception("Unable to load property '"+propName+"'");

	return propVal;
}

uint32 GetXmlPropertyUInt(xmlNodePtr node, std::string propName) {
	sint64 propVal;

	propVal=NLMISC::CIXml::getIntProperty(node, propName.c_str(), -1);
	
	if(propVal < 0)
		throw NLMISC::Exception("Unable to load property '"+propName+"'");

	return (uint32)propVal;
}

sint32 GetXmlPropertySInt(xmlNodePtr node, std::string propName) {
	sint32 propVal;

	propVal=NLMISC::CIXml::getIntProperty(node, propName.c_str(), 0);

	return propVal;
}

NLMISC::CRGBA GetXmlPropertyColor(xmlNodePtr node, std::string propName) {
	bool res;
	std::string propVal;
	NLMISC::CRGBA color;

	// Retrieve the directional light position.
	res = NLMISC::CIXml::getPropertyString(propVal, node, propName.c_str());
	if(!res)
		throw NLMISC::Exception("Unable to load '"+propName+"'");
	
	// Split the string up (input as 0,0,0)
	std::vector<std::string> colors;
	NLMISC::splitString(propVal, ",", colors);
	if(colors.size() < 3)
		throw NLMISC::Exception("Not enough color attributes specified for '"+propName+"': '"+propVal+"'");
	
	// Parse split string into the directional color variable.
	NLMISC::fromString(colors[0], color.R);
	NLMISC::fromString(colors[1], color.G);
	NLMISC::fromString(colors[2], color.B);
	
	if(colors.size() == 4) // Provided an alpha value.
		NLMISC::fromString(colors[3], color.A);
	else
		color.A = 255;

	return color;
}

NLMISC::CVector GetXmlPropertyPosition(xmlNodePtr node, std::string propName) {
	bool res;
	std::string propVal;
	NLMISC::CVector position;

	// Retrieve the position.
	res = NLMISC::CIXml::getPropertyString(propVal, node, propName.c_str());
	if(!res)
		throw NLMISC::Exception("Unable to load '"+propName+"'");

	
	// Split the string up (input as 0,0,0)
	std::vector<std::string> pos;
	NLMISC::splitString(propVal, ",", pos);
	if(pos.size() != 3)
		throw NLMISC::Exception("Not enough coordinate attributes specified for '"+propName+"': '"+propVal+"'");

	// Parse split string into the directional color variable.
	NLMISC::fromString(pos[0], position.x);
	NLMISC::fromString(pos[1], position.y);
	NLMISC::fromString(pos[2], position.z);

	return position;
}
