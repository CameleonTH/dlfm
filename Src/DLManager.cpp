#include <curl\curl.h>
#include <wx/listctrl.h>

#include "DLManager.h"
#include "FileDownloader.h"
#include "FreeFileDownloader.h"
#include "MainWindow.h"
#include "FilePage.h"

DLManager::DLManager(Config *config)
{
	wxLogMessage("Download Manager Initialisation");

	mConfig = config;
	//mConfig=NULL;
	curl_global_init(CURL_GLOBAL_ALL);

	List.Clear();

	LoadDownloads();

	if (mConfig)
	if (mConfig->ReadIntValue("StartDownload",0))
	{
		for (int i=0;i<List.Count();i++)
		{
			if (List[i]->GetStatus()!=FileDownloader::FFD_FINISH)
				List[i]->StartDownload();
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
	if (!link)
		return false;

	FileDownloader *temp;
	bool exist=false;
	for (int i=0;i<List.Count();i++)
	{
		temp = (FileDownloader*)List[i];
		
		if (temp!=NULL && temp->GetLink()==link)
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

	if (download->GetStatus()==FileDownloader::FFD_ERROR)
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

	Time = GetTickCount();
	long item;
	bool grey=false;
	wxString text;
	wxLogDebug("UdateScreen");
	if (mMain && mMain->GetListCtrl())
	{
		wxLogDebug("UdateScreen2");
		int selected = -1/*mMain->GetListCtrl()->GetTopItem()*/;
		for (int long i=0;i<mMain->GetListCtrl()->GetItemCount();i++)
		{
			if (mMain->GetListCtrl()->GetItemState(i,wxLIST_STATE_SELECTED/*|wxLIST_STATE_FOCUSED*/))
			{
				selected=i;
				break;
			}
		}
		wxLogDebug("Id : %d",selected);
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

		long TotalSpeed=0;
		for (int i=0 ; i<List.Count();i++)
		{
			FileDownloader *tmp = (FileDownloader*)List[i];
			if (!tmp)
				continue;

			if (tmp)
			{
				//tmp->Pause();
				tmp->pMutex->Lock();				
				//mMain->GetListCtrl()->GetItemData(tmp->GetItemId());
				//text.Printf("%d",tmp->GetFileSize());
				//wxLogMessage(text);
				item = mMain->GetListCtrl()->InsertItem(0,tmp->GetFilename());
				//wxLogMessage(tmp->GetFilename());
				if (tmp->GetFileSize()==-2)
					text.Printf("");
				else if (tmp->GetFileSize()==-1)
					text.Printf("??");
				else
					text.Printf("%d",tmp->GetFileSize());
				mMain->GetListCtrl()->SetItem(item,1,text);
				if (tmp->GetDownloadedSize()==0)
					text.Printf("");
				else
					text.Printf("%d",tmp->GetDownloadedSize());
				mMain->GetListCtrl()->SetItem(item,2,text);
				//tmp->GetFileSize()/tmp->GetDownloadedSize();
				if (tmp->GetFileSize()==-1 || tmp->GetFileSize()==-2)
					text.Printf("");
				else
					text.Printf("%.2f",(float)tmp->GetDownloadedSize()/tmp->GetFileSize()*100);
					
				mMain->GetListCtrl()->SetItem(item,3,text);

				switch (tmp->GetStatus())
				{
				case FileDownloader::FFD_START:
					text.Printf("Start");
					break;
				case FileDownloader::FFD_STOP:
					text.Printf("Stop");
					break;
				case FileDownloader::FFD_ERROR:
					text.Printf("%s",ERROR_DESC[tmp->GetErrorCode()]);
					break;
				case FileDownloader::FFD_FINISH:
					text.Printf("Finish");
					break;
				default:
					text.Printf("");
				}
				mMain->GetListCtrl()->SetItem(item,4,text);

				TotalSpeed+=tmp->GetMoySpeed();
				text.Printf("%.1f Kb/s",tmp->GetMoySpeed()/1000);
				mMain->GetListCtrl()->SetItem(item,5,text);

				mMain->GetListCtrl()->SetItem(item,6,tmp->GetLink());

				mMain->GetListCtrl()->SetItemBackgroundColour(item,grey==true ? wxColor(224,224,224) : wxColor(255,255,255));

				if (List.Count()-i-1==selected)
					mMain->GetListCtrl()->SetItemState(item,wxLIST_STATE_SELECTED,wxLIST_STATE_SELECTED);

				if (Deleted)
				{
					mMain->GetNotebook()->InsertPage(1,new FilePage(mMain->GetNotebook()),tmp->GetFilename());
				}else{
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

				tmp->pMutex->Unlock();				
				//tmp->Resume();
				grey=!grey;
			}
		}

		text.Printf("Total speed : %.1f Kb/s",TotalSpeed/1000.0);
		mMain->SetStatusText(text);
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
		FileDownloader *temp = (FileDownloader*)List[i];

		if (!temp)
			continue;
	
		//wxLogDebug("Mutex OK %d",temp->pMutex->IsOk());
		
		temp->pMutex->Lock();
		wxLogDebug("Mutex Lock");
		if (temp!=NULL && temp->GetFilename()==link)
		{
			if (temp->GetStatus()!=FileDownloader::FFD_START && temp->GetStatus()!=FileDownloader::FFD_FINISH)
			{
				if (temp->AlreadyRun())
				{
					temp->pMutex->Unlock();
					if (temp->GetClass()=="FreeFileDownloader")
						List[i] = new FreeFileDownloader(*temp);
					else 
						List[i] = new FileDownloader(*temp);
					delete temp;
					temp = List[i];
					temp->pMutex->Lock();
				}
				temp->StartDownload();
			}
			UpdateScreen(true);
			Found=true;
			//break;
		}
		temp->pMutex->Unlock();
		if (Found)
			break;
	}
}

void DLManager::StopDownload(wxString link)
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
			temp->pMutex->Lock();
			if (temp!=NULL && temp->GetFilename()==link)
			{
				if (temp->GetStatus()!=FileDownloader::FFD_FINISH)
				{
					temp->StopDownload();
					UpdateScreen(true);
				}
				Found=true;
			}
			temp->pMutex->Unlock();
		}

		if (Found)
			break;
	}
}

void DLManager::DeleteDownload(wxString link)
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
			temp->pMutex->Lock();
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
				temp->pMutex->Unlock();
				delete temp;
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

		long Size;

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

			fread(&Size,sizeof(long),1,file);
			temp->SetFileSize(Size);
			
			fread(&Size,sizeof(long),1,file);
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

		long Size;
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
			fwrite(&Size,sizeof(long),1,file);
			Size = temp->GetDownloadedSize();
			fwrite(&Size,sizeof(long),1,file);
		}
		fclose(file);
	}
}