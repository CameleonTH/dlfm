#include "MainWindow.h"
#include "DialogNewDL.h"
//#include "wx/generic/dirctrlg.h"
#include "../lib/wxvcl/inifiles.h"
#include "FileDownloader.h"

BEGIN_EVENT_TABLE(MainWindow, wxFrame)
	EVT_CLOSE(MainWindow::OnClose)
	EVT_TOOL(wxID_ANY, MainWindow::OnLeftClick)
	//EVT_IDLE(MainWindow::OnIdle)
	//EVT_TIMER(wxID_ANY,MainWindow::OnTimer)
	 EVT_TIMER(wxID_ANY,MainWindow::OnTimer)
END_EVENT_TABLE()

MainWindow::MainWindow(wxFrame *frame, const wxString& title, int x, int y, int w, int h)
 : wxFrame(frame, wxID_ANY, title, wxPoint(x, y), wxSize(w, h))
{
	wxColour col = wxColour(117,174,255);
	//SetBackgroundColour(col);
	//this->SetBackgroundColor(wxColor(0,0,0));
	// Création de la fenetre de log

	LogWin = new LogWindow(this,wxString("Log Window"),10,10,300,200);
//#ifdef DEBUG
	//LogWin->Show();
//#endif

	SetSize(640,480);
	//mPanel = new wxPanel(this);

	mSplitter = new wxSplitterWindow(this);
	//mSplitter->SetBackgroundColour(col);

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(mainSizer);

	mainSizer->Add(mSplitter, 1, wxEXPAND);

	//mainSizer->Add(mToolBar, 0, wxEXPAND);

	mToolBar = new wxToolBar(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxTB_HORIZONTAL | wxTB_FLAT | wxTB_TEXT);
	//wxTB_HORIZONTAL
	//mToolBar->SetBackgroundColour(col);

	mToolBar->SetMargins(4, 4);
	mToolBar->SetToolBitmapSize(wxSize(48,48));
		
	wxToolBarToolBase *tool = mToolBar->AddTool(ID_ADD,wxString("Add"),wxBitmap("IDB_ADD"));
	//tool->Enable(false);
	mToolBar->AddTool(ID_START,wxString("Start"),wxBitmap("IDB_START"));
	mToolBar->AddTool(ID_STOP,wxString("Stop"),wxBitmap("IDB_STOP"));
	mToolBar->AddTool(ID_DELETE,wxString("Delete"),wxBitmap("IDB_DELETE"));

	//.EnableTool(0,false)
	//mToolBar->AddRadioTool(wxID_ANY,wxString("Test"),NULL);
	//mToolBar
	

	mToolBar->Realize();

	SetToolBar(mToolBar);

	mGroup = new wxTreeCtrl(mSplitter);
	//wxTR_HIDE_ROOT | wxTR_SINGLE

	wxTreeItemId Parent = mGroup->InsertItem(mGroup,NULL,wxString("Manager"));

	//mPanelList = new wxPanel(NULL);
	//mPanelList->SetBackgroundColour(wxColour(255,0,0));

	//mList->setst

	//mGroup->SetItemImage(item,NULL);

	//mGroup->InsertItem(item,NULL,wxString("Group1"));
	/*wxTreeItemId main = mGroup->InsertItem(Parent,NULL,wxString("En téléchargement"));
	mGroup->InsertItem(main,NULL,wxString("Group1"));

	mGroup->InsertItem(Parent,main,wxString("Finish"));

	mGroup->ExpandAll();*/

	//wxListItem itemCol;
    //itemCol.SetText(_T("Column 1"));
    //itemCol.SetImage(-1);
    //mGroup->InsertColumn(0, itemCol);
	//mGroup->InsertItem(0,wxString("Download"));
	//mGroup->InsertItem(1,wxString("Finish"));


	//mGroup->SetItem(1,0,wxString("Main"));
	//mGroup->AddChild(wxListItem());
	//mGroup->GetItem(

	bool m_bVC71Style=0;

	long bookStyle = 0; 
	m_bVC71Style ? bookStyle |= wxFNB_VC71 : bookStyle |= wxFNB_VC8;
	//bookStyle |= wxFNB_FANCY_TABS;
	//bookStyle |= wxFNB_TABS_BORDER_SIMPLE;
	//bookStyle |= wxFNB_FF2;
	//bookStyle |= wxFNB_X_ON_TAB;
	bookStyle |= wxFNB_NO_X_BUTTON;
	//bookStyle |= wxFNB_BACKGROUND_GRADIENT;
	bookStyle |= wxFNB_COLORFUL_TABS;
	bookStyle |= wxFNB_NODRAG;
	bookStyle |= wxFNB_CUSTOM_DLG;
	//bookStyle |= wxFNB_SMART_TABS;	

	book = new wxFlatNotebook(mSplitter, wxID_ANY, wxDefaultPosition, wxSize(300, 400), bookStyle);
	if (book)
	{
		//book->SetBackgroundColour(col);
		book->SetCustomizeOptions(wxFNB_CUSTOM_TAB_LOOK | wxFNB_CUSTOM_LOCAL_DRAG | wxFNB_CUSTOM_FOREIGN_DRAG);

		//book->SetBackgroundColour(wxColour(89,177,255));
		//book->SetGradientColorFrom(wxColour(89,177,255));
		//book->SetGradientColorTo(wxColour(89,177,255));
		book->SetGradientColorBorder(wxColour(89,177,255));

		mList = new wxListCtrl(book,wxID_ANY,wxDefaultPosition,book->GetPageBestSize(),wxLC_SINGLE_SEL |wxLC_REPORT);
		mList->InsertColumn(0,wxString("Filename"));
		mList->InsertColumn(1,wxString("Size"));
		mList->InsertColumn(2,wxString("Downloaded"));
		mList->InsertColumn(3,wxString("%"));
		mList->InsertColumn(4,wxString("Status"));
		mList->InsertColumn(5,wxString("Speed"));
		mList->InsertColumn(6,wxString("URL"));
		
		/*long tmp = mList->InsertItem(0,wxString("test"));
		wxLogMessage("id : %d",tmp);
		long tmp2 = mList->InsertItem(0,wxString("test2"));
		wxLogMessage("id : %d",tmp2);
		long tmp3 = mList->InsertItem(0,wxString("test3"));
		wxLogMessage("id : %d",tmp3);

		//mList->DeleteItem(tmp2);
		wxLogMessage(mList->GetItemText(tmp2));*/
		/*mList->SetItem(tmp,1,"123");
		mList->SetItem(tmp,2,"96");
		mList->SetItem(tmp,3,"75%");
		mList->SetItem(tmp,4,"http://test");*/
		//wxListItem *nouveau = new wxListItem();

		//mDLManager->mMain = this;

		bool result = book->AddPage(mList,wxString("Downloads List"));
		//wxWindow(
		//wxLogWarning("%d",result);
		//result = book->AddPage(new wxPanel(book),wxString("Options"));
		//wxLogWarning("%d",result);
		wxPanel *finalPanel = new wxPanel(book);
		
			//result = book->AddPage(finalPanel,wxString("Test"));
			//wxLogWarning("%d",result);

			//mGauge = new wxVistagauge(finalPanel,wxID_ANY,100,wxDefaultPosition,wxSize(250,18));
			//mGauge->SetValue(300);
			mGauge=NULL;
	}


	mSplitter->SplitVertically(mGroup,book,150);

	CreateStatusBar();

	Center();

	time = GetTickCount();
	Timer.SetOwner(this,ID_TIMER);
	Timer.Start(1000);
}

