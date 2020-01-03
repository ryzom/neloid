#ifndef NL_NELOIDGAME_H
#define NL_NELOIDGAME_H

#include <string>
#include <deque>

#include <nel/misc/types_nl.h>
#include <nel/misc/app_context.h>
#include <nel/misc/debug.h>
#include <nel/misc/path.h>
#include <nel/misc/config_file.h>

#include <nel/3d/u_driver.h>
#include <nel/3d/u_text_context.h>
#include <nel/3d/u_scene.h>
#include <nel/3d/u_camera.h>
#include <nel/3d/u_light.h>
#include <nel/3d/u_instance.h>
#include <nel/3d/u_material.h>
#include <nel/misc/geom_ext.h>

#include "CState.h"

class Cube;

class NeloidGame {
	NeloidGame() : m_initialized(false), m_needExit(false), m_driver(NULL), m_textContext(NULL) { };
	~NeloidGame();

	NLMISC_SAFE_SINGLETON_DECL(NeloidGame);
public:
	void init();
	void run();

	enum TGameStates {
		MainMenu,
		Playing,
		Paused,
		EndLevel,
		Editor
	};

	/// Allow other classes to request the main loop to end.
	void requestExit(bool exit) { m_needExit=exit; };

	/// Process the keyboard input.
	void processKeyboardInput();

	/// Request the game to start.
	void requestStartGame();

	/// Request the game to be paused.
	void requestPauseGame();

	/// Request a paused game to be resumed.
	void requestResumeGame();

	/// Called when a level ends.
	void requestLevelEnd();

	/// Called when the block lands on a portal, warps them to the new tile.
	void requestLevelPortal(uint32 portalId);

	/// Called to restart a level (restore broken tiles, move the cuboid back.
	void requestLevelRestart();

	/// Called when the Level Editor is requested at the main menu.
	void requestLevelEditor();

	void requestRotateRight();
	void requestRotateLeft();
	void requestRotateUp();
	void requestRotateDown();

	void checkMoveResult();

	TGameStates getCurrentGameState() { return m_gameState; }

	NL3D::UDriver &driver() const { return *m_driver; };

	NL3D::UScene &scene() const { return *m_scene; };

	NL3D::UTextContext &textContext() const { return *m_textContext; }

	NL3D::ULight &light() const { return *m_light; };

	NLMISC::CConfigFile &configFile() { return m_configFile; };

	NL3D::UCamera &camera() { return m_camera; }

	double getTime();
	
	
private:
	bool m_initialized;
	bool m_needExit;
	TGameStates m_gameState;
	Cube *m_cube;
	double m_dt, m_prevTime;

	//uint32 m_falls;
	//uint32 m_moves;

	typedef std::deque<uint32> TTileCheckQueue;
	TTileCheckQueue		 m_tileCheckQueue;

	// The 3d driver
	NL3D::UDriver        *m_driver;

	// holds the pointer to the configuration file(s)
	NLMISC::CConfigFile	 m_configFile;

	// This variable is used to display text on the screen
	NL3D::UTextContext   *m_textContext;

	NL3D::UScene		 *m_scene;

	NL3D::UCamera		  m_camera;

	NL3D::ULight		 *m_light;

	CState				  m_cuboidState;
	NL3D::UInstance		  m_logicTester1, m_logicTester2;
	NL3D::UInstance		  m_axes;
	NL3D::UMaterial		  m_overlayMat;
	NLMISC::CQuadUV		  m_screenQuad;
};

#endif // NL_NELOIDGAME_H