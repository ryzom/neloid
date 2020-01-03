#include "neloidgame.h"

#include "gui_manager.h"
#include "level_manager.h"
#include "level_editor.h"
#include "cube.h"
#include "CState.h"
#include "CMove.h"
#include "tile.h"
#include "level.h"
#include "session.h"

#include <nel/misc/time_nl.h>
#include <nel/3d/bloom_effect.h>
#include <nel/3d/driver_user.h>
#include <nel/3d/material.h>

#include <nel/misc/hierarchical_timer.h>

NLMISC_SAFE_SINGLETON_IMPL(NeloidGame)

NeloidGame::~NeloidGame() { 
	delete m_cube; 

	// TODO release NeL stuff.
}



void NeloidGame::init() {
	// Create an application context.
    NLMISC::createDebug();
	m_prevTime = getTime();

    // Add a search path so we know how to find fonts.
    NLMISC::CPath::addSearchPath("./data", true, false, NULL);

	m_configFile.load(NLMISC::CPath::lookup("neloid.cfg"));

    // Create a driver
	nlinfo("Creating driver.");
	m_driver = NL3D::UDriver::createDriver(0, false);
    nlassert(m_driver);

    // Create the window with config file values
	m_driver->setDisplay(NL3D::UDriver::CMode(	m_configFile.getVar("ScreenWidth").asInt(), 
												m_configFile.getVar("ScreenHeight").asInt(), 
												m_configFile.getVar("ScreenDepth").asInt()));


    // Create a Text context for later text rendering
    m_textContext = m_driver->createTextContext(NLMISC::CPath::lookup("VeraMono.ttf"));
    nlassert(m_textContext);

	m_scene = m_driver->createScene(false);

	// Key light
	m_light = NL3D::ULight::createLight();
	m_light->setMode(NL3D::ULight::DirectionalLight);
	m_light->setDirection(NLMISC::CVector(0.035f, -0.655f, -0.755f));
	m_light->setAmbiant(NLMISC::CRGBA(20, 30, 50));
	m_light->setDiffuse(NLMISC::CRGBA(255, 255, 229));
	m_driver->setLight(0, *m_light);
	m_driver->enableLight(0);

	// Fill light
	m_light = NL3D::ULight::createLight();
	m_light->setMode(NL3D::ULight::DirectionalLight);
	m_light->setDirection(NLMISC::CVector(-0.375f, 0.574f, 0.728f));
	m_light->setAmbiant(NLMISC::CRGBA(20, 20, 0));
	m_light->setDiffuse(NLMISC::CRGBA(90, 133, 178));
	m_driver->setLight(1, *m_light);
	m_driver->enableLight(1);
//	m_driver->setAmbientColor(NLMISC::CRGBA(25,25,25));

	m_camera = m_scene->getCam();

	m_camera.setTransformMode(NL3D::UTransformable::DirectMatrix);
	m_camera.setPerspective ((float)NLMISC::Pi/2.f, 1.33f, 0.1f, 1000);

	m_cube = NULL;

	GuiManager::getInstance().init();
	LevelManager::getInstance().init();
	LevelEditor::getInstance().init();
	SessionManager::getInstance().init();

	m_overlayMat = m_driver->createMaterial();
	m_overlayMat.getObjectPtr()->initUnlit();
	m_overlayMat.getObjectPtr()->setColor(NLMISC::CRGBA(0, 0, 0, 128));
	m_overlayMat.getObjectPtr()->setBlend(true);
	m_overlayMat.getObjectPtr()->setBlendFunc(NL3D::CMaterial::srcalpha, NL3D::CMaterial::invsrcalpha);
	m_overlayMat.getObjectPtr()->setZWrite(false);
	m_overlayMat.getObjectPtr()->setZFunc(NL3D::CMaterial::always);
	m_overlayMat.getObjectPtr()->setDoubleSided(true);

	m_screenQuad.V0 = NLMISC::CVector(-1.f, -1.f,  0.5f);
	m_screenQuad.V1 = NLMISC::CVector(1.f,  -1.f,  0.5f);
	m_screenQuad.V2 = NLMISC::CVector(1.f,  1.f,   0.5f);
	m_screenQuad.V3 = NLMISC::CVector(-1.f, 1.f,   0.5f);

	NL3D::CBloomEffect::instance().setDriver(m_driver);
	NL3D::CBloomEffect::instance().setScene(m_scene);
	NL3D::CBloomEffect::instance().init(true);
	NL3D::CBloomEffect::instance().setDensityBloom(120);
	NL3D::CBloomEffect::instance().setSquareBloom(false);

	m_initialized = true;
}

