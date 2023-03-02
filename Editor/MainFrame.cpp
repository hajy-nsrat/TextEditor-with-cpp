#include "MainFrame.h"


MainFrame::MainFrame(const wxString& title) 
	: wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize) 
{
	CreateMuneBar(); // this is my function

 	/* Status bar */
	auto status = CreateStatusBar(2);
	status->SetBackgroundColour(wxColor(62, 62, 66));

	this->SetBackgroundColour(wxColor(62, 62, 66));

	//this->SetIcon(wxIcon(wxT("mondrian")));
	//SetIcon(wxIcon(wxT("editor-icon.xpm")));

	// Create panels
	wxPanel* left_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(250, 250));
	left_panel->SetBackgroundColour(wxColor(37, 37, 38));
	wxPanel* right_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(250, 250));
	right_panel->SetBackgroundColour(wxColor(100, 200, 100));
	

	/* Text Area */
	text = new wxTextCtrl(right_panel, wxID_ANY, "", wxDefaultPosition, wxSize(800, 500), wxTE_MULTILINE);
	text->SetBackgroundColour(wxColor(30, 30, 30));
	text->SetForegroundColour(wxColor(255, 255, 255));
	wxFont font(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	text->SetFont(font);
	text->Bind(wxEVT_TEXT, &MainFrame::OnText, this);

	/* Slider for changing the text size */
	auto text_size_slider = new wxSlider(status, wxID_ANY, 14, 10, 50, wxDefaultPosition, wxSize(250, 20));
	text_size_slider->Bind(wxEVT_SLIDER, &MainFrame::OnTextSizeSlider, this);
	
	//
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL); // wxVERTICAL
	sizer->Add(left_panel, 0, wxEXPAND | wxALL, 5);
	sizer->Add(right_panel, 1, wxEXPAND | wxALL, 5);

	this->SetSizerAndFit(sizer);

	// here we change the text area size with the window size
	this->Bind(wxEVT_SIZE, &MainFrame::OnResize, this);
}

void MainFrame::CreateMuneBar()
{
	auto menubar   = new wxMenuBar();
	auto menu_file = new wxMenu();

	menu_file->Append(wxID_OPEN, "Open File...\tctrl+O");
	menu_file->Append(wxID_NEW,  "New File...\tctrl+N");
	menu_file->AppendSeparator();
	menu_file->Append(wxID_SAVE, "Save File...\tctrl+S");
	menu_file->AppendSeparator();
	menu_file->Append(wxID_EXIT, "Exit");

	Bind(wxEVT_MENU, &MainFrame::OnNew,   this, wxID_NEW);
	Bind(wxEVT_MENU, &MainFrame::OnOpen,  this, wxID_OPEN);
	Bind(wxEVT_MENU, &MainFrame::OnSave,  this, wxID_SAVE);
	Bind(wxEVT_MENU, &MainFrame::OnClose, this, wxID_EXIT);

	menubar->Append(menu_file, "&File");
	SetMenuBar(menubar);
}

void MainFrame::OnText(wxCommandEvent& evt) {
	//wxLogStatus("typing");
	file_is_saved = false;
	SetStatusText("Typing", 1);
}

void MainFrame::OnTextSizeSlider(wxCommandEvent& evt) 
{
	int16_t s = evt.GetInt();
	wxFont font(s, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	text->SetFont(font);
	auto tt = wxString::Format("changing text size=%d", s);
	SetStatusText(tt, 1);
}

void MainFrame::OnResize(wxSizeEvent& evt)
{
	SetStatusText("Resizing", 1);
	text->SetSize(GetClientSize().x - 270, GetClientSize().y - 10);
	Refresh();
	evt.Skip();
}

void MainFrame::OnNew(wxCommandEvent& evt) 
{
	if (file_is_saved) {
		text->Clear();
		file_path.clear();
		SetStatusText("New File", 1);
	} else {
		OnSave(evt);
	}
}

void MainFrame::OnOpen(wxCommandEvent& evt)
{
	text->Clear();
	wxFileDialog openFileDialog(this, _("Open XYZ file"), 
		"", "", "", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() != wxID_OK) return;

	wxFileInputStream input_stream(openFileDialog.GetPath());
	if (!input_stream.IsOk()) {
		wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
		return;
	}
	
	// file path is initilaized in class private
	file_path = openFileDialog.GetPath();
	std::fstream file(file_path.ToStdString(), std::ios::in);
	std::string line;
	while (!file.eof()) {
		std::getline(file, line);
		text->AppendText(line + '\n');
	}

	if (!file_is_saved) {
		OnSave(evt);
	}
	file_is_saved = false;
	SetStatusText("Open File", 1);
}

void MainFrame::OnSave(wxCommandEvent& evt) 
{
	if (file_path.empty()) {
		wxFileDialog SaveDialog(this,
			_T("Save file dialog"),
			wxEmptyString,
			_T("text.txt"),
			_T("Text files (*.txt)|*.txt"),
			wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
		SaveDialog.ShowModal();
		file_path = SaveDialog.GetPath();
	}
	std::fstream file(file_path.ToStdString(), std::ios::out);
	std::string tt = text->GetValue().ToStdString();
	file << tt;
	file_is_saved = true;
	SetStatusText("Save File", 1);
}

void MainFrame::OnClose(wxCommandEvent& evt)
{
	this->Close(true);
}
