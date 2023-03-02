#pragma once

#include <wx/wx.h>
#include <wx/wfstream.h>

#include <iostream>
#include <fstream>
#include <string>

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);

	void CreateMuneBar();

	void OnText(wxCommandEvent& evt);
	void OnTextSizeSlider(wxCommandEvent& evt);

	void OnResize(wxSizeEvent& evt);

	void OnNew(wxCommandEvent& evt);
	void OnOpen(wxCommandEvent& evt);
	void OnSave(wxCommandEvent& evt);
	void OnClose(wxCommandEvent& evt);

private:
	wxTextCtrl* text;
	wxString file_path;
	bool file_is_saved = false;
};