void NeloidGame::run() {
	while ((!m_needExit) && m_driver->isActive()) {
		// Time stuff
		m_dt = getTime() - m_prevTime;
		m_prevTime = getTime();


		// Check the configuration files for changes.
		m_configFile.checkConfigFiles();

		// Call the render method so that the level can update and render.
		if(m_gameState == Playing) {
			// Update the cube.
			if(m_cube != NULL) {
				m_cube->update(m_dt);
				NLMISC::CVector lookFrom(5.f, -8.f, 6.f);
				lookFrom.x+=m_cube->getDummyPos().x;
				lookFrom.y+=m_cube->getDummyPos().y;
				m_camera.lookAt(lookFrom, m_cube->getDummyPos());
			}
			// Update the logic testing debug objects.
			if(!m_logicTester1.empty()) {
				m_logicTester1.setPos(NLMISC::CVector((float)m_cuboidState.x1, (float)m_cuboidState.y1, 0.0f));
				m_logicTester2.setPos(NLMISC::CVector((float)m_cuboidState.x2, (float)m_cuboidState.y2, 0.0f));
			}

			// Upload the current level.
			LevelManager::getInstance().getCurrentLevel().update(m_dt);

			// Check to see the result of our move.
			if(m_cuboidState.NeedMoveCheck && !m_cube->isAnimating())
				checkMoveResult();
		}

		// Handle the level editor.
		if(m_gameState == Editor) {
			LevelEditor::getInstance().update();
		}

		// Request scene animation.
		m_scene->animate(getTime());

		// Render the scene.
		if(NL3D::CBloomEffect::instance().getDriver() != NULL) {
			NL3D::CBloomEffect::instance().initBloom();
		}
        // Clear all buffers
		m_driver->clearBuffers(NLMISC::CRGBA(111, 111, 111));
		m_scene->render();
		if(NL3D::CBloomEffect::instance().getDriver() != NULL) {
			NL3D::CBloomEffect::instance().endBloom();
			NL3D::CBloomEffect::instance().endInterfacesDisplayBloom();
//			((NL3D::CDriverUser *) m_driver)->getDriver()->setRenderTarget(NULL);
		}


		m_driver->setMatrixMode2D11();

        // Display a text on the screen
		m_textContext->setHotSpot(NL3D::UTextContext::TopLeft);
		m_textContext->setColor(NLMISC::CRGBA(255, 255, 255, 255));
        m_textContext->setFontSize(14);
		m_textContext->printfAt(0.01f, 0.99f, "Use arrow keys to move the cube. pressing h shows or hides the dummy cube (for debugging)");
		
		// temporarily putting this here to print our cursor location text.
		LevelEditor::getInstance().renderText();

		if(m_gameState == Playing) {
			std::string moves = NLMISC::toString(SessionManager::getInstance().getCurrentSession().getCurrentPlay().Moves);
			std::string falls = NLMISC::toString(SessionManager::getInstance().getCurrentSession().getCurrentPlay().Falls);
			std::string debugStr = "Moves: " + moves+" Falls: "+falls;
			m_textContext->printfAt(0.01f, 0.95f, debugStr.c_str());
		}
		
		if(m_gameState == Paused) {
			m_driver->drawQuad(m_screenQuad, m_overlayMat);
		}

		// Render the GUI
		GuiManager::getInstance().render();

        // Swap 3d buffers
        m_driver->swapBuffers ();

        // Pump user input messages
        m_driver->EventServer.pump();

        // Manage the keyboard
		processKeyboardInput();
    }
}

void NeloidGame::processKeyboardInput() {
	if (m_driver->AsyncListener.isKeyDown(NLMISC::KeySHIFT)
			&& m_driver->AsyncListener.isKeyDown(NLMISC::KeyESCAPE)) {
           // Shift Escape -> quit
           m_needExit = true;
       }

	// Allow escape to pull up/close the menu.
	if(m_driver->AsyncListener.isKeyPushed(NLMISC::KeyESCAPE)) {
		if(m_gameState == Playing)
			requestPauseGame();
		else if(m_gameState == Paused)
			requestResumeGame();
	}

	// Toggle the mouse cursor. Temporary while testing level editor code.
	if(m_driver->AsyncListener.isKeyPushed(NLMISC::KeyTAB) && m_gameState == Playing) {
		GuiManager::getInstance().toggleCursor(!GuiManager::getInstance().getCursorState());
	}

	// Handle gameplay keys.
	if(m_cube != NULL && !m_cube->isAnimating() && m_gameState == Playing) {
		// Rotate right.
		if(m_driver->AsyncListener.isKeyPushed(NLMISC::KeyRIGHT)) {
			requestRotateRight();
		}
		// Rotate left.
		else if(m_driver->AsyncListener.isKeyPushed(NLMISC::KeyLEFT)) {			
			requestRotateLeft();
		}
		// Rotate down.
		else if(m_driver->AsyncListener.isKeyPushed(NLMISC::KeyDOWN)) {
			requestRotateDown();
		}
		// Rotate up.
		else if(m_driver->AsyncListener.isKeyPushed(NLMISC::KeyUP)) {
			requestRotateUp();
		}

		// Allow toggling of visual debug information.
		if(m_driver->AsyncListener.isKeyPushed(NLMISC::KeyH)) {
			m_cube->toggleDebugVisibility();
			if(m_logicTester1.getVisibility() == NL3D::UTransform::Show) {
				m_logicTester1.hide();
				m_logicTester2.hide();
				m_axes.hide();
			} else {
				m_logicTester1.show();
				m_logicTester2.show();
				m_axes.show();
			}
		}
	}
}

