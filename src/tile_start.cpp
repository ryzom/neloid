#include "tile_start.h"

#include "neloidgame.h"
#include "session.h"
#include "xml_helpers.h"

void TileStart::processXml(xmlNodePtr node) {
	Tile::processXml(node);
}

void TileStart::serial(NLMISC::IStream &stream) {
	Tile::serial(stream);
}

void TileStart::load() {
	// Load the 3D Media
	TileInstance = NeloidGame::getInstance().scene().createInstance("tile_start.shape");
	// Position the tile appropriately.
	TileInstance.setTransformMode(NL3D::UTransform::RotQuat);
	TileInstance.setPos(NLMISC::CVector(float(PosX), float(PosY), -1.2f));
}

void TileStart::unload() {
	TileInstance.hide();
	NeloidGame::getInstance().scene().deleteInstance(TileInstance);
}
