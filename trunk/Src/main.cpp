//#include <stdio.h>
#include <string.h>

//#include "wx/wxprec.h"
#include <wx\wx.h>
//#include <wx/thread.h>

#include "MainWindow.h"
#include "DLManager.h"
#include "FreeFileDownloader.h"
#include "FileDownloader.h"
#include "Parser.h"

class MyApp : public wxApp
{
public:
    //MyApp();
    //virtual ~MyApp(){};

    virtual bool OnInit();
};

// Create a new application object
//DECLARE_APP(MyApp)
IMPLEMENT_APP(MyApp)
//IMPLEMENT_APP_CONSOLE(MyApp)


/*MyApp::MyApp()
{

}*/

bool MyApp::OnInit()
{
	wxImage::AddHandler(new wxPNGHandler);

	MainWindow *main = new MainWindow(NULL,wxString("DL.Free Manager 0.2 Alpha By CameleonTH"),10,10,640,480);
	
	SetTopWindow(main);

	DLManager *manager = new DLManager();

	if (manager)
	{
		main->AttachDLManager(manager);
		main->Show();
	}else
		exit(1);

	return true;
}