void NeloidGame::requestRotateRight() {
	m_cuboidState *= CMove::xpos;
	m_cube->applyMove(CMove::xpos);
}

void NeloidGame::requestRotateLeft() {
	m_cuboidState *= CMove::xneg;
	m_cube->applyMove(CMove::xneg);
}

void NeloidGame::requestRotateUp() {
	m_cuboidState *= CMove::ypos;
	m_cube->applyMove(CMove::ypos);
}

void NeloidGame::requestRotateDown() {
	m_cuboidState *= CMove::yneg;
	m_cube->applyMove(CMove::yneg);
}

void NeloidGame::checkMoveResult() {
	try {
		Tile &tile1 = LevelManager::getInstance().getCurrentLevel().getTileByCoords(m_cuboidState.x1, m_cuboidState.y1);
		Tile &tile2 = LevelManager::getInstance().getCurrentLevel().getTileByCoords(m_cuboidState.x2, m_cuboidState.y2);

		tile1.contact(m_cuboidState.isUpright());
		if(tile1.ID != tile2.ID)
			tile2.contact(m_cuboidState.isUpright());

		// If either of the tiles have broken.
		if(!tile1.TileActive || !tile2.TileActive) {
			// Increment the falls.
			SessionManager::getInstance().getCurrentSession().incrementFalls();

			// Restart the level.
			requestLevelRestart();
		}

		// Before we add the tiles we're on, check and clear out the previous queue.
		while(!m_tileCheckQueue.empty()) {
			uint32 tileId = m_tileCheckQueue.front();
			try {
				Tile &tile = LevelManager::getInstance().getCurrentLevel().getTileById(tileId);

				if( tile.PosX != m_cuboidState.x1 || tile.PosY != m_cuboidState.y1 || /* not on tile 1 */
					tile.PosX != m_cuboidState.x2 || tile.PosY != m_cuboidState.y2 ) { /* not on tile 2 */
						tile.contactLeft();
				}
			} catch(NLMISC::Exception) {
				nlinfo("Problem retrieving tile ID %d from current level.", tileId);
			}
			m_tileCheckQueue.pop_front();
		}


		// Add the tile to the queue to check when contact is left.
		m_tileCheckQueue.push_back(tile1.ID);
		if(tile1.ID != tile2.ID)
			m_tileCheckQueue.push_back(tile2.ID);

	} catch(NLMISC::Exception) {
		// Increment the falls.
		SessionManager::getInstance().getCurrentSession().incrementFalls();

		// Restart the level.
		requestLevelRestart();
	}

	SessionManager::getInstance().getCurrentSession().incrementMoves();
	m_cuboidState.NeedMoveCheck = false;
}

