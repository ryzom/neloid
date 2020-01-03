#include "tile_goal.h"

#include "neloidgame.h"
#include "session.h"
#include "xml_helpers.h"

void TileGoal::load() {
	// Load the 3D Media
	TileInstance = NeloidGame::getInstance().scene().createInstance("tile_goal.shape");
	// Position the tile appropriately.
	TileInstance.setTransformMode(NL3D::UTransform::RotQuat);
	TileInstance.setPos(NLMISC::CVector(float(PosX), float(PosY), -1.2f));
}

void TileGoal::unload() {
	NeloidGame::getInstance().scene().deleteInstance(TileInstance);
}

void TileGoal::contact(bool upright) {
	if(upright) {
		nlinfo("OH YES UR WINNAR");
		// Finalize this level.
		SessionManager::getInstance().getCurrentSession().finalizePlay();
		// And save the session out to a file.
		SessionManager::getInstance().save();
		NeloidGame::getInstance().requestLevelEnd();
	}
}

void TileGoal::serial(NLMISC::IStream &stream) {
	Tile::serial(stream);
}
