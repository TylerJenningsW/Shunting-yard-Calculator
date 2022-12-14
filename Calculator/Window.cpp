
#include "Window.h"
#include "Factory.h"
#include "CalculatorProcessor.h"
#include "idlist.h"
#include "Helper.h"
Window::Window(bool iconOn) : wxFrame(nullptr, ids::MAINWINDOW, "Calculator", wxDefaultPosition, wxSize(340, 540)) {
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
	if (iconOn == true) {
		wxInitAllImageHandlers();
		wxIcon icon{ "images/calc.png", wxBITMAP_TYPE_PNG, 32, 32 };
		this->SetIcon(icon);
	}
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

void Window::SetWindowGridSizer(wxFlexGridSizer* gridSizer) {
	_gridSizer = gridSizer;
}

void Window::SetTextControl(wxTextCtrl* textCtrl) {
	_output = textCtrl;
}

void Window::SetFontSize(int size)
{
	_fontSize = size;
}

void Window::OnButton(wxCommandEvent& evt) {
	// fetch singleton to parse event
	CalculatorProcessor* _processor = CalculatorProcessor::GetInstance();
	_processor->ParseId(this, (ids)evt.GetId());
	evt.Skip();
}

void Window::OnButtonClick(wxMouseEvent& evt) {
	wxEventType type = evt.GetEventType();
	wxButton* btn = (wxButton*)evt.GetEventObject();
	wxColour color = btn->GetBackgroundColour();
	size_t red = color.GetRed();
	size_t green = color.GetGreen();
	size_t blue = color.GetBlue();
	if (type == wxEVT_LEFT_DOWN) {
		IncreaseLight(btn, red, green, blue);
	}
	else if (type == wxEVT_LEFT_UP && btn->IsMouseInWindow()) {
		DecreaseLight(btn, red, green, blue);
	}
	else if (type == wxEVT_LEFT_UP) {
		btn->Thaw();
	}
	evt.Skip();
}

void Window::OnChar(wxKeyEvent& evt) {
	// Evaluate expression if enter key is pressed
	if (evt.GetKeyCode() == WXK_RETURN) {
		CalculatorProcessor* _processor = CalculatorProcessor::GetInstance();
		_processor->ParseId(this, (ids)evt.GetId());
		return;
	}

	// Process all other keys as usual
	evt.Skip();
}

void Window::SetFont(int fontsize) {

	wxFont font = wxFont(fontsize, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
	_font = font;
}

void Window::OnButtonHover(wxMouseEvent& evt) {
	wxEventType type = evt.GetEventType();
	wxButton* btn = (wxButton*)evt.GetEventObject();
	wxColour color = btn->GetBackgroundColour();
	size_t red = color.GetRed();
	size_t green = color.GetGreen();
	size_t blue = color.GetBlue();
	if (type == wxEVT_ENTER_WINDOW) {
		IncreaseLight(btn, red, green, blue);
	}
	else if (type == wxEVT_LEAVE_WINDOW && !btn->IsMouseInWindow() && !evt.LeftIsDown()) {
		DecreaseLight(btn, red, green, blue);
	}
	else if (type == wxEVT_LEAVE_WINDOW && !btn->IsMouseInWindow() && evt.LeftIsDown()) {
		DecreaseLight(btn, red, green, blue);
		DecreaseLight(btn, btn->GetBackgroundColour().GetRed(), btn->GetBackgroundColour().GetGreen(), btn->GetBackgroundColour().GetBlue());
		Update();
		btn->Freeze();
	}
	evt.Skip();
}

void Window::OnSize(wxSizeEvent& evt) {
	wxTextCtrl* txt = (wxTextCtrl*)evt.GetEventObject();
	wxSize size = txt->GetSize();
	int y = size.GetHeight();
	_fontSize = (y / 3) * 2;
	SetFont(_fontSize);
	txt->SetFont(_font);
	txt->SetMinSize(wxSize(324, 46));
	evt.Skip();
}

void Window::AddToSizer() {
	_sizer->Add(_output, 1, wxEXPAND | wxTOP | wxBOTTOM | wxLEFT | wxRIGHT);
	_sizer->Add(_gridSizer, 10, wxEXPAND);
}
void Window::AddToIncludes() {
	_includeList.Add("\n");
	_includeList.Add("^");
	_includeList.Add(".");
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

void Window::IncreaseLight(wxButton* button, size_t red, size_t green, size_t blue) {
	red = truncate(red + 15);
	green = truncate(green + 15);
	blue = truncate(blue + 15);
	button->SetOwnBackgroundColour(wxColour(red, green, blue));
	button->Refresh();
}

void Window::DecreaseLight(wxButton* button, size_t red, size_t green, size_t blue) {
	red = truncate(red - 15);
	green = truncate(green - 15);
	blue = truncate(blue - 15);
	button->SetOwnBackgroundColour(wxColour(red, green, blue));
	button->Refresh();
}