void NeloidGame::requestStartGame() {
	nlinfo("Start game requested.");

	// Hide the menu.
	GuiManager::getInstance().toggleWindow("Root/MainMenu", false);
	GuiManager::getInstance().toggleCursor(false);

	// Load the level. For now we'll just load the fisrt beginner level.
	LevelManager::getInstance().loadLevel("Beginner", 1);
	//LevelManager::getInstance().generateLevel();

	// Reset cube (+ stats)
	if(m_cube==NULL)
		m_cube = new Cube();
	m_cube->setVisible(true);

	if(m_axes.empty()) {
		m_axes = scene().createInstance("axes.shape");
		m_axes.hide();
	}
	if(m_logicTester1.empty()) {
		m_logicTester1 = scene().createInstance("logicTester.shape");
		m_logicTester1.setTransformMode(NL3D::UTransformable::RotEuler, NLMISC::CMatrix::XYZ);
		m_logicTester1.hide();
	}
	if(m_logicTester2.empty()) {
		m_logicTester2 = scene().createInstance("logicTester.shape");
		m_logicTester2.setTransformMode(NL3D::UTransformable::RotEuler, NLMISC::CMatrix::XYZ);
		m_logicTester2.hide();
	}

	// TODO since we don't allow loading of saved games yet just create a new one.
	SessionManager::getInstance().getNewSession("mattsession").startNewPlay("Beginner", 1);
	//SessionManager::getInstance().getCurrentSession().startNewPlay("Beginner", 1);

	// Get the start tile for the current level and move the cube there.
	requestLevelRestart();

	// Calculate the camera look-at position
	m_camera.lookAt(NLMISC::CVector(5.f, -8.f, 6.f), NLMISC::CVector::Null);

	// Enable game controls.

	// allow 'esc' to pause the game (and show menu.)
	m_gameState = Playing;
}

void NeloidGame::requestResumeGame() {
	// Enable game controls, disable UI controls.

	// Hide the menu (hide resume, show start)
	GuiManager::getInstance().toggleWindow("Root/MainMenu/Start Game", true);
	GuiManager::getInstance().toggleWindow("Root/MainMenu/ResumeGame", false);
	GuiManager::getInstance().toggleWindow("Root/MainMenu", false);
	GuiManager::getInstance().toggleCursor(false);

	// Allow 'esc' to pause the game.
	m_gameState = Playing;
}

void NeloidGame::requestPauseGame() {
	// Disable game controls, enable UI controls.

	// Show the menu (hide start,show resume.)
	GuiManager::getInstance().toggleWindow("Root/MainMenu/Start Game", false);
	GuiManager::getInstance().toggleWindow("Root/MainMenu/ResumeGame", true);
	GuiManager::getInstance().toggleWindow("Root/MainMenu/Level Editor", false);
	GuiManager::getInstance().toggleWindow("Root/MainMenu", true);
	GuiManager::getInstance().toggleCursor(true);

	// Allow 'esc' to resume the game.
	m_gameState = Paused;
}

void NeloidGame::requestLevelEnd() {
	try {
		// Get the next category/level number to load.
		std::string levelCat = LevelManager::getInstance().getCurrentLevel().ParentCategory->Name;

		// TODO: Add a method like 'getNextLevelId() instead of assuming the next level ID is simply incremented.
		uint32 levelNum = LevelManager::getInstance().getCurrentLevel().ID+1;

		// Unload the previous level's 3D data.
		LevelManager::getInstance().unloadLevel();

		// Load the new level.
		LevelManager::getInstance().loadLevel(levelCat, levelNum);

		// Restart the level.
		requestLevelRestart();
	} catch(ENoSuchLevel) {
		// Show menu (+ make sure resume game is hidden and start game is visible.)
		GuiManager::getInstance().toggleWindow("Root/MainMenu/Start Game", true);
		GuiManager::getInstance().toggleWindow("Root/MainMenu/ResumeGame", false);
		GuiManager::getInstance().toggleWindow("Root/MainMenu", true);
		GuiManager::getInstance().toggleCursor(true);

		m_gameState = EndLevel;
	}	
}

void NeloidGame::requestLevelRestart() {
	Level &currentLevel = LevelManager::getInstance().getCurrentLevel();
	Tile &startTile = currentLevel.getStartTile();
	m_cuboidState = CState(startTile);
	m_cube->setToState(m_cuboidState);
}

void NeloidGame::requestLevelPortal(uint32 portalId) {
	try {
		Level currentLevel = LevelManager::getInstance().getCurrentLevel();
		Tile &portalTile = currentLevel.getTileById(portalId);
		m_cuboidState = CState(portalTile);
		m_cube->setToState(m_cuboidState);
	} catch(NLMISC::Exception) {
		nlinfo("ITS A TRAP, THIS PORTAL GOES NOWHERE!#");
		requestLevelRestart();
		SessionManager::getInstance().getCurrentSession().incrementFalls();
	}
}

void NeloidGame::requestLevelEditor() {
	// Hide the menu.
	GuiManager::getInstance().toggleWindow("Root/MainMenu", false);
	GuiManager::getInstance().toggleCursor(false);

	// Show the editor menu.
	GuiManager::getInstance().toggleWindow("Root/LevelEditor", true);

	m_gameState = Editor;

	LevelEditor::getInstance().setActive(true);
}

double NeloidGame::getTime() {
	return NLMISC::CTime::ticksToSecond(NLMISC::CTime::getPerformanceTime());
}