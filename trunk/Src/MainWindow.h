#include "wx/wx.h"
#include "wx/listctrl.h"
#include "wx/treectrl.h"
#include "wx/splitter.h"
#include "wx/toolbar.h"

#include "wxVistagauge.h"

#include "DLManager.h"
#include "Parser.h"

//#include "wx/aui/aui.h"

#include "wx/wxFlatNotebook/wxFlatNotebook.h"

class MainWindow : public wxFrame
{
public:
	MainWindow(wxFrame *frame, const wxString& title, int x, int y, int w, int h);
	~MainWindow();

	void AttachDLManager(DLManager *manager);

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

	wxPanel			*mPanel;
	wxTreeCtrlBase	*mGroup;

	wxPanel			*mPanelList;
	wxListCtrl		*mList;

	wxFlatNotebook	*book;

	wxToolBar		*mToolBar;

	wxSplitterWindow *mSplitter;

	DLManager		*mDLManager;

	wxVistagauge	*mGauge;

	long time;

	wxTimer			Timer;
};