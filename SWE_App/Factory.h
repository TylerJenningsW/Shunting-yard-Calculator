#pragma once
#include <unordered_map>
enum ids;
class wxGridSizer;
class Window;
static class Factory {
public:
	static std::unordered_map<ids, std::string> FillPairs(std::string idStrings[], Window* parent);
	static void CreateButtons(wxGridSizer* grid, Window* parent);
	static void CreateSizers(Window* parent);
	static void CreateTextCtrl(Window* parent, wxTextValidator* validator);
};