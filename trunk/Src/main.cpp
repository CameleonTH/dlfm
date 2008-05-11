//#include <stdio.h>
#include <string.h>

//#include "wx/wxprec.h"
#include <wx\wx.h>
//#include <wx/thread.h>

#include "MainWindow.h"
#include "TaskBarIcon.h"
#include "DLManager.h"
#include "FreeFileDownloader.h"
#include "FileDownloader.h"
#include "Parser.h"
#include "Config.h"

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

	Config *config = new Config("config.cfg");
	config->Load();
	MainWindow *main = new MainWindow(NULL,config);
	
	SetTopWindow(main);

	DLManager *manager = new DLManager(config);

	TaskBarIcon *TBIcon = new TaskBarIcon(main);

	if (manager)
	{
		main->AttachDLManager(manager);
		main->Show();
	}else
		exit(1);

	return true;
}
