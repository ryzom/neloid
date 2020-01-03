#ifndef NL_TILE_START_H
#define NL_TILE_START_H

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


class TileStart : public Tile {
public:
	TileStart() : Tile() { };
	TileStart(Level *parentLevel) : Tile(parentLevel) { };
	TileStart(uint32 ID, sint32 PosX, sint32 PosY, bool TileActive) : Tile(ID, "start", PosX, PosY, TileActive) {};
	void processXml(xmlNodePtr node);
	void serial(NLMISC::IStream &stream);
	void contact(bool upright) { };
	void contactLeft() { };
	void load();
	void unload();
	void update(double deltaTime) { };

	NLMISC_DECLARE_CLASS(TileStart);
};

#endif // NL_TILE_START_H
