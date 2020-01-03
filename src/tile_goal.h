#ifndef NL_TILE_GOAL_H
#define NL_TILE_GOAL_H

#include <string>

#include <nel/misc/types_nl.h>
#include <nel/misc/app_context.h>
#include <nel/misc/debug.h>

#include <nel/misc/i_xml.h>

#include <nel/misc/rgba.h>
#include <nel/misc/vector.h>

#include <nel/3d/u_instance.h>

#include "tile.h"

class Level;

class TileGoal : public Tile {
public:
	TileGoal() : Tile() { }
	TileGoal(Level *parentLevel) : Tile(parentLevel) { };
	TileGoal(uint32 ID, sint32 PosX, sint32 PosY, bool TileActive) : Tile(ID, "goal", PosX, PosY, TileActive) {};
	void processXml(xmlNodePtr node);
	void contact(bool upright);
	void contactLeft() { };
	void load();
	void unload();
	void update(double deltaTime) { };

	void serial(NLMISC::IStream &stream);

	NLMISC_DECLARE_CLASS(TileGoal);
};

#endif // NL_TILE_GOAL_H
