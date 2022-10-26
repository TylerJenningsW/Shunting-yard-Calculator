#pragma once
#include "wx/wx.h"
#include "idList.h"
#include <unordered_map>
#include "Token.h"
#include "Window.h"
static class Factory {
public:
		static std::unordered_map<ids, std::string> FillPairs(std::string idStrings[], Window* parent) {
			int count = (ids::COS - ids::MAINWINDOW)-2;
			std::unordered_map<ids, std::string> btns = parent->GetPairs();
			for (int i = (ids::COS); i >= ids::EQUALS; --i) {
				btns.insert(std::make_pair((ids)i, idStrings[count]));
				--count;
			}
			return btns;
		}
		static void CreateButtons(wxGridSizer* grid, Window* parent) {
			int j = (ids::COS - ids::MAINWINDOW)-2;
			for (int i = (ids::COS); i >= ids::EQUALS; --i)
			{
				wxButton* btn = new wxButton(parent, (ids)i, _idStrings[j], wxDefaultPosition, wxSize(wxDefaultSize), wxBORDER_NONE);
				btn->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_SEMIBOLD, false));
				// Set button background color based on text
				if (i == (ids::EQUALS)) {
					btn->SetOwnBackgroundColour(wxColour(19, 66, 110));
				}
				else if (i == (ids::CLEAR)) {
					btn->SetOwnBackgroundColour(wxColour(255, 70, 0));
				}
				else {
					btn->SetOwnBackgroundColour(wxColour(18, 18, 19));
				}
				// Set text color
				btn->SetOwnForegroundColour(wxColour(255, 255, 255));

				btn->Bind(wxEVT_BUTTON, &Window::OnButtonClick, parent);
				grid->Add(btn, 0, wxEXPAND);
				--j;
			}
		}
		static void CreateSizers(Window* parent) {
				wxBoxSizer* _sizer = new wxBoxSizer(wxVERTICAL);
				parent->SetWindowSizer(_sizer);
				wxGridSizer* _gridSizer = new wxGridSizer(6, 4, 0, 0);
				parent->SetWindowGridSizer(_gridSizer);
		}
		static void CreateTextCtrl(Window* parent, wxTextValidator* validator) {
			wxTextCtrl* _output = new wxTextCtrl(parent, ids::OUTPUT, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_RIGHT, *validator);
			_output->SetFont(wxFont(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false));

			// Set textbox background and text color
			_output->SetBackgroundColour(wxColour(6, 11, 11));
			_output->SetForegroundColour(wxColour(255, 255, 255));

			parent->SetTextControl(_output);
		}
};