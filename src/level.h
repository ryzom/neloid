#ifndef NL_LEVEL_H
#define NL_LEVEL_H

#include <string>

#include <CEGUI.h>

#include <nel/misc/types_nl.h>
#include <nel/misc/app_context.h>
#include <nel/misc/debug.h>
#include <nel/misc/dynloadlib.h>

#include <nel/misc/i_xml.h>

#include <nel/misc/rgba.h>
#include <nel/misc/vector.h>

#include <nel/3d/u_instance.h>

/**
 * Level XML Format
<Level Name="SomeDescriptiveName" Category="SomeCategory" Number="1">
	<Tile ID="0" Type="<tile|weak|fragile|switch|start|goal>" PosX="0" PosY="0"/>
</Level>
 */

class Level;
class Tile;

class LevelCategory : public NLMISC::IStreamable {
public:
	LevelCategory() { };

	std::string Name;

	typedef std::vector<Level *> TLevelsList;
	TLevelsList Levels;

	void processXml(xmlNodePtr node);

	virtual Level *getLevelByID(uint32 levelId);
	void serial(NLMISC::IStream &stream);

	NLMISC_DECLARE_CLASS(LevelCategory);
};

class LevelCategoryProgressive : public LevelCategory {
public:

	LevelCategoryProgressive() { Name = "Progressive"; };
	void processXml(xmlNodePtr node) { };

	Level *getLevelByID(uint32 levelId);

	NLMISC_DECLARE_CLASS(LevelCategoryProgressive);
};

class Level : public NLMISC::IStreamable {
public:
	Level() : ParentCategory(NULL) { };
	Level(LevelCategory *parentCategory) : ParentCategory(parentCategory) { };
	LevelCategory *ParentCategory;

	uint32 ID;
	std::string Name;
	std::string Background;
	std::string BackgroundMusic;

	NLMISC::CRGBA AmbientLightColor;
	NLMISC::CRGBA DirectionalLightColor;
	NLMISC::CVector DirectionalLightPos;

	typedef std::vector<Tile *> TTilesList;
	TTilesList Tiles;

	void processXml(xmlNodePtr node);
	void load();
	void unload();
	void update(double deltaTime);
	void serial(NLMISC::IStream &stream);


	Tile &getStartTile();
	Tile &getTileByCoords(sint32 x, sint32 y);
	bool isTileByCoords(sint32 x, sint32 y) const;
	Tile &getTileById(uint32 id);
	bool isTileById(uint32 id);

	bool addTile(Tile *tile);

	NLMISC_DECLARE_CLASS(Level);
};

#endif // NL_LEVEL_H
