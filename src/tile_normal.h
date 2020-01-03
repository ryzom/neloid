#ifndef NL_TILE_NORMAL_H
#define NL_TILE_NORMAL_H

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

class TileNormal : public Tile {
public:
	TileNormal() : Tile() { };
	TileNormal(Level *parentLevel) : Tile(parentLevel) { };
	TileNormal(uint32 ID, sint32 PosX, sint32 PosY, bool TileActive) : Tile(ID, "normal", PosX, PosY, TileActive) {};
	void processXml(xmlNodePtr node);
	void serial(NLMISC::IStream &stream);
	void contact(bool upright) { };
	void contactLeft() { };
	void load();
	void unload();
	void update(double deltaTime) { };

	NLMISC_DECLARE_CLASS(TileNormal);
};

#endif // NL_TILE_NORMAL_H
