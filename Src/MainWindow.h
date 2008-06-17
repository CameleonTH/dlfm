#include "wx/wx.h"
#include "wx/listctrl.h"
#include "wx/treectrl.h"
#include "wx/splitter.h"
#include "wx/toolbar.h"

#include "wxVistagauge.h"

#include "DLManager.h"
#include "Parser.h"
#include "LogWindow.h"
#include "Config.h"
#include "TaskBarIcon.h"
#include "error.h"

//#include "wx/aui/aui.h"

#include "wx/wxFlatNotebook/wxFlatNotebook.h"

#ifndef _MAINWINDOW_
#define _MAINWINDOW_

class MainWindow : public wxFrame
{
public:
	MainWindow(wxFrame *frame, Config *config/*, const wxString& title, int x, int y, int w, int h*/);
	~MainWindow();

	void AttachDLManager(DLManager *manager);
	void AttachTaskBarIcon(TaskBarIcon *icon);

	inline wxListCtrl *GetListCtrl() { return mList; }
	inline wxFlatNotebook *GetNotebook() { return book; }

	void AddDownload();
	void StartDownload();
	void StopDownload();
	void DeleteDownload();

	void OnLeftClick(wxCommandEvent &event);
	void OnIdle(wxIdleEvent &event);
	void OnTimer(wxTimerEvent &event);
	void OnClose(wxCloseEvent &event);
	void OnIconize(wxIconizeEvent &event);

	void ShowLog(bool show=true);
	
	wxFrame *GetLogWindow() { return LogWin; }

	enum
	{
		ID_ADD,
		ID_START,
		ID_STOP,
		ID_DELETE,
		ID_TIMER,
		ID_MAX
	};
private:
	DECLARE_EVENT_TABLE()

	TaskBarIcon		*mTaskBarIcon;

	Config			*mConfig;

	LogWindow		*LogWin;

	wxPanel			*mPanel;
	wxTreeCtrlBase	*mGroup;

	wxPanel			*mPanelList;
	wxListCtrl		*mList;

	wxFlatNotebook	*book;

	wxToolBar		*mToolBar;

	wxSplitterWindow *mSplitter;

	DLManager		*mDLManager;

	wxVistaGauge	*mGauge;

	long time;

	wxTimer			Timer;
};

#endif