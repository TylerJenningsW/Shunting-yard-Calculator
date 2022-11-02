#include "App.h"
#include "Window.h"
#include "CalculatorProcessor.h"
wxIMPLEMENT_APP(App);

App::App() {

}
App::~App() {
	
}
bool App::OnInit() {
	window = new Window(true);
	window->Show();
	return true;
}
int App::OnExit() {
	CalculatorProcessor::DestroyInstance();
	window = nullptr;
	return 0;
}