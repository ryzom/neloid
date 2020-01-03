#include "tile.h"

#include "neloidgame.h"
#include "session.h"
#include "xml_helpers.h"

void Tile::processXml(xmlNodePtr node) {
	ID = GetXmlPropertyUInt(node, "ID");
	
	TileType = GetXmlPropertyString(node, "Type");
	PosX = GetXmlPropertySInt(node, "PosX");
	PosY = GetXmlPropertySInt(node, "PosY");
}

void Tile::serial(NLMISC::IStream &stream) {
	stream.serial(ID);
	stream.serial(TileType);
	stream.serial(PosX);
	stream.serial(PosY);
}
