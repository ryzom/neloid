#include "tile_normal.h"

#include "neloidgame.h"
#include "session.h"
#include "xml_helpers.h"

void TileNormal::processXml(xmlNodePtr node) {
	Tile::processXml(node);
}

void TileNormal::load() {
	// Load the 3D Media
	TileInstance = NeloidGame::getInstance().scene().createInstance("tile.shape");
	// Position the tile appropriately.
	TileInstance.setTransformMode(NL3D::UTransform::RotQuat);
	TileInstance.setPos(NLMISC::CVector(float(PosX), float(PosY), -1.2f));
}

void TileNormal::unload() {
	TileInstance.hide();
	NeloidGame::getInstance().scene().deleteInstance(TileInstance);
}

void TileNormal::serial(NLMISC::IStream &stream) {
	Tile::serial(stream);
}
