#ifndef NL_TILE_PORTAL_H
#define NL_TILE_PORTAL_H

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

class TilePortal : public Tile {
public:
	TilePortal() : Tile() { };
	TilePortal(Level *parentLevel) : Tile(parentLevel) { };
	void processXml(xmlNodePtr node);
	void serial(NLMISC::IStream &stream);
	void contact(bool upright);
	void contactLeft() { };
	void load();
	void unload();
	void update(double deltaTime) { };
	/// If the type is a portal we need the destination ID.
	uint32 PortalID;

	NLMISC_DECLARE_CLASS(TilePortal);
};

#endif // NL_TILE_PORTAL_H
