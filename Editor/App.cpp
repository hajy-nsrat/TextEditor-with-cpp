#include "App.h"
#include "MainFrame.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    MainFrame* mainframe = new MainFrame("HJ Editor");
    // Hi
    mainframe->SetClientSize(800, 500);
    mainframe->Center();
    mainframe->Show();
	mainframe->SetIcon(wxIcon("editor-icon.xpm"));
    return true;
}

/*
* color palet
	Hex 	RGB
	#007acc (0,122,204)
	#3e3e42 (62,62,66)
	#2d2d30 (45,45,48)
	#252526 (37,37,38)
	#1e1e1e (30,30,30)
*/