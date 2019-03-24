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

void TileStart::processXml(xmlNodePtr node) {
	Tile::processXml(node);
}

void TileGoal::processXml(xmlNodePtr node) {
	Tile::processXml(node);
}

void TileNormal::processXml(xmlNodePtr node) {
	Tile::processXml(node);
}

void TileWeak::processXml(xmlNodePtr node) {
	Tile::processXml(node);
}

void TilePortal::processXml(xmlNodePtr node) {
	Tile::processXml(node);

	if(TileType == "portal")
		PortalID = GetXmlPropertyUInt(node, "PortalID");
}

void TileStart::load() {
	// Load the 3D Media
	TileInstance = NeloidGame::getInstance().scene().createInstance("tile_start.shape");
	// Position the tile appropriately.
	TileInstance.setTransformMode(NL3D::UTransform::RotQuat);
	TileInstance.setPos(NLMISC::CVector(float(PosX), float(PosY), -1.2f));
}

void TileStart::unload() {
	NeloidGame::getInstance().scene().deleteInstance(TileInstance);
}

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

void TileNormal::load() {
	// Load the 3D Media
	TileInstance = NeloidGame::getInstance().scene().createInstance("tile.shape");
	// Position the tile appropriately.
	TileInstance.setTransformMode(NL3D::UTransform::RotQuat);
	TileInstance.setPos(NLMISC::CVector(float(PosX), float(PosY), -1.2f));
}

void TileNormal::unload() {
	NeloidGame::getInstance().scene().deleteInstance(TileInstance);
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

void TileWeak::contact(bool upright) {
	Touched = true;
}

void TileWeak::contactLeft() { 
	if(Touched == true) {
		TileActive = false;
		TileInstance.hide();
	}
}

void TilePortal::contact(bool upright) {
	nlinfo("ZZZZZAP, being moved to %d", PortalID);
	NeloidGame::getInstance().requestLevelPortal(PortalID);
}
