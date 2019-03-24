#include <nel/misc/types_nl.h>
#include <nel/misc/app_context.h>
#include <nel/misc/path.h>

#include <nel/3d/u_driver.h>
#include <nel/3d/u_text_context.h>

#include "neloidgame.h"

using namespace std;
using namespace NLMISC;
using namespace NL3D;

#ifndef DATA_DIR
#       define DATA_DIR "."
#endif

#ifdef NL_OS_WINDOWS
#	ifdef _UNICODE
#		define tstring wstring
#	else
#		define tstring string
#	endif
sint WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdline, int nCmdShow)
#else
sint main(int argc, char **argv)
#endif
{
	NLMISC::CApplicationContext applicationContext;
	NeloidGame::getInstance().init();
	NeloidGame::getInstance().driver().setCapture(false);
	NeloidGame::getInstance().run();
}
