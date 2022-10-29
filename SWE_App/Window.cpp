
#include "Window.h"
#include "Factory.h"
#include "CalculatorProcessor.h"
#include "idlist.h"
#include "Helper.h"

Window::Window() : wxFrame(nullptr, ids::MAINWINDOW, "Calculator", wxDefaultPosition, wxSize(340, 540)) {
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
	SetMinSize(wxSize(340, 540));
	SetBackgroundColour(wxColour(0, 0, 0));
	
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

void Window::OnChar(wxKeyEvent& evt) {
	// Evaluate expression if enter key is pressed
	if (evt.GetKeyCode() == WXK_RETURN)	{
		CalculatorProcessor* _processor = CalculatorProcessor::GetInstance();
		_processor->ParseId(this, (ids)evt.GetId());
		return;
	}

	// Process all other keys as usual
	evt.Skip();
}

void Window::OnButtonHover(wxMouseEvent& evt) {
	wxEventType type = evt.GetEventType();
	wxButton* btn = (wxButton*)evt.GetEventObject();
	wxColour color = btn->GetBackgroundColour();
	size_t red = color.GetRed();
	size_t green = color.GetGreen();
	size_t blue = color.GetBlue();
	if (type == wxEVT_ENTER_WINDOW)	{

		red = truncate(red + 15);
		green = truncate(green + 15);
		blue = truncate(blue + 15);
		btn->SetOwnBackgroundColour(wxColour(red,green,blue));
		btn->Refresh();
	}
	else if (type == wxEVT_LEAVE_WINDOW) {
		red = truncate(red - 15);
		green = truncate(green - 15);
		blue = truncate(blue - 15);
		btn->SetOwnBackgroundColour(wxColour(red, green, blue));
		btn->Refresh();
	}

	evt.Skip();
}

 void Window::OnButtonUnHover(wxMouseEvent& evt) {
	wxButton* btn = (wxButton*)evt.GetEventObject();
	wxColour newColor = btn->GetBackgroundColour();
	btn->SetBackgroundColour(newColor.ChangeLightness(45));
	evt.Skip();
}

void Window::AddToSizer() {
	_sizer->Add(_output, 1, wxEXPAND | wxTOP | wxBOTTOM | wxLEFT | wxRIGHT);
	_sizer->Add(_gridSizer, 10, wxEXPAND);
}
void Window::AddToIncludes() {
	_includeList.Add("\n");
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
	_validator->SuppressBellOnError(true);
	_validator->SetIncludes(_includeList);
}