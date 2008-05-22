#include "wx/wx.h"

#ifndef _LOGWINDOW_
#define _LOGWINDOW_

class LogWindow : public wxFrame
{
public:
	LogWindow(wxFrame *frame, const wxString& title, int x, int y, int w, int h);
	~LogWindow();
private:
	DECLARE_EVENT_TABLE()

	//wxPanel *mPanel;
	wxTextCtrl *mLogText;
	wxLogTextCtrl *mLogCtrl;
};

#endif