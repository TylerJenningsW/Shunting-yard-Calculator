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
    void SetWindowGridSizer(wxFlexGridSizer* gridSizer);
    void SetTextControl(wxTextCtrl* textCtrl);
    void SetFontSize(int size);
    void SetFont(int fontsize);
    void OnButtonClick(wxCommandEvent& evt);
    void OnButtonHover(wxMouseEvent& evt);
    void OnChar(wxKeyEvent& evt);
    void OnSize(wxSizeEvent& evt);
private:
    int _fontSize = 24;
    wxFont _font;
    wxBoxSizer* _sizer = nullptr;
    wxFlexGridSizer* _gridSizer = nullptr;
    std::string _currNumberStr = "";
    wxString _data = "";
    wxTextValidator* _validator = nullptr;
    wxArrayString _includeList;
    wxTextCtrl* _output = nullptr;
    std::unordered_map<ids, std::string> _btnPairs{};
    wxColour original;
    void AddToSizer();
    void AddToIncludes();
};