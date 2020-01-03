#include "tile_portal.h"

#include "neloidgame.h"
#include "session.h"
#include "xml_helpers.h"

void TilePortal::processXml(xmlNodePtr node) {
	Tile::processXml(node);

	if(TileType == "portal")
		PortalID = GetXmlPropertyUInt(node, "PortalID");
}

void TilePortal::serial(NLMISC::IStream &stream) {
	Tile::serial(stream);
	stream.serial(PortalID);
}

void TilePortal::load() {
	// Load the 3D Media
	TileInstance = NeloidGame::getInstance().scene().createInstance("tile.shape");
	// Position the tile appropriately.
	TileInstance.setTransformMode(NL3D::UTransform::RotQuat);
	TileInstance.setPos(NLMISC::CVector(float(PosX), float(PosY), -1.2f));
}

void TilePortal::unload() {
	NeloidGame::getInstance().scene().deleteInstance(TileInstance);
}

void TilePortal::contact(bool upright) {
	nlinfo("ZZZZZAP, being moved to %d", PortalID);
	NeloidGame::getInstance().requestLevelPortal(PortalID);
}
