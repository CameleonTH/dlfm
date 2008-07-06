#include <curl\curl.h>
#include <wx/listctrl.h>

#include "DLManager.h"
#include "FileDownloader.h"
#include "FreeFileDownloader.h"
#include "MainWindow.h"
#include "FilePage.h"
#include "error.h"

DLManager::DLManager(Config *config)
{
	wxLogMessage("Download Manager Initialisation");

	mConfig = config;
	//mConfig=NULL;
	curl_global_init(CURL_GLOBAL_ALL);

	mMain=NULL;
	List.Clear();

	LoadDownloads();

	if (mConfig)
	{
		MaxRetry = mConfig->ReadIntValue("MaxRetry",3);
		RetryTime = mConfig->ReadIntValue("RetryTime",90);
		MaxDL = mConfig->ReadIntValue("MaxDL",3);

		if (mConfig->ReadIntValue("StartDownload",0))
		{
			int DLCount=0;
			for (int i=List.Count()-1;i>=0;i--)
			{
				if ( List[i]->GetStatus()!=FileDownloader::FFD_FINISH)
				{
					if (DLCount<MaxDL)
					{
						List[i]->StartDownload();
						DLCount++;
					}else
						List[i]->SetStatus(FileDownloader::FFD_WAIT);
				}
			}
		}
	}

	UpdateScreen(true);


	Time = GetTickCount();
}

DLManager::~DLManager()
{
}

bool DLManager::AddDownload(wxString link,wxString filename)
{
	if (!link || !filename)
		return false;

	FileDownloader *temp;
	bool exist=false;
	for (int i=0;i<List.Count();i++)
	{
		temp = (FileDownloader*)List[i];
		
		if (temp!=NULL && (temp->GetLink()==link || temp->GetFilename()==filename))
		{
			exist=true;
			break;
		}
	}

	wxLogDebug("Exist : %d",exist);

	if (exist)
		return false;

	FileDownloader *download=NULL;
	if (Parser::IsFreeLink(link))
		download = new FreeFileDownloader(link,filename,this);
	else
		download = new FileDownloader(link,filename,this);
	
	if (!download)
		return false;

	if (/*download->GetStatus()==FileDownloader::FFD_ERROR ||*/ download->IsOK()==false)
	{
		delete download;
		return false;
	}

	//wxLogMessage("adr %d",download);
	//wxLogMessage("%s",download->GetLink());
	
	List.Add(download);

	mMain->GetNotebook()->InsertPage(1,new FilePage(mMain->GetNotebook()),download->GetFilename());

	//download->SetItemId(itemId);
	//download->StartDownload();
	UpdateScreen(true);

	return true;
}

