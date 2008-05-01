#include "wx/wx.h"
#ifndef _PARSER_
#define _PARSER_

class Parser
{
public:
	static bool IsValidLink(wxString link);
	static bool IsFreeLink(wxString link);

	static long GetFileSizeHTTP(wxString header);

	static wxString GetFilenameFromURL(wxString url);

	static wxString FirstLine(wxString text);
};

#endif