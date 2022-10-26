#include "Factory.h"
#include "Window.h"
#include "idList.h"
#include "Token.h"

std::unordered_map<ids, std::string> Factory::FillPairs(std::string idStrings[], Window* parent)
{
	int count = (ids::COS - ids::MAINWINDOW) - 2;
	std::unordered_map<ids, std::string> btns = parent->GetPairs();
	for (int i = (ids::COS); i >= ids::EQUALS; --i) {
		btns.insert(std::make_pair((ids)i, idStrings[count]));
		--count;
	}
	return btns;
}

void Factory::CreateButtons(wxGridSizer* grid, Window* parent)
{
	int j = (ids::COS - ids::MAINWINDOW) - 2;
	for (int i = (ids::COS); i >= ids::EQUALS; --i) {
		wxButton* btn = new wxButton(parent, (ids)i, _idStrings[j]);
		btn->SetFont(wxFont(16, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false));
		btn->SetBackgroundColour(wxColour(50, 50, 50));
		btn->SetForegroundColour(wxColour(255, 255, 255));
		btn->Bind(wxEVT_BUTTON, &Window::OnButtonClick, parent);
		grid->Add(btn, 0, wxEXPAND);
		--j;
	}
}

void Factory::CreateSizers(Window* parent)
{
	wxBoxSizer* _sizer = new wxBoxSizer(wxVERTICAL);
	parent->SetWindowSizer(_sizer);
	wxGridSizer* _gridSizer = new wxGridSizer(6, 4, 3, 3);
	parent->SetWindowGridSizer(_gridSizer);
}

void Factory::CreateTextCtrl(Window* parent, wxTextValidator* validator)
{
	wxTextCtrl* _output = new wxTextCtrl(parent, ids::OUTPUT, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_RIGHT, *validator);
	_output->SetFont(wxFont(32, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false));
	parent->SetTextControl(_output);
}
