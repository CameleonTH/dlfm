#include <wx/wx.h>
#include "DLManager.h"


#ifndef _ARGUMENTPARSER_
#define _ARGUMENTPARSER_

class ArgumentParser
{
public:
	ArgumentParser(wxString Arguments);

	void AttachManager(DLManager *manager);

private:
	DLManager *Manager;
};

#endif