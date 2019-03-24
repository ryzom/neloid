#ifndef NL_XML_HELPERS_H
#define NL_XML_HELPERS_H

#include <string>


#include <nel/misc/types_nl.h>

#include <nel/misc/i_xml.h>

#include <nel/misc/rgba.h>
#include <nel/misc/vector.h>


std::string GetXmlPropertyString(xmlNodePtr node, std::string propName);

uint32 GetXmlPropertyUInt(xmlNodePtr node, std::string propName);

sint32 GetXmlPropertySInt(xmlNodePtr node, std::string propName);

NLMISC::CRGBA GetXmlPropertyColor(xmlNodePtr node, std::string propName);

NLMISC::CVector GetXmlPropertyPosition(xmlNodePtr node, std::string propName);

#endif // NL_XML_HELPERS_H