#ifndef NL_LEVEL_MANAGER_H
#define NL_LEVEL_MANAGER_H

#include <string>

#include "CEGUI.h"

#include <nel/misc/types_nl.h>
#include <nel/misc/app_context.h>
#include <nel/misc/debug.h>
#include <nel/misc/dynloadlib.h>

#include <nel/misc/i_xml.h>

#include <nel/misc/rgba.h>
#include <nel/misc/vector.h>

#include <nel/3d/u_instance.h>

#include "neloid_exceptions.h"

class Level;
class Tile;
class LevelCategory;

class LevelManager {
	LevelManager() { m_currentLevel = NULL; };
	~LevelManager() { };

	NLMISC_SAFE_SINGLETON_DECL(LevelManager);

public:
	void init();
	void update();
	void render();

	Level *findLevel(std::string catName, uint32 levelNbr);
	void loadLevel(std::string catName, uint32 levelNbr);
	void unloadLevel();

	Level &getCurrentLevel() { if(m_currentLevel == NULL) throw ENoLevelLoaded(); return *m_currentLevel; }

	void generateLevel();
private:
	typedef std::vector<LevelCategory *> TLevelCategories;
	TLevelCategories m_levelCategories;
	Level *m_currentLevel;

};

#endif // NL_LEVEL_MANAGER_H