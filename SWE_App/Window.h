#pragma once
#include "wx/wx.h"
#include <string>
#include <unordered_map>
// GUI
enum ids;
class Factory;
class Window : public wxFrame {
public:
	Window();
    ~Window();
    wxTextCtrl* GetOutput();
    std::unordered_map<ids, std::string> GetPairs();
    void SetWindowSizer(wxBoxSizer* sizer);
    void SetWindowGridSizer(wxGridSizer* gridSizer);
    void SetTextControl(wxTextCtrl* textCtrl);
    void OnButtonClick(wxCommandEvent& evt);
private:
    wxBoxSizer* _sizer = nullptr;
    wxGridSizer* _gridSizer = nullptr;
    std::string _currNumberStr = "";
    wxString _data = "";
    wxTextValidator* _validator = nullptr;
    wxArrayString _includeList;
    wxTextCtrl* _output = nullptr;
    std::unordered_map<ids, std::string> _btnPairs{};
    void AddToSizer();
    void AddToIncludes();
};

