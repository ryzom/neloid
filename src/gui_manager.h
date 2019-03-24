#ifndef NL_GUI_MANAGER_H
#define NL_GUI_MANAGER_H

#include <string>

#include "CEGUI.h"

#include <nel/misc/types_nl.h>
#include <nel/misc/app_context.h>
#include <nel/misc/debug.h>
#include <nel/misc/dynloadlib.h>

class GuiManager {
	GuiManager() { };
	~GuiManager() { };

	NLMISC_SAFE_SINGLETON_DECL(GuiManager);
public:
	void init();
	void update();
	void render();
	void toggleWindow(std::string windowName, bool visibility);
	void toggleCursor(bool visibility);
	bool getCursorState();

private:
	CEGUI::System		*m_GuiSystem;
	CEGUI::Renderer	*m_GuiRenderer;
	NLMISC::CLibrary m_DriverLib;
};

#endif // NL_GUI_MANAGER_H