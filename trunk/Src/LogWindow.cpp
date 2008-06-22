#include "LogWindow.h"

BEGIN_EVENT_TABLE(LogWindow, wxFrame)
	EVT_CLOSE(LogWindow::OnClose)
END_EVENT_TABLE()


LogWindow::LogWindow(wxFrame *frame, const wxString& title, int x, int y, int w, int h)
 : wxFrame(frame, wxID_ANY, title, wxPoint(x, y), wxSize(w, h))
{
	mLogText = new wxTextCtrl(this,wxID_ANY,wxEmptyString,
                                 wxDefaultPosition, wxDefaultSize,
                                 wxTE_MULTILINE | wxSUNKEN_BORDER);

	mLogCtrl = new wxLogTextCtrl(mLogText);

	wxLog::SetActiveTarget(mLogCtrl);

	wxLogMessage("Log Initialiser");
}

LogWindow::~LogWindow()
{
	wxLog::SetActiveTarget(NULL);
}

void LogWindow::OnClose(wxCloseEvent &event)
{
	Hide();
}