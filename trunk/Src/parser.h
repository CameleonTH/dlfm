#include "wx/wx.h"
#include <curl\curl.h>

#ifndef _PARSER_
#define _PARSER_

class Parser
{
public:
	static bool IsValidLink(wxString link);
	static bool IsFreeLink(wxString link);

	static curl_off_t GetFileSizeHTTP(wxString header);

	static wxString GetFilenameFromURL(wxString url);

	static wxString FirstLine(wxString text);
};

#endif