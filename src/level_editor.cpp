#include <math.h>

#include <nel/misc/random.h>
#include <nel/misc/time_nl.h>

#include <nel/3d/material.h>

#include "level_editor.h"

#include "neloidgame.h"
#include "level_manager.h"
#include "tiles.h"
#include "level.h"
#include "CMove.h"

NLMISC_SAFE_SINGLETON_IMPL(LevelEditor)

LevelEditor::~LevelEditor() { 
	m_levelEdMouseListener->unregisterWithServer(NeloidGame::getInstance().driver().EventServer);
	delete m_levelEdMouseListener;
};

void LevelEditor::init() {
	nlinfo("Loading Level Editor");
	// Retrieve the width and height of the window in order to initialize the mouse listener.
	//float width = NeloidGame::getInstance().driver().getWindowWidth();
	//float height = NeloidGame::getInstance().driver().getWindowHeight();

	// Reset the tile location to 0,0
	m_tileX=0;
	m_tileY=0;

	// Create the mouse listener and assign it to the event server.
	m_levelEdMouseListener = new LevelEditorMouseListener();
	m_levelEdMouseListener->registerWithServer(NeloidGame::getInstance().driver().EventServer);

	m_selectedTile = 0;
	m_currentTile = new TileNormal(NULL); 
	setupTile(m_currentTile);

	setupTile(new TileStart(NULL));
	setupTile(new TilePortal(NULL));
	setupTile(new TileWeak(NULL));
	setupTile(new TileGoal(NULL));

	m_editorActive = false;
	m_levelLoaded = false;
}

void LevelEditor::setupTile(Tile *tile) {
	// Load the tile's 3D data
	tile->load();

	// Modify its material so that is is partially transparent.
	/*tile->TileInstance.getMaterial(0).setColor(NLMISC::CRGBA(0, 0, 0, 192));
	tile->TileInstance.getMaterial(0).setBlend(true);
	tile->TileInstance.getMaterial(0).setBlendFunc(NL3D::UInstanceMaterial::srcalpha, NL3D::UInstanceMaterial::invsrcalpha);
	tile->TileInstance.getMaterial(0).setZWrite(false);
	tile->TileInstance.getMaterial(0).setZFunc(NL3D::UInstanceMaterial::always);
	tile->TileInstance.getMaterial(0).getObjectPtr()->setDoubleSided(false);*/

	// And hide it, we'll show it when we need it.
	tile->TileInstance.hide();

	// Add it to the list of selectable tiles
	m_selectionList.push_back(tile);
}

void LevelEditor::update() {

	// Anything beyond here requires that we load a level to edit.
	if(!m_levelLoaded)
		return;

	if(LevelManager::getInstance().getCurrentLevel().isTileByCoords(m_levelEdMouseListener->getTilePosition().x, m_levelEdMouseListener->getTilePosition().y)) {
		// Overlay the existing tile by only dropping -1 (1/2 the upright cube.)
		m_currentTile->TileInstance.setPos(NLMISC::CVector(m_levelEdMouseListener->getTilePosition().x, m_levelEdMouseListener->getTilePosition().y, -1.0f));
		m_currentTile->TileInstance.show();
	} else {
		// Position a tile normally.
		m_currentTile->TileInstance.setPos(NLMISC::CVector(m_levelEdMouseListener->getTilePosition().x, m_levelEdMouseListener->getTilePosition().y, -1.2f));
		m_currentTile->TileInstance.show();
	}
}

void LevelEditor::renderText() {
	std::string editor_loc = "Cursor loc: "+
		NLMISC::toString("%f,%f,%f",
			m_levelEdMouseListener->getMousePosition().x,
			m_levelEdMouseListener->getMousePosition().y,
			m_levelEdMouseListener->getMousePosition().z) + 
		" Tile Pos: "+
		NLMISC::toString("%f,%f", 
			m_levelEdMouseListener->getTilePosition().x,
			m_levelEdMouseListener->getTilePosition().y) +
		" Nbr Tiles: " +
		NLMISC::toString(m_selectionList.size()) +
		" Selected Tile: "+
		NLMISC::toString(m_selectedTile);
	NeloidGame::getInstance().textContext().setHotSpot(NL3D::UTextContext::TopLeft);
	NeloidGame::getInstance().textContext().setColor(NLMISC::CRGBA(255, 255, 255, 255));
	NeloidGame::getInstance().textContext().setFontSize(14);
	NeloidGame::getInstance().textContext().printfAt(0.01f, 0.05f, editor_loc.c_str());
}

void LevelEditor::selectNext() {
	// Wrap the selection back to the top.
	if( (m_selectedTile+1) >= m_selectionList.size())
		m_selectedTile=0;
	else
		m_selectedTile++;

	// Disable the current tile.
	m_currentTile->TileInstance.hide();

	// Retrieve the new tile.
	m_currentTile = m_selectionList[m_selectedTile];

	// Move and show the new tile.
	update();
}

