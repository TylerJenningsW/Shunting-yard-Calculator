#pragma once
#include "wx/wx.h"
#include "idList.h"
#include <unordered_map>
#include "Token.h"
#include "Window.h"
static class Factory {
public:
		static std::unordered_map<ids, std::string> FillPairs(std::string idStrings[], Window* parent) {
			int count = (ids::TAN - ids::MAINWINDOW)-2;
			std::unordered_map<ids, std::string> btns = parent->GetPairs();
			for (int i = (ids::TAN); i >= ids::BACK; --i) {
				btns.insert(std::make_pair((ids)i, idStrings[count]));
				--count;
			}
			return btns;
		}
		static void CreateButtons(wxGridSizer* grid, Window* parent) {
			int j = (ids::TAN - ids::MAINWINDOW)-2;
			for (int i = (ids::TAN); i >= ids::BACK; --i)
			{
				wxButton* btn = new wxButton(parent, (ids)i, _idStrings[j]);
				btn->SetFont(wxFont(16, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false));
				btn->Bind(wxEVT_BUTTON, &Window::OnButtonClick, parent);
				grid->Add(btn, 0, wxEXPAND);
				--j;
			}
		}
		static void CreateSizers(Window* parent) {
				wxBoxSizer* _sizer = new wxBoxSizer(wxVERTICAL);
				parent->SetWindowSizer(_sizer);
				wxGridSizer* _gridSizer = new wxGridSizer(4, 6, 3, 3);
				parent->SetWindowGridSizer(_gridSizer);
		}
		static void CreateTextCtrl(Window* parent, wxTextValidator* validator) {
			wxTextCtrl* _output = new wxTextCtrl(parent, ids::OUTPUT, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_RIGHT, *validator);
			parent->SetTextControl(_output);
		}
};