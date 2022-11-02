#include "App.h"
#include "Window.h"
wxIMPLEMENT_APP(App);

App::App() {

}
App::~App() {
	
}
bool App::OnInit() {
	window = new Window();
	window->Show();
	return true;
}
int App::OnExit() {
	window = nullptr;
	return 0;
}