void DLManager::UpdateScreen(bool force)
{
	if (GetTickCount()-Time<1000 && force==false)
		return;

	//wxLogMessage("Language %s",STARTDOWNLOAD);
	int DLCount=0;		 

	for (int i=0;i<List.Count();i++)
	{
		FileDownloader *temp = (FileDownloader*)List[i];
		if (temp)
			if (temp->GetStatus()==FileDownloader::FFD_RETRY)
			{
				if (GetTickCount()-temp->GetErrorTime()>=RetryTime*1000)
				{
					wxLogMessage("Retry %d %d",temp->RetryCount,GetTickCount()-temp->GetErrorTime());
					temp->RetryCount++;
					StartDownload(temp,false,true);					
				}
			}			 
	}

	for (int i=0;i<List.Count();i++)
	{
		FileDownloader *temp = (FileDownloader*)List[i];
		if (temp)
			if (temp->GetStatus()==FileDownloader::FFD_START || temp->GetStatus()==FileDownloader::FFD_RETRY)
			{
				DLCount++;
			}			 
	}

	if (DLCount<MaxDL)
		for (int i=List.Count()-1;i>=0;i--)
		{
			FileDownloader *temp = (FileDownloader*)List[i];
			if (temp)
				if (temp->GetStatus()==FileDownloader::FFD_WAIT 
				/*|| temp->GetStatus()==FileDownloader::FFD_RETRY*/)
				{
					if (temp->IsRunning()==false)
					{
						temp->StartDownload();
						DLCount++;
						if (DLCount>=MaxDL)
							break;
					}
				}			 
		}

	Time = GetTickCount();
	long item;
	bool grey=false;
	wxString text;
	//wxLogDebug("UdateScreen");
	if (mMain && mMain->GetListCtrl()!=NULL)
	{
		//wxLogDebug("UdateScreen2");
		int selected = -1/*mMain->GetListCtrl()->GetTopItem()*/;

		//wxLogMessage("Count %d %d",mMain->GetListCtrl()->GetItemCount(),List.Count());
		if (mMain->GetListCtrl()->GetItemCount()==List.Count()/*-1*/)
		{ // Update
			for (int long i=1;i<=List.Count();i++)
			{

			}
			for (int long i=0;i<mMain->GetListCtrl()->GetItemCount();i++)
			{

			}
		}else{ // Delete all list link and update
			mMain->GetListCtrl()->DeleteAllItems();

			wxLogDebug("Count %d %d",mMain->GetNotebook()->GetPageCount(),List.Count());
			bool Deleted=false;
			if (mMain->GetNotebook()->GetPageCount()!=List.Count()+1)
			{
				Deleted=true;
				wxLogDebug("Delete pages");
				for (;1<mMain->GetNotebook()->GetPageCount();)
					mMain->GetNotebook()->DeletePage(1);
			}
		}


		//return;
		long TotalSpeed=0;
		for (int i=0 ; i<List.Count();i++)
		{
			FileDownloader *tmp = (FileDownloader*)List[i];
			if (!tmp)
				continue;

			//wxLogMessage("Update %s",tmp->GetFilename());
			UpdateOnce(tmp,grey);
			TotalSpeed+=tmp->GetMoySpeed();

			grey=!grey;
		}

		text.Printf("Total speed : %.1f Kb/s",TotalSpeed/1000.0);
		mMain->SetStatusText(text);
	}
}

