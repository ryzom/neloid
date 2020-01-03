#ifndef NL_LEVEL_EDITOR_H
#define NL_LEVEL_EDITOR_H

#include <string>

#include "CEGUI.h"

#include <nel/misc/types_nl.h>
#include <nel/misc/app_context.h>
#include <nel/misc/debug.h>
#include <nel/misc/dynloadlib.h>
#include <nel/misc/events.h>
#include <nel/misc/event_listener.h>
#include <nel/misc/plane.h>

class Tile;
class Level;

class LevelEditorMouseListener : public NLMISC::IEventListener {
	void operator ()(const NLMISC::CEvent& event);

public:
	LevelEditorMouseListener();

	void registerWithServer(NLMISC::CEventServer &server);

	void unregisterWithServer(NLMISC::CEventServer &server);
	
	NLMISC::CVector getMousePosition() { return m_3dMousePosition; }
	NLMISC::CVector getTilePosition() { return m_tilePos; }
private:
	NLMISC::CVector m_tilePos;

	NLMISC::CVector m_3dMousePosition;

	float m_cursorWidth;
	float m_cursorHeight;
};

class LevelEditor {
	LevelEditor() { };
	~LevelEditor();

	NLMISC_SAFE_SINGLETON_DECL(LevelEditor);

public:
	void init();
	void update();

	/// Prints debug text on the screen.
	void renderText();

	/// Change to the next selected tile.
	void selectNext();

	/// Change to the previous selected tile.
	void selectPrev();

	Level *generateLevel(uint16 steps);

	void setActive(bool active) { m_editorActive = active; }
	bool getActive() { return m_editorActive; }

private:
	typedef std::vector<Tile *> TSelectionList;
	/// Contains selectable tiles.
	TSelectionList m_selectionList;

	/// Holds the index to the currently selected tile.
	uint32 m_selectedTile;

	/// Determines if we're in editor mode thus whether to show the editor tile.
	bool m_editorActive;

	/// Determines whether to run the tile updates.
	bool m_levelLoaded;

	void setupTile(Tile *tile);
	sint32 m_tileX, m_tileY;
	LevelEditorMouseListener *m_levelEdMouseListener;

	Tile	*m_currentTile;
};

#endif // NL_LEVEL_EDITOR_H