#ifndef NL_TILE_H
#define NL_TILE_H

#include <string>

#include <nel/misc/types_nl.h>
#include <nel/misc/app_context.h>
#include <nel/misc/debug.h>

#include <nel/misc/i_xml.h>

#include <nel/misc/rgba.h>
#include <nel/misc/vector.h>

#include <nel/3d/u_instance.h>

class Level;

class Tile : public NLMISC::IStreamable {
public:
	Tile() : ParentLevel(NULL) { }
	Tile(Level *parentLevel) : ParentLevel(parentLevel), TileActive(true) { };
	Tile(uint32 ID, std::string TileType, sint32 PosX, sint32 PosY, bool TileActive) : TileActive(TileActive), ID(ID), TileType(TileType), PosX(PosX), PosY(PosY) {};
	Level *ParentLevel;

	/// Whether this tile is actively available to land on (used for weak/fragile tiles).
	bool TileActive;

	/// Unique ID of this tile in the level.
	uint32 ID;
	
	/// Valid types are start, goal, weak, fragile, switch, portal and normal.
	std::string TileType;

	/// Tile X position (tile-space, not real space.)
	sint32 PosX;

	/// Tile Y position (tile-space, not real space.)
	sint32 PosY;

	/// The tile's 3D object instance.
	NL3D::UInstance TileInstance;

	void processXml(xmlNodePtr node);
	void serial(NLMISC::IStream &stream);

	virtual void contact(bool upright) = 0;
	virtual void contactLeft() = 0;
	virtual void load() = 0;
	virtual void unload() = 0;
	virtual void update(double deltaTime) = 0;

};

#endif // NL_TILE_H