void DLManager::UpdateOnce(FileDownloader *dl,bool grey)
{
	if (!dl)
		return;

	FileDownloader *tmp = dl;
	wxString text;
	if (tmp)
	{

		long item=-1;
		for (int long i=0;i<mMain->GetListCtrl()->GetItemCount();i++)
		{		
			//wxLogMessage(mMain->GetListCtrl()->GetItemText(i));
			if (mMain->GetListCtrl()->GetItemText(i)==dl->GetFilename())
			{
				item=i;
			}
		}


		if (item==-1)
		{
			item = mMain->GetListCtrl()->InsertItem(0,tmp->GetFilename());
			//wxLogDebug("Insert Item");
		}


		if (tmp->GetFileSize()==-2)
			text.Printf("");
		else if (tmp->GetFileSize()==-1)
			text.Printf("??");
		else
			text.Printf("%I64d",tmp->GetFileSize());
		mMain->GetListCtrl()->SetItem(item,1,text);
		if (tmp->GetDownloadedSize()==0)
			text.Printf("");
		else
			text.Printf("%I64d",tmp->GetDownloadedSize());
		mMain->GetListCtrl()->SetItem(item,2,text);
		//tmp->GetFileSize()/tmp->GetDownloadedSize();
		if (tmp->GetFileSize()==-1 || tmp->GetFileSize()==-2)
			text.Printf("");
		else
			text.Printf("%.2f",((float)tmp->GetDownloadedSize()/tmp->GetFileSize())*100);
			
		mMain->GetListCtrl()->SetItem(item,3,text);


		switch (tmp->GetStatus())
		{
		case FileDownloader::FFD_START:
			text.Printf("Start");
			mMain->GetListCtrl()->SetItemImage(item,0);
			break;
		case FileDownloader::FFD_STOP:
			text.Printf("Stop");
			mMain->GetListCtrl()->SetItemImage(item,1);
			break;
		case FileDownloader::FFD_WAIT:
			text.Printf("Wait");
			mMain->GetListCtrl()->SetItemImage(item,3);
			break;
		case FileDownloader::FFD_ERROR:
			text.Printf("%s",ERROR_DESC[tmp->GetErrorCode()]);
			mMain->GetListCtrl()->SetItemImage(item,2);
			break;
		case FileDownloader::FFD_RETRY:
			text.Printf("Retry n°%d dans %d secondes : %s",tmp->RetryCount,RetryTime-(GetTickCount()-tmp->GetErrorTime())/1000,ERROR_DESC[tmp->GetErrorCode()]);
			mMain->GetListCtrl()->SetItemImage(item,0);
			break;
		case FileDownloader::FFD_FINISH:
			text.Printf("Finish");
			mMain->GetListCtrl()->SetItemImage(item,4);
			break;
		default:
			text.Printf("");
		}
		mMain->GetListCtrl()->SetItem(item,4,text);

		text.Printf("%.1f Kb/s",tmp->GetMoySpeed()/1000);
		mMain->GetListCtrl()->SetItem(item,5,text);

		if (tmp->GetFileSize()>0
			&& tmp->GetMoySpeed()>0)
		{
			long TimeRemaining = (tmp->GetFileSize()-tmp->GetDownloadedSize())/tmp->GetMoySpeed();

			float TempTime=TimeRemaining/3600;
			long Hours = TempTime;
			if (TempTime-Hours>0)
				Hours--;

			long Minutes=0;
			long Seconds=0;


			text.Printf("%d:%2d:%2d",Hours,Minutes,Seconds);
		}else
			text.Printf("??:??:??");

		mMain->GetListCtrl()->SetItem(item,6,text);

		mMain->GetListCtrl()->SetItem(item,7,tmp->GetLink());

		mMain->GetListCtrl()->SetItemBackgroundColour(item,grey==true ? wxColor(224,224,224) : wxColor(255,255,255));

		//if (List.Count()-i-1==selected)
			//mMain->GetListCtrl()->SetItemState(item,wxLIST_STATE_SELECTED,wxLIST_STATE_SELECTED);

		/*if (Deleted)
		{
			mMain->GetNotebook()->InsertPage(1,new FilePage(mMain->GetNotebook()),tmp->GetFilename());
		}else*/{
			for (int j=1;j<mMain->GetNotebook()->GetPageCount();j++)
			{
				if (mMain->GetNotebook()->GetPageText(j) == tmp->GetFilename())
				{
					FilePage *page = (FilePage*)mMain->GetNotebook()->GetPage(j);

					page->SetGaugeValue((float)tmp->GetDownloadedSize()/tmp->GetFileSize()*100.0);
					page->SetFileSize(tmp->GetFileSize());
					page->SetDownloadedSize(tmp->GetDownloadedSize());

					if (mConfig)
						page->SetBlockSize(mConfig->ReadIntValue("BlockSize",2048));

					page->Refresh();
					
					break;
				}
			}
		}

		/*tmp->pMutex->Unlock();*/				
		//tmp->Resume();
	}

	FilePage *page=NULL;
	for (int j=1;j<mMain->GetNotebook()->GetPageCount();j++)
	{
		if (mMain->GetNotebook()->GetPageText(j) == tmp->GetFilename())
		{
			page = (FilePage*)mMain->GetNotebook()->GetPage(j);
			break;
		}
	}

	if (page==NULL)
	{
		mMain->GetNotebook()->InsertPage(1,new FilePage(mMain->GetNotebook()),tmp->GetFilename());

		for (int j=1;j<mMain->GetNotebook()->GetPageCount();j++)
		{
			if (mMain->GetNotebook()->GetPageText(j) == tmp->GetFilename())
			{
				page = (FilePage*)mMain->GetNotebook()->GetPage(j);
				break;
			}
		}
	}

	if (page)
	{
		page->SetGaugeValue((float)tmp->GetDownloadedSize()/tmp->GetFileSize()*100.0);
		page->SetFileSize(tmp->GetFileSize());
		page->SetDownloadedSize(tmp->GetDownloadedSize());

		if (mConfig)
			page->SetBlockSize(mConfig->ReadIntValue("BlockSize",2048));

		page->Refresh();
	}
}

