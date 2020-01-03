#include "gui_manager.h"

#include <nel/cegui/inellibrary.h>
#include <nel/cegui/nelrenderer.h>

#include "neloidgame.h"

NLMISC_SAFE_SINGLETON_IMPL(GuiManager)

bool handleExitGameBtn(const CEGUI::EventArgs& e) {
	NeloidGame::getInstance().requestExit(true);
	return true;
}

bool handleStartGameBtn(const CEGUI::EventArgs& e) {
	NeloidGame::getInstance().requestStartGame();
	return true;
}

bool handleResumeGameBtn(const CEGUI::EventArgs& e) {
	NeloidGame::getInstance().requestResumeGame();
	return true;
}

bool handleLevelEditorBtn(const CEGUI::EventArgs& e) {
	NeloidGame::getInstance().requestLevelEditor();
	return true;
}

void GuiManager::init() {
	nlinfo("Loading NeL CEGUI Renderer.");

	// Load the CEGUI renderer and get a handle to the library.
	if(!m_DriverLib.loadLibrary("nelceguirenderer", true, true , true)) {
		nlerror("Failed to load NeL CEGUI Renderer library.");
	}
	NELRENDERER_CREATE_PROC createNelRenderer = reinterpret_cast<NELRENDERER_CREATE_PROC>(m_DriverLib.getSymbolAddress(NELRENDERER_CREATE_PROC_NAME));

	// create the CEGUI renderer.
	try {
		m_GuiRenderer = createNelRenderer(&NeloidGame::getInstance().driver(), true);
	} catch(CEGUI::GenericException &e) {
		nlerror("Caught CEGUI Exception during renderer creation: %s", e.getMessage().c_str());
	}

	try {
		m_GuiSystem = new CEGUI::System(m_GuiRenderer);
	} catch(CEGUI::GenericException &e) {
		nlerror("Caught CEGUI Exception during System creation: %s", e.getMessage().c_str());
	}

	CEGUI::NeLRenderer *rndr = (CEGUI::NeLRenderer *)m_GuiRenderer;
	rndr->activateInput();
	rndr->captureCursor(true);

	try	{
		using namespace CEGUI;

		Logger::getSingleton().setLoggingLevel(Insane);

		// load scheme and set up defaults
		SchemeManager::getSingleton().loadScheme("TaharezLook.scheme");
		System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");

		// create the font and set it up as the default.
		FontManager::getSingleton().createFont("Commonwealth-10.font");


		//Window* sheet = WindowManager::getSingleton().createWindow("DefaultWindow", "root_wnd");
		Window* sheet = WindowManager::getSingleton().loadWindowLayout("neloid.layout");
		System::getSingleton().setGUISheet(sheet);
		WindowManager::getSingleton().getWindow("Root/MainMenu/ResumeGame")->hide();
		WindowManager::getSingleton().getWindow("Root/LevelEditor")->hide();
	} catch(CEGUI::GenericException &e) {
		nlinfo("Caught CEGUI Exception during System creation: %s", e.getMessage().c_str());
	} catch(CEGUI::Exception &e) {	// catch to prevent exit (errors will be logged).
		nlinfo("Failed to initialize CEGUI system: %s", e.getMessage().c_str());
	}

	const CEGUI::Font *fnt = m_GuiSystem->getDefaultFont();

	// Set up the botton subscriptions
	CEGUI::WindowManager::getSingleton().getWindow("Root/MainMenu/ExitGame")->
		subscribeEvent(CEGUI::PushButton::EventClicked, handleExitGameBtn);
	CEGUI::WindowManager::getSingleton().getWindow("Root/MainMenu/Start Game")->
		subscribeEvent(CEGUI::PushButton::EventClicked, handleStartGameBtn);
	CEGUI::WindowManager::getSingleton().getWindow("Root/MainMenu/ResumeGame")->
		subscribeEvent(CEGUI::PushButton::EventClicked, handleResumeGameBtn);
	CEGUI::WindowManager::getSingleton().getWindow("Root/MainMenu/Level Editor")->
		subscribeEvent(CEGUI::PushButton::EventClicked, handleLevelEditorBtn);
}

void GuiManager::update() {

}

void GuiManager::render() {
	m_GuiSystem->renderGUI();
}

void GuiManager::toggleWindow(std::string windowName, bool visibility) {
	if(visibility)
		CEGUI::WindowManager::getSingleton().getWindow(windowName.c_str())->show();
	else
		CEGUI::WindowManager::getSingleton().getWindow(windowName.c_str())->hide();
}

void GuiManager::toggleCursor(bool visibility) {
	if(visibility)
		CEGUI::MouseCursor::getSingleton().show();
	else
		CEGUI::MouseCursor::getSingleton().hide();
}

bool GuiManager::getCursorState() {
	return CEGUI::MouseCursor::getSingleton().isVisible();
}