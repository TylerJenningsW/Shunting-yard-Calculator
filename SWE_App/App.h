#pragma once
#include "wx/wx.h"

// Launcher
class Window;
class App : public wxApp
{
	Window* window = nullptr;
public:
	App();
	~App();
	virtual bool OnInit();
	virtual int OnExit();
};

