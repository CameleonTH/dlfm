#include "wx/wx.h"
#include "wx/taskbar.h"

#include "Config.h"

#ifndef _TASKBARICON_
#define _TASKBARICON_

class MainWindow;

class TaskBarIcon : public wxTaskBarIcon
{
public:

    TaskBarIcon(wxFrame *frame,Config *config);

	void OnLeftButtonDClick(wxTaskBarIconEvent&);
    /*void OnMenuRestore(wxCommandEvent&);
    void OnMenuExit(wxCommandEvent&);
    void OnMenuSetNewIcon(wxCommandEvent&);
    void OnMenuSetOldIcon(wxCommandEvent&);
    void OnMenuCheckmark(wxCommandEvent&);
    void OnMenuUICheckmark(wxUpdateUIEvent&);
    void OnMenuSub(wxCommandEvent&);
    virtual wxMenu *CreatePopupMenu();*/

	DECLARE_EVENT_TABLE()
private:
	MainWindow *dialog;
	Config *mConfig;
};

#endif