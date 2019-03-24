#include "level_manager.h"

#include <nel/misc/types_nl.h>
#include <nel/misc/file.h>
#include <nel/misc/path.h>
#include <nel/misc/i_xml.h>
#include <nel/misc/rgba.h>
#include <nel/misc/algo.h>

#include "neloidgame.h"
#include "tile.h"
#include "level.h"
#include "xml_helpers.h"
#include "level_editor.h"

NLMISC_SAFE_SINGLETON_IMPL(LevelManager)

void LevelManager::init() {
	nlinfo("Loading Level Manager");

	try {
		NLMISC::CIFile file;

		if(file.open(NLMISC::CPath::lookup("exampleLevels.xml"))) {
			NLMISC::CIXml input;

			if(input.init(file)) {
				// Handle reading the XML file here.
				xmlNodePtr rootNode = input.getRootNode();

				// Verify that the root node is a Levels tag
				// ...

				LevelCategory *levelCategory = NULL;
				try {
					// Create the new level category and process it.
					levelCategory = new LevelCategory();
					levelCategory->processXml(rootNode);

					// Add it to the list of categories available.
					m_levelCategories.push_back(levelCategory);
				} catch(NLMISC::Exception &e) {
					// Clean up the level category.
					if(levelCategory != NULL)
						delete levelCategory;

					nlwarning("Unable to process level category: %s", e.what());
				}
				
			}

			// Cleanup.
			file.close();
		} else
			nlwarning("Unable to load file: exampleLevel.xml");
	} catch(NLMISC::Exception &e) {
		// Handle the exception here...
		nlinfo("Caugh exception: %s", e.what());
	}
}

void LevelManager::update() {

}

void LevelManager::render() {

}

void LevelManager::loadLevel(std::string catName, uint32 levelNbr) {
	Level *level = findLevel(catName, levelNbr);

	level->load();
	m_currentLevel = level;
}

Level *LevelManager::findLevel(std::string catName, uint32 levelNbr) {
	Level *foundLevel=NULL;
	TLevelCategories::iterator itr = m_levelCategories.begin();
	while(itr != m_levelCategories.end()) {
		if( (*itr)->Name == catName ) {
			LevelCategory::TLevelsList::iterator levelsItr = (*itr)->Levels.begin();
			while(levelsItr != (*itr)->Levels.end()) {
				if((*levelsItr)->ID == levelNbr) {
					foundLevel=(*levelsItr);
					break;
				}	
				levelsItr++;
			}
		}

		if(foundLevel != NULL)
			break;

		itr++;
	}

	if(foundLevel == NULL)
		throw ENoSuchLevel("Unable to find level: '"+catName+"' : "+NLMISC::toString(levelNbr));

	return foundLevel;
}

void LevelManager::unloadLevel() {
	m_currentLevel->unload();
	m_currentLevel = NULL;
}

void LevelManager::generateLevel() {
	m_currentLevel = LevelEditor::getInstance().generateLevel(10);
}