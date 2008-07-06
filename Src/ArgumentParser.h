#include <wx/wx.h>
#include "DLManager.h"


#ifndef _ARGUMENTPARSER_
#define _ARGUMENTPARSER_

class ArgumentParser
{
public:
	//ArgumentParser(wxString arguments);

	void AttachManager(DLManager *manager);

	void Parse();
	void Init();

	
//private:
protected:
	//static bool AlreadyRun;
	//static DLManager *Manager;
	//wxString Arguments;
};

//extern bool AlreadyRun;
//extern DLManager *Manager;

#endif