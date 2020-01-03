#include "level.h"

#include <nel/misc/types_nl.h>
#include <nel/misc/file.h>
#include <nel/misc/path.h>
#include <nel/misc/i_xml.h>
#include <nel/misc/rgba.h>
#include <nel/misc/algo.h>

#include "neloidgame.h"
#include "tile.h"
#include "xml_helpers.h"
#include "level_editor.h"

#include "tiles.h"

void LevelCategory::processXml(xmlNodePtr node) {
	Name = GetXmlPropertyString(node, "Category");
	nlinfo("Category: %s", Name.c_str());

	xmlNodePtr levelPtr = NLMISC::CIXml::getFirstChildNode(node, "Level");
	while(levelPtr != NULL) {
		// Handle the level.
		nlinfo("Handling a level.");

		Level *newLevel = new Level(this);
		Levels.push_back(newLevel);
		newLevel->processXml(levelPtr);		

		// Advance to the next one.
		levelPtr = NLMISC::CIXml::getNextChildNode(levelPtr, "Level");
	}
}

void LevelCategory::serial(NLMISC::IStream &stream) {
	
	stream.serial(Name);	
	stream.serialContPolyPtr(Levels);

	if(stream.isReading()) {
		TLevelsList::iterator iter = Levels.begin();
		while(iter != Levels.end()) {
			(*iter)->ParentCategory = this;
			iter++;
		}
	}
}

Level *LevelCategory::getLevelByID(uint32 levelId) {
	Level *foundLevel=NULL;

	TLevelsList::iterator levelsItr = Levels.begin();
	while(levelsItr != Levels.end()) {
		if((*levelsItr)->ID == levelId) {
			foundLevel=(*levelsItr);
			break;
		}	
		levelsItr++;
	}
	return foundLevel;
}

Level *LevelCategoryProgressive::getLevelByID(uint32 levelId) {
	uint32 levelSize = 7 + 3*levelId;
	Level *level = LevelEditor::getInstance().generateLevel(levelSize);
	level->ID = levelId;
	level->ParentCategory = dynamic_cast<LevelCategory*>(this);
	return level;
}

void Level::processXml(xmlNodePtr node) {
	ID = GetXmlPropertyUInt(node, "Number");
	Name = GetXmlPropertyString(node, "Name");
	Background = GetXmlPropertyString(node, "Background");
	BackgroundMusic = GetXmlPropertyString(node, "BackgroundMusic");

	AmbientLightColor = GetXmlPropertyColor(node, "AmbientLightColor");
	DirectionalLightColor = GetXmlPropertyColor(node, "DirectionalLightColor");
	DirectionalLightPos = GetXmlPropertyPosition(node, "DirectionalLightPos");

	xmlNodePtr tilePtr = NLMISC::CIXml::getFirstChildNode(node, "Tile");
	while(tilePtr != NULL) {
		// Handle the level.
		nlinfo("Handling a tile.");

		std::string tileType = GetXmlPropertyString(tilePtr, "Type");
		Tile *newTile = NULL;
		if(tileType == "start") {
			newTile = new TileStart(this);
		} else if (tileType == "portal") {
			newTile = new TilePortal(this);
		} else if (tileType == "weak") {
			newTile = new TileWeak(this);
		} else if (tileType == "goal") {
			newTile = new TileGoal(this);
		} else if (tileType == "normal") {
			newTile = new TileNormal(this);
		} else {
			throw NLMISC::Exception("Unknown tile type specified: '"+tileType+"'");
		}

		newTile->processXml(tilePtr);
		// TODO mattr: add a check here - the tile ID should be unique and goal/start should only have one tile.

		addTile(newTile);

		// Advance to the next one.
		tilePtr = NLMISC::CIXml::getNextChildNode(tilePtr, "Tile");
	}
}

void Level::serial(NLMISC::IStream &stream) {
	stream.serial(ID);
	stream.serial(Name);
	stream.serial(Background);
	stream.serial(BackgroundMusic);
	stream.serial(AmbientLightColor);
	stream.serial(DirectionalLightColor);
	stream.serial(DirectionalLightPos);
	stream.serialContPolyPtr(Tiles);
}

void Level::load() {
	nlinfo("Loading level '%s'", Name.c_str());

	// Load all of the tiles.
	TTilesList::iterator itr = Tiles.begin();
	while(itr != Tiles.end()) {
		(*itr)->load();
		itr++;
	}

	NeloidGame::getInstance().scene().setSunAmbient(AmbientLightColor);
}

void Level::unload() {
	nlinfo("Unloading level '%s'", Name.c_str());

	// Load all of the tiles.
	TTilesList::iterator itr = Tiles.begin();
	while(itr != Tiles.end()) {
		(*itr)->unload();
		itr++;
	}
}

Tile &Level::getStartTile() {
	Tile *startTile;
	bool foundStartTile=false;

	TTilesList::iterator itr = Tiles.begin();
	while(itr != Tiles.end()) {
		startTile = (*itr);
		if(startTile->TileType == "start") {
			foundStartTile=true;		
			break;
		}
		itr++;
	}

	if(!foundStartTile)
		throw NLMISC::Exception("Unable to find start tile for level '%s'", Name.c_str());

	return *startTile;
}

Tile &Level::getTileByCoords(sint32 x, sint32 y) {
	Tile *tile;
	bool foundTile=false;

	TTilesList::iterator itr = Tiles.begin();
	while(itr != Tiles.end()) {
		tile = (*itr);
		if(tile->PosX == x && tile->PosY == y) {
			foundTile=true;		
			break;
		}
		itr++;
	}

	if(!foundTile)
		throw NLMISC::Exception("Unable to find tile at %d,%d for level '%s'", x, y, Name.c_str());

	return *tile;
}

bool Level::isTileByCoords(sint32 x, sint32 y) const {
	TTilesList::const_iterator itr = Tiles.begin();
	while(itr != Tiles.end()) {
		if((*itr)->PosX == x && (*itr)->PosY == y)
			return true;
		itr++;
	}
	return false;
}

Tile &Level::getTileById(uint32 id) {
	Tile *tile;
	bool foundTile=false;

	TTilesList::iterator itr = Tiles.begin();
	while(itr != Tiles.end()) {
		tile = (*itr);
		if(tile->ID == id) {
			foundTile=true;		
			break;
		}
		itr++;
	}

	if(!foundTile)
		throw NLMISC::Exception("Unable to find tile ID %d for level '%s'", id, Name.c_str());

	return *tile;
}

bool Level::isTileById(uint32 id) {
	TTilesList::iterator itr = Tiles.begin();
	while(itr != Tiles.end()) {
		if((*itr)->ID == id)
			return true;
		itr++;
	}
	return false;
}
	
void Level::update(double deltaTime) {
	TTilesList::iterator itr = Tiles.begin();
	while(itr != Tiles.end()) {
		(*itr)->update(deltaTime);
		
		itr++;
	}
}

bool Level::addTile(Tile *tile) {
	if (!isTileByCoords(tile->PosX, tile->PosY)) {
		Tiles.push_back(tile);
		return true;
	}

	delete tile;
	return false;
}
