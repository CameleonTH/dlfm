//#include <stdio.h>
#include <string.h>

//#include "wx/wxprec.h"
#include <wx\wx.h>
#include <wx\snglinst.h>
//#include <wx/thread.h>
#include <windows.h>

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
    virtual bool OnInit();
	virtual int OnExit();

	//LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam);
	//DECLARE_MESSAGE_MAP()

	wxSingleInstanceChecker *checker;
};

// Create a new application object
IMPLEMENT_APP(MyApp)
//IMPLEMENT_APP_CONSOLE(MyApp)

//INIT_LOC

/*BEGIN_MESSAGE_MAP(MyApp, wxApp)
  ON_MESSAGE(WM_COPYDATA, OnMyMessage)
END_MESSAGE_MAP()*/


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

	//wxInitializer initializer;

	const wxString name = wxString::Format("DLFM-%s", wxGetUserId().c_str());

	checker = new wxSingleInstanceChecker(name);
    if ( checker )
    {
        if ( checker->IsAnotherRunning() )
        {
            wxPrintf(_T("Another instance of the program is running, exiting.\n"));

			//wxWindow::MSWProcessMessage();
			//
			wxString Args;
			for (int i=1;i<argc;i++)
			{
				Args+="\"";
				Args+=argv[i];
				Args+="\" ";
			}

			COPYDATASTRUCT data;
			data.dwData = 100;
			data.lpData = (void*)Args.c_str();
			data.cbData = strlen(Args)+1;
			HWND handle = 0;
			handle = FindWindowEx(0,0, NULL,TITLE);
		
			if (handle > 0) 
				SendMessageA(handle, WM_COPYDATA, 0, (LPARAM)(&data));
				//SendMessage(HWND_BROADCAST, WM_COPYDATA, 0, (LPARAM)(&data));

			exit(0);
        }
	}else
		exit(2);

	Config *config = new Config("config.cfg");
	if (config==NULL)
	{
		//Message erreur
		exit(1);
	}
	config->Load();

	if (LAGNUAGEMANAGERVAR)
	{
		bool result = LAGNUAGEMANAGERVAR->LoadLanguage(config->ReadStringValue("Language","English"));
		wxArrayString strings = GETLANGUAGEAVAILABLE;

		//for (int i=0;i<strings.Count();i++)
			//wxLogMessage("%s",strings[i]);
		//wxLogMessage("Loaded language : %d %s",result,config->ReadStringValue("Language","English"));
	}	

	MainWindow *main = new MainWindow(NULL,config);
	
	SetTopWindow(main);

	DLManager *manager = new DLManager(config);

	TaskBarIcon *TBIcon = new TaskBarIcon(main,config);
	
	if (TBIcon)
	{
		main->AttachTaskBarIcon(TBIcon);
	}

	if (manager)
	{
		main->AttachDLManager(manager);
		main->Show();
	}else
		exit(1);

	return true;
}


int MyApp::OnExit()
{
	delete checker;
	return 0;
}