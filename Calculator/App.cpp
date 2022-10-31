#include "App.h"
#include "Window.h"
#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
#ifdef __WXMSW__
#include <wx/msw/msvcrt.h>      // redefines the new() operator 
#endif
wxIMPLEMENT_APP(App);

App::App() {

}
App::~App() {
	
}
bool App::OnInit() {
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
	_CrtMemDumpAllObjectsSince(NULL);
	_CrtDumpMemoryLeaks();
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF
		| _CRTDBG_LEAK_CHECK_DF);
	window = new Window();
	window->Show();
	return true;
}
int App::OnExit() {
	window = nullptr;
	return 0;
}