void LevelEditor::selectPrev() {
	// Wrap the selection back to the bottom (last).
	if(m_selectedTile == 0)
		m_selectedTile=m_selectionList.size()-1;
	else
		m_selectedTile--;

	// Disable the current tile.
	m_currentTile->TileInstance.hide();

	// Retrieve the new tile.
	m_currentTile = m_selectionList[m_selectedTile];

	// Move and show the new tile.
	update();
}

Level *LevelEditor::generateLevel(uint16 steps) {
	if(steps < 2)
		steps = 2;

	Level *level = new Level(NULL);

	Tile *StartTile = new TileStart(1, 0, 0, true);
	StartTile->load ();
	level->addTile(StartTile);
	CState state(*StartTile);

	uint32 tileCounter=2, stepCount = 0;
	NLMISC::CRandom moveRandom;
	moveRandom.srand(NLMISC::CTime::getSecondsSince1970());
	CMove moves[4] = {CMove::xpos, CMove::xneg, CMove::ypos, CMove::yneg};
	sint32 lastMoveID = 0;

	bool placeGoal = false;
	Tile *tile;
	do {
		sint32 moveID = moveRandom.rand(3);
		if(moves[moveID].isReverse(moves[lastMoveID]))
			moveID = (moveID + 1) % 4;

		lastMoveID = moveID;

		state *= moves[lastMoveID];
		if(state.x1 != state.x2 || state.y2 != state.y1) {
			if (!level->isTileByCoords(state.x1, state.y1)) {
				tile = new TileNormal(tileCounter++, state.x1, state.y1, true);
				tile->load();
				level->addTile(tile);
			}

			if (!level->isTileByCoords(state.x2, state.y2)) {
				tile = new TileNormal(tileCounter++, state.x2, state.y2, true);
				tile->load();
				level->addTile(tile);
			}
		} else {
			if (!level->isTileByCoords(state.x1, state.y1)) {
				if (stepCount>steps) {
					tile = new TileGoal(tileCounter++, state.x1, state.y1, true);
					tile->load();
					level->addTile(tile);
					placeGoal=true;
				} else {
					tile = new TileNormal(tileCounter++, state.x1, state.y1, true);
					tile->load();
					level->addTile(tile);
				}
			}
		}
		stepCount++;
	} while (!placeGoal);

	return level;
}


LevelEditorMouseListener::LevelEditorMouseListener() : 
		m_tilePos(NLMISC::CVector::Null), 
		m_3dMousePosition(NLMISC::CVector::Null) {

}

void LevelEditorMouseListener::operator()(const NLMISC::CEvent& event) {
	// Handle mouse-moves.
	if(event==NLMISC::EventMouseMoveId) {
		NLMISC::CEventMouse *mouseEvent=(NLMISC::CEventMouse *)&event;

		// Create a normal plane (z)
		NLMISC::CPlane plane;
		plane.make(NLMISC::CVector::K, NLMISC::CVector::Null);

		// Retrieve a ray from the camera at the mouse position
		NLMISC::CVector pos, dir;
		NeloidGame::getInstance().driver().getViewport().getRayWithPoint(
			mouseEvent->X, mouseEvent->Y, pos, dir, 
			NeloidGame::getInstance().camera().getMatrix(),
			NeloidGame::getInstance().camera().getFrustum());
		// Get the point clicked on.
		m_3dMousePosition = plane.intersect(pos, pos+dir);

		m_tilePos.x = floor(m_3dMousePosition.x);
		m_tilePos.y = floor(m_3dMousePosition.y);
	}

	// Handle the mouse wheel, changes selected tile.
	if(event==NLMISC::EventMouseWheelId) {
		NLMISC::CEventMouseWheel* mouseEvent=(NLMISC::CEventMouseWheel*)&event;

		// Wheel scrolled forward.
		if(mouseEvent->Direction)
			LevelEditor::getInstance().selectNext();
		else
			LevelEditor::getInstance().selectPrev();
	}
}

void LevelEditorMouseListener::registerWithServer(NLMISC::CEventServer &server) {
	server.addListener(NLMISC::EventMouseMoveId, this);
	server.addListener(NLMISC::EventMouseDownId, this);
	server.addListener(NLMISC::EventMouseUpId, this);
	server.addListener(NLMISC::EventMouseWheelId, this);
}

void LevelEditorMouseListener::unregisterWithServer(NLMISC::CEventServer &server) {
	server.removeListener(NLMISC::EventMouseMoveId, this);
	server.removeListener(NLMISC::EventMouseDownId, this);
	server.removeListener(NLMISC::EventMouseUpId, this);
	server.removeListener(NLMISC::EventMouseWheelId, this);
 }