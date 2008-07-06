#include "ArgumentParser.h"
#include "MainWindow.h"

bool AlreadyRun=false;
DLManager *Manager=NULL;

void ArgumentParser::Init()
{
	//ArgumentParser::AlreadyRun=false;
	AlreadyRun=false;
	Manager=NULL;
}

/*ArgumentParser::ArgumentParser(wxString arguments)
{
	Arguments = arguments;
}*/

void ArgumentParser::AttachManager(DLManager *manager)
{
	Manager = manager;
}

void ArgumentParser::Parse()
{
	if (Manager==NULL)
		return;

	if (AlreadyRun==false)
	{
		AlreadyRun=true;
		wxString Before;
		wxString After;
		wxString Arguments;

		while(Manager->Stack.Count()>0)
		{
			//wxLogMessage("Stack %d",Manager->Stack.Count());
			Arguments = Manager->Stack[0];
			Before = Arguments.BeforeFirst('\r');
			After = Arguments.AfterFirst('\r');

			while (After!="" || Before!="")
			{
				wxLogMessage("Before %s",Before);
				Manager->mMain->AddDownload(Before);	
				
				Before = After.BeforeFirst('\r');
				After = After.AfterFirst('\r');		
			}
			Manager->Stack.Remove(Arguments);
		}

		AlreadyRun=false;
	}
}

//ArgumentParser::AlreadyRun = false;
//Init();