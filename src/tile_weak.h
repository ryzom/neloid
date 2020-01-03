#ifndef NL_TILE_WEAK_H
#define NL_TILE_WEAK_H

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

class TileWeak : public Tile {
public:
	TileWeak() : Tile() { };
	TileWeak(Level *parentLevel) : Tile(parentLevel), Touched(false) { };
	void processXml(xmlNodePtr node);
	void serial(NLMISC::IStream &stream);
	void contact(bool upright);
	void contactLeft();
	void load();
	void unload();
	void update(double deltaTime) { };

	bool Touched;

	NLMISC_DECLARE_CLASS(TileWeak);
};

#endif // NL_TILE_WEAK_H
