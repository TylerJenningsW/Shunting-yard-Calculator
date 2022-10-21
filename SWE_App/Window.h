#pragma once
#include "wx/wx.h"
#include <string>
#include <unordered_map>
// GUI
enum ids;
class Window : public wxFrame {
    //double _currNumber = NULL;
    //double _otherNumber = NULL;
    //double _result = NULL;
    //bool operating = false;
    //int _currOp = NULL;
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

