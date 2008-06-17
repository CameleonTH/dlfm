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
#include "LanguageManager.h"

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
INIT_LOC

BEGIN_SETUP_LOC
	ADD_CAPTION("AddDownload","Add download")
	ADD_CAPTION("StartDownload","Start download")
	ADD_CAPTION("StopDownload","Stop download")
	ADD_CAPTION("DeleteDownload","Delete download")
	ADD_CAPTION("DownloadsList","Downloads list")
	ADD_CAPTION("NewDownload","New download")
	ADD_CAPTION("Cancel","Cancel")
	ADD_CAPTION("TotalSpeed","Total speed")
	ADD_CAPTION("Filename","File name")
	ADD_CAPTION("Size","Size")
	ADD_CAPTION("Downloaded","Downloaded")
	ADD_CAPTION("Status","Status")
	ADD_CAPTION("Speed","Speed")
END_SETUP_LOC

//SETLANGUAGE("English");

bool MyApp::OnInit()
{
	//wxImage::AddHandler(new wxPNGHandler);

	Config *config = new Config("config.cfg");
	if (config==NULL)
	{
		//Message erreur
		exit(1);
	}
	config->Load();

	if (Language)
	{
		bool result = Language->LoadLanguage(config->ReadStringValue("Language","English"));
		Language->AvailableLanguage();
		//wxLogMessage("Loaded language : %d %s",result,config->ReadStringValue("Language","English"));
	}	

	MainWindow *main = new MainWindow(NULL,config);
	
	SetTopWindow(main);

	DLManager *manager = new DLManager(config);

	TaskBarIcon *TBIcon = new TaskBarIcon(main);
	
	if (TBIcon)
		main->AttachTaskBarIcon(TBIcon);

	if (manager)
	{
		main->AttachDLManager(manager);
		main->Show();
	}else
		exit(1);

	return true;
}
