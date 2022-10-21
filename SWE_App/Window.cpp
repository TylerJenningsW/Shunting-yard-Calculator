
#include "Window.h"
#include "Factory.h"
#include "CalculatorProcessor.h"


Window::Window() : wxFrame(nullptr, ids::MAINWINDOW, "Calculator", wxDefaultPosition, wxDefaultSize) {
	// top sizer -> grid sizer
	Factory::CreateSizers(this);
	// include specific symbols and numbers
	AddToIncludes();
	// create output
	Factory::CreateTextCtrl(this, _validator);
	// pair id with string
	_btnPairs = Factory::FillPairs(_idStrings, this);
	// add grid sizer and text output to top sizer
	AddToSizer();
	SetSizer(_sizer);
	// add buttons to grid
	Factory::CreateButtons(_gridSizer, this);
	// minimum window size & open in center of screen
	SetMinSize(wxSize(400, 600));
	Centre();
}
Window::~Window() {
	delete _validator;
	_validator = nullptr;
}

wxTextCtrl* Window::GetOutput() {
	return _output;
}

std::unordered_map<ids, std::string> Window::GetPairs()
{
	return _btnPairs;
}

void Window::SetWindowSizer(wxBoxSizer* sizer) {
	_sizer = sizer;
}

void Window::SetWindowGridSizer(wxGridSizer* gridSizer) {
	_gridSizer = gridSizer;
}

void Window::SetTextControl(wxTextCtrl* textCtrl) {
	_output = textCtrl;
}

void Window::OnButtonClick(wxCommandEvent& evt) {
	// fetch singleton to parse event
	CalculatorProcessor* _processor = CalculatorProcessor::GetInstance();
	_processor->ParseId(this, (ids)evt.GetId());
	evt.Skip();
}

void Window::AddToSizer() {
	_sizer->Add(_output, 0, wxEXPAND | wxTOP | wxBOTTOM | wxLEFT | wxRIGHT, 4);
	_sizer->Add(_gridSizer, 2, wxEXPAND);
}
void Window::AddToIncludes() {
	_includeList.Add("_");
	_includeList.Add("+");
	_includeList.Add("-");
	_includeList.Add("%");
	_includeList.Add("*");
	_includeList.Add("/");
	_includeList.Add("(");
	_includeList.Add(")");
	_includeList.Add("SIN");
	_includeList.Add("COS");
	_includeList.Add("TAN");
	_validator = new wxTextValidator(wxFILTER_NUMERIC | wxFILTER_INCLUDE_CHAR_LIST);
	_validator->SetIncludes(_includeList);
}