void DLManager::UpdateBlocks(FileDownloader *tmp)
{
	if (tmp)
		for (int j=1;j<mMain->GetNotebook()->GetPageCount();j++)
		{
			if (mMain->GetNotebook()->GetPageText(j) == tmp->GetFilename())
			{
				FilePage *page = (FilePage*)mMain->GetNotebook()->GetPage(j);

				//page->SetGaugeValue((float)tmp->GetDownloadedSize()/tmp->GetFileSize()*100.0);
				page->SetFileSize(tmp->GetFileSize());
				page->SetDownloadedSize(tmp->GetDownloadedSize());

				//if (mConfig)
					//page->SetBlockSize(mConfig->ReadIntValue("BlockSize",2048));

				page->Refresh();
				
				break;
			}
		}
}

void DLManager::StartDownload(wxString link)
{
	FileDownloader *temp;
	bool Found=false;

	for (int i=0;i<List.Count();i++)
	{
		temp = (FileDownloader*)List[i];

		if (!temp)
			continue;
	
		//wxLogDebug("Mutex OK %d",temp->pMutex->IsOk());
		
		/*temp->pMutex->Lock();*/
		wxLogDebug("Mutex Lock");
		if (temp!=NULL && temp->GetFilename()==link)
		{
			if (temp->GetStatus()!=FileDownloader::FFD_START 
				&& temp->GetStatus()!=FileDownloader::FFD_WAIT
				&& temp->GetStatus()!=FileDownloader::FFD_RETRY
				&& temp->GetStatus()!=FileDownloader::FFD_FINISH)
			{
				StartDownload(temp,true);
			}
			UpdateScreen(true);
			Found=true;
			//break;
		}
		/*temp->pMutex->Unlock();*/
		if (Found)
			break;
	}
}

void DLManager::StartDownload(FileDownloader *dl,bool ResetRetyCount, bool force)
{
	wxLogMessage("StartDownload FileDL");
	if (dl==NULL)
		return;

	int i=-1;
	int DLCount=0;
	
	for (int j=0;j<List.Count();j++)
	{
		FileDownloader *temp = (FileDownloader*)List[j];
		if (temp)
		{
			if (temp==dl)
				i=j;

			if (temp->GetStatus()!=FileDownloader::FFD_START
			 /*|| temp->GetStatus()==FileDownloader::FFD_RETRY*/)
			{
				DLCount++;
			}			 
		}
	}

	if (i==-1)
		return;

	//if (dl->IsRunning())
		//return;

	if (ResetRetyCount)
		dl->RetryCount=0;

	if (dl->AlreadyRun())
	{
		/*dl->pMutex->Unlock();*/
		if (dl->GetClass()=="FreeFileDownloader")
			List[i] = new FreeFileDownloader(*dl);
		else 
			List[i] = new FileDownloader(*dl);
		delete dl;
		dl = List[i];
		/*dl->pMutex->Lock();*/
	}

	if (DLCount>=MaxDL && !force)
	{
		//wxLogMessage("Set status");
		dl->SetStatus(FileDownloader::FFD_WAIT);
	}else{
		//wxLogMessage("Start Download");
		/*if (dl->RetryCount>=MaxRetry)
		{
			dl->SetStatus(FileDownloader::FFD_ERROR);
		}else*/
			dl->StartDownload();
	}
}

void DLManager::StopDownload(wxString link,bool update)
{
	wxLogMessage("StartDownload String");
	FileDownloader *temp;
	bool Found=false;
	for (int i=0;i<List.Count();i++)
	{
		temp = (FileDownloader*)List[i];

		if (!temp)
			continue;
	
		if (temp/* && temp->pMutex && temp->pMutex->IsOk()*/)
		{
			/*temp->pMutex->Lock();*/
			if (temp!=NULL && temp->GetFilename()==link)
			{
				if (temp->GetStatus()!=FileDownloader::FFD_FINISH)
				{
					temp->StopDownload();
					if (update)
						UpdateScreen(true);
				}
				Found=true;
			}
			/*temp->pMutex->Unlock();*/
		}

		if (Found)
			break;
	}
}

void DLManager::DeleteDownload(wxString link,bool update)
{
	FileDownloader *temp;
	bool Found=false;
	for (int i=0;i<List.Count();i++)
	{
		FileDownloader *temp = (FileDownloader*)List[i];

		if (!temp)
			continue;
	
		if (temp/* && temp->pMutex && temp->pMutex->IsOk()*/)
		{
			/*temp->pMutex->Lock();*/
			if (temp!=NULL && temp->GetFilename()==link)
			{
				temp->StopDownload();
				Found=true;
				List.Remove(temp);
				for (int i=1;i<mMain->GetNotebook()->GetPageCount();i++)
				{
					if (mMain->GetNotebook()->GetPageText(i) == temp->GetFilename())
					{
						mMain->GetNotebook()->DeletePage(i);
						break;
					}
				}
				/*temp->pMutex->Unlock();*/
				delete temp;
				if (update)
					UpdateScreen(true);
			}
			
		}

		if (Found)
			break;
	}
}

void DLManager::LoadDownloads()
{
	FILE *file=fopen("downloads.cfg","rb");
	if (file)
	{
		/*fseek(file,0,SEEK_END);
		if (ftell(file)==0)
			return;*/

		long Count;
		fread(&Count,sizeof(long),1,file);
		wxString text;

		curl_off_t Size;

		char buffer[256*256];
		FileDownloader *temp=NULL;
		for (int i=0;i<Count;i++)
		{
			Size=0;
			fread(&Size,sizeof(char),1,file);
			if (Size==DL_HTTP)
				temp = new FileDownloader("","",this);	
			else if (Size==DL_FREE)
				temp = new FreeFileDownloader("","",this);	

			if (temp==NULL)
				return;
			
			Size=0;
			fread(&Size,sizeof(short),1,file);
			fread(buffer,sizeof(char),Size,file);
			text="";
			text.append(buffer,Size);
			temp->SetLink(text);

			
			Size=0;
			fread(&Size,sizeof(short),1,file);
			fread(buffer,sizeof(char),Size,file);
			text="";
			text.append(buffer,Size);
			temp->SetFilename(text);

			fread(&Size,sizeof(curl_off_t),1,file);
			temp->SetFileSize(Size);
			
			fread(&Size,sizeof(curl_off_t),1,file);
			temp->SetDownloadedSize(Size);

			if (temp->GetFileSize()==temp->GetDownloadedSize())
				temp->SetStatus(FileDownloader::FFD_FINISH);

			List.Add(temp);
		}
		fclose(file);
	}
}

void DLManager::SaveDownloads()
{
	FILE *file=fopen("downloads.cfg","wb");
	if (file)
	{
		long Count = List.Count();
		fwrite(&Count,sizeof(long),1,file);

		curl_off_t Size;
		FileDownloader *temp;
		for (int i=0;i<List.Count();i++)
		{
			FileDownloader *temp = (FileDownloader*)List[i];
			if (temp==NULL)
				continue;

			Size=0;
			if (temp->GetClass()=="FileDownloader")
				Size=DL_HTTP;
			else if (temp->GetClass()=="FreeFileDownloader")
				Size=DL_FREE;


			fwrite(&Size,sizeof(char),1,file);

			Size=temp->GetLink().length();
			fwrite(&Size,sizeof(short),1,file);
			fwrite(temp->GetLink().c_str(),sizeof(char),temp->GetLink().length(),file);

			Size=temp->GetFilename().length();
			fwrite(&Size,sizeof(short),1,file);
			fwrite(temp->GetFilename().c_str(),sizeof(char),temp->GetFilename().length(),file);

			Size = temp->GetFileSize();
			fwrite(&Size,sizeof(curl_off_t),1,file);
			Size = temp->GetDownloadedSize();
			fwrite(&Size,sizeof(curl_off_t),1,file);
		}
		fclose(file);
	}
}