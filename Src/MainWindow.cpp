#include "MainWindow.h"
#include "DialogNewDL.h"
//#include "wx/generic/dirctrlg.h"
#include "../lib/wxvcl/inifiles.h"
#include "FileDownloader.h"
#include "ArgumentParser.h"

//BEGIN_DECLARE_EVENT_TYPES()
	//DECLARE_EVENT_TYPE(wxEVT_COPYDATA,WM_COPYDATA)
//END_DECLARE_EVENT_TYPES()

BEGIN_EVENT_TABLE(MainWindow, wxFrame)
	//EVT_CUSTOM(WM_COPYDATA,wxID_ANY,MainWindow::OnCopyData)
	EVT_CLOSE(MainWindow::OnClose)
	EVT_ICONIZE(MainWindow::OnIconize)
	EVT_TOOL(wxID_ANY, MainWindow::OnLeftClick)
	//EVT_IDLE(MainWindow::OnIdle)
	//EVT_TIMER(wxID_ANY,MainWindow::OnTimer)
	 EVT_TIMER(wxID_ANY,MainWindow::OnTimer)
END_EVENT_TABLE()

MainWindow::MainWindow(wxFrame *frame,Config *config/*, const wxString& title, int x, int y, int w, int h*/)
 : wxFrame(frame, wxID_ANY, wxString(TITLE), wxPoint(10, 10), wxSize(640,480))
{
	wxColour col = wxColour(117,174,255);
	//SetBackgroundColour(col);
	//this->SetBackgroundColor(wxColor(0,0,0));
	// Création de la fenetre de log

	LogWin = new LogWindow(this,wxString("Log Window"),10,10,300,200);
//#ifdef DEBUG
	if (config->ReadIntValue("LogWindow",0))
		LogWin->Show();
//#endif

	mConfig = config;

	SetSize(config->ReadIntValue("Width",800),config->ReadIntValue("Height",600));

	if (config->ReadIntValue("FullScreen",0))
		Maximize();

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
		
	wxToolBarToolBase *tool = mToolBar->AddTool(ID_ADD,ADDDOWNLOAD,wxBitmap("IDB_ADD"));
	//tool->Enable(false);
	mToolBar->AddTool(ID_START,STARTDOWNLOAD,wxBitmap("IDB_START"));
	mToolBar->AddTool(ID_STOP,STOPDOWNLOAD,wxBitmap("IDB_STOP"));
	mToolBar->AddTool(ID_DELETE,DELETEDOWNLOAD,wxBitmap("IDB_DELETE"));

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
	//bookStyle |= wxFNB_CUSTOM_DLG;
	//bookStyle |= wxFNB_SMART_TABS;	
	mList=NULL;

	book = new wxFlatNotebook(mSplitter, wxID_ANY, wxDefaultPosition, wxSize(300, 400), bookStyle);
	if (book)
	{
		//book->SetBackgroundColour(col);
		//book->SetCustomizeOptions(wxFNB_CUSTOM_TAB_LOOK | wxFNB_CUSTOM_LOCAL_DRAG | wxFNB_CUSTOM_FOREIGN_DRAG);

		//book->SetBackgroundColour(wxColour(89,177,255));
		//book->SetGradientColorFrom(wxColour(89,177,255));
		//book->SetGradientColorTo(wxColour(89,177,255));
		book->SetGradientColorBorder(wxColour(89,177,255));

		mList = new wxListCtrl(book,wxID_ANY,wxDefaultPosition,book->GetPageBestSize(),/*wxLC_SINGLE_SEL |*/ wxLC_REPORT);
		mList->InsertColumn(0,FILENAME);
		mList->SetColumnWidth(0,mConfig->ReadIntValue("ColumnFilenameWidth",200));
		mList->InsertColumn(1,SIZE);
		mList->SetColumnWidth(1,mConfig->ReadIntValue("ColumnSizeWidth",100));
		mList->InsertColumn(2,DOWNLOADED);
		mList->SetColumnWidth(2,mConfig->ReadIntValue("ColumnDownloadedWidth",100));
		mList->InsertColumn(3,wxString("%"));
		mList->SetColumnWidth(3,mConfig->ReadIntValue("ColumnPercentageWidth",50));
		mList->InsertColumn(4,STATUS);
		mList->SetColumnWidth(4,mConfig->ReadIntValue("ColumnStatusWidth",150));
		mList->InsertColumn(5,SPEED);
		mList->SetColumnWidth(5,mConfig->ReadIntValue("ColumnSpeedWidth",75));
		mList->InsertColumn(6,TIMEREMAINING);
		mList->SetColumnWidth(6,mConfig->ReadIntValue("ColumnTimeRemaining",90));
		mList->InsertColumn(7,wxString("URL"));
		mList->SetColumnWidth(7,mConfig->ReadIntValue("ColumnURLWidth",200));

		wxImageList *imagelist = new wxImageList(16,16,true);
		imagelist->Add(wxBitmap("IDB_STARTED"));
		imagelist->Add(wxBitmap("IDB_STOPED"));
		imagelist->Add(wxBitmap("IDB_ERROR"));
		imagelist->Add(wxBitmap("IDB_WAIT"));
		imagelist->Add(wxBitmap("IDB_FINISH"));
		mList->SetImageList(imagelist,wxIMAGE_LIST_SMALL);

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

		bool result = book->AddPage(mList,DOWNLOADSLIST);
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


	mSplitter->SplitVertically(mGroup,book,1/*50*/);

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

void MainWindow::AttachTaskBarIcon(TaskBarIcon *icon)
{
	mTaskBarIcon = icon;
}

void MainWindow::ShowLog(bool show)
{
	if (show)
		LogWin->Show();
	else
		LogWin->Hide();
}

void MainWindow::AddDownload(wxString URL)
{
	DialogNewDL *dlg = new DialogNewDL(this);

	if (URL!="")
		dlg->SetLink(URL);

	if (dlg->ShowModal()==0)
	{
		if (mDLManager->AddDownload(dlg->GetLink(),dlg->GetFilename())==false)
		{
			//TODO : Message d'erreur à placer
		}
	}

	dlg->Destroy();
	delete dlg;
}

void MainWindow::StartDownload()
{
	for (int long i=0;i<mList->GetItemCount();i++)
	{
		if (mList->GetItemState(i,wxLIST_STATE_SELECTED))
		{
			mDLManager->StartDownload(mList->GetItemText(i));
			//break;
		}
	}
}

void MainWindow::StopDownload()
{
	for (int long i=0;i<mList->GetItemCount();i++)
	{
		if (mList->GetItemState(i,wxLIST_STATE_SELECTED))
		{
			mDLManager->StopDownload(mList->GetItemText(i),false);
		}
	}

	mDLManager->UpdateScreen(true);
}

void MainWindow::DeleteDownload()
{

	for (int long i=mList->GetItemCount()-1;i>=0;i--)
	{
		if (mList->GetItemState(i,wxLIST_STATE_SELECTED))
		{
			wxLogMessage("%d %s",i,mList->GetItemText(i));
			mDLManager->DeleteDownload(mList->GetItemText(i),false);
		}
	}
	mDLManager->UpdateScreen(true);
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

	/*if (mGauge)
	{
		mGauge->SetValue(mGauge->GetValue()+1);
		if (mGauge->GetValue()>mGauge->GetRange())
			mGauge->SetValue(0);
	}

	mGauge->Refresh();*/
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

	mConfig->WriteIntValue("ColumnFilenameWidth",mList->GetColumnWidth(0));
	mConfig->WriteIntValue("ColumnSizeWidth",mList->GetColumnWidth(1));
	mConfig->WriteIntValue("ColumnDownloadedWidth",mList->GetColumnWidth(2));
	mConfig->WriteIntValue("ColumnPercentageWidth",mList->GetColumnWidth(3));
	mConfig->WriteIntValue("ColumnStatusWidth",mList->GetColumnWidth(4));
	mConfig->WriteIntValue("ColumnSpeedWidth",mList->GetColumnWidth(5));
	mConfig->WriteIntValue("ColumnTimeRemaining",mList->GetColumnWidth(6));
	mConfig->WriteIntValue("ColumnURLWidth",mList->GetColumnWidth(7));

	mConfig->WriteIntValue("Width",GetSize().x);
	mConfig->WriteIntValue("Height",GetSize().y);
	mConfig->WriteIntValue("Fullscreen",IsMaximized());

	mConfig->Save();

	mTaskBarIcon->RemoveIcon();

	exit(0);
}

void MainWindow::OnIconize(wxIconizeEvent &event)
{                                                                                                      
	Show(false);
	LogWin->Show(false);
}

void MainWindow::OnCopyData(wxEvent &event)
{
	wxLogMessage("Copy Data Message");
}

WXLRESULT MainWindow::MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam)
{
	if ( nMsg == WM_COPYDATA )
	{
		COPYDATASTRUCT *data = (COPYDATASTRUCT*)lParam;
		wxLogMessage("Copy Data %s",data->lpData);
		//ArgumentParser *ArgParser = new ArgumentParser((char*)data->lpData);
		ArgumentParser *ArgParser = new ArgumentParser();
		ArgParser->AttachManager(mDLManager);
		//ArgumentParser::AttachManager(mDLManager);
		//while(Stack.Count()!=0);
		
		mDLManager->Stack.Add((char*)data->lpData);
	
		ArgParser->Parse();
		//ArgumentParser::Parse();
		//delete ArgParser;
		//return true;
	}

	return wxFrame::MSWWindowProc(nMsg,wParam,lParam);
}