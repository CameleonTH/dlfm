#include "TaskBarIcon.h"

BEGIN_EVENT_TABLE(TaskBarIcon, wxTaskBarIcon)
	EVT_TASKBAR_LEFT_DCLICK(TaskBarIcon::OnLeftButtonDClick)
    /*EVT_MENU(PU_RESTORE, MyTaskBarIcon::OnMenuRestore)
    EVT_MENU(PU_EXIT,    MyTaskBarIcon::OnMenuExit)
    EVT_MENU(PU_NEW_ICON,MyTaskBarIcon::OnMenuSetNewIcon)
    EVT_MENU(PU_OLD_ICON,MyTaskBarIcon::OnMenuSetOldIcon)
    EVT_MENU(PU_CHECKMARK,MyTaskBarIcon::OnMenuCheckmark)
    EVT_UPDATE_UI(PU_CHECKMARK,MyTaskBarIcon::OnMenuUICheckmark)    
    EVT_MENU(PU_SUB1, MyTaskBarIcon::OnMenuSub)
    EVT_MENU(PU_SUB2, MyTaskBarIcon::OnMenuSub)*/
END_EVENT_TABLE()


TaskBarIcon::TaskBarIcon(wxFrame *frame)
{
	dialog = frame;
	SetIcon(wxIcon("IDI_ICON"),"DL.Free Manager");
}

void TaskBarIcon::OnLeftButtonDClick(wxTaskBarIconEvent& event)
{
	if (dialog)
	{
		dialog->Show(true);
		dialog->Restore();
		dialog->GetLogWindow()->Show(true);
		dialog->GetLogWindow()->Restore();
	}
}
