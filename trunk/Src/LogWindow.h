#include "wx/wx.h"

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