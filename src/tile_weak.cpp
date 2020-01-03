#include "tile_weak.h"

#include "neloidgame.h"
#include "session.h"
#include "xml_helpers.h"

void TileWeak::processXml(xmlNodePtr node) {
	Tile::processXml(node);
}

void TileWeak::serial(NLMISC::IStream &stream) {
	Tile::serial(stream);
}

void TileWeak::load() {
	// Load the 3D Media
	TileInstance = NeloidGame::getInstance().scene().createInstance("tile.shape");
	// Position the tile appropriately.
	TileInstance.setTransformMode(NL3D::UTransform::RotQuat);
	TileInstance.setPos(NLMISC::CVector(float(PosX), float(PosY), -1.2f));
	TileInstance.show();
}

void TileWeak::unload() {
	NeloidGame::getInstance().scene().deleteInstance(TileInstance);
}

void TileWeak::contact(bool upright) {
	Touched = true;
}

void TileWeak::contactLeft() { 
	if(Touched == true) {
		TileActive = false;
		TileInstance.hide();
	}
}