MainWindow::~MainWindow()
{
}

void MainWindow::AttachDLManager(DLManager *manager)
{
	mDLManager = manager;
	manager->mMain=this;
	mDLManager->UpdateScreen(true);
}


void MainWindow::AddDownload()
{
	DialogNewDL *dlg = new DialogNewDL(this);
	if (dlg->ShowModal()==0)
	{
		mDLManager->AddDownload(dlg->GetLink(),dlg->GetFilename());
	}

	dlg->Destroy();
	delete dlg;
}

void MainWindow::StartDownload()
{
	long selected = -1;
	for (int long i=0;i<mList->GetItemCount();i++)
	{
		if (mList->GetItemState(i,wxLIST_STATE_SELECTED))
		{
			selected=i;
			break;
		}
	}
	if (selected!=-1)
	{
		wxLogDebug(mList->GetItemText(selected));
		mDLManager->StartDownload(mList->GetItemText(selected));
	}
}

void MainWindow::StopDownload()
{
	long selected = -1;
	for (int long i=0;i<mList->GetItemCount();i++)
	{
		if (mList->GetItemState(i,wxLIST_STATE_SELECTED))
		{
			selected=i;
			break;
		}
	}
	if (selected!=-1)
	{
		wxLogDebug(mList->GetItemText(selected));
		mDLManager->StopDownload(mList->GetItemText(selected));
	}
}

void MainWindow::DeleteDownload()
{
	long selected = -1;
	for (int long i=0;i<mList->GetItemCount();i++)
	{
		if (mList->GetItemState(i,wxLIST_STATE_SELECTED))
		{
			selected=i;
			break;
		}
	}
	if (selected!=-1)
	{
		wxLogDebug(mList->GetItemText(selected));
		mDLManager->DeleteDownload(mList->GetItemText(selected));
	}
}

void MainWindow::OnLeftClick(wxCommandEvent& event)
{
	switch (event.GetId())
	{
	case ID_ADD:
		AddDownload();
		break;
	case ID_START:
		StartDownload();
		break;
	case ID_STOP:
		StopDownload();
		break;
	case ID_DELETE:
		DeleteDownload();
		break;
	}
}
//MainWindow

void MainWindow::OnIdle(wxIdleEvent &event)
{
}

void MainWindow::OnTimer(wxTimerEvent &event)
{
	mDLManager->UpdateScreen();

	if (mGauge)
	{
		mGauge->SetValue(mGauge->GetValue()+1);
		if (mGauge->GetValue()>mGauge->GetRange())
			mGauge->SetValue(0);
	}

	mGauge->Refresh();
}

void MainWindow::OnClose(wxCloseEvent &event)
{
	//Save File
	FileDownloader *temp;
	for (int i=0;i<mDLManager->List.Count();i++)
	{
		FileDownloader *temp = (FileDownloader*)mDLManager->List[i];	
		temp->StopDownload();
	}

	mDLManager->SaveDownloads();

	exit(0);
}