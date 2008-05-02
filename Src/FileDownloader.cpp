#include "FileDownloader.h"
#include "DLManager.h"

FileDownloader::FileDownloader(FileDownloader& copy)
	: wxThread(wxTHREAD_JOINABLE)
{
	pLink = copy.pLink;
	pFileName = copy.pFileName;

	pMutex = new wxMutex(); // copy.pMutex;

	pStatePage.pParent = this;
	pStateHeader.pParent = this;
	pStateData.pParent = this;
	pStateNone.pParent = this;

	iFileSize=copy.iFileSize;	
	iDataPos=copy.iDataPos;	
	iTime=GetTickCount();
	Status = copy.Status;
	bAlreadyRun=false;

	pOutput=NULL;	
	pCurl=NULL;

	fMoySpeed=0;
	fSpeed=0;

	Manager = copy.Manager;

	if ( Create() != wxTHREAD_NO_ERROR )
    {
        wxLogError(wxT("Can't create thread!"));
		Status=FFD_ERROR;
		Manager->UpdateScreen(true);
        return;
    }
}

FileDownloader::FileDownloader(wxString link,wxString filename,DLManager *manager)
	: wxThread(wxTHREAD_JOINABLE)
{
	iTime = GetTickCount();
	/*pLinkPageRef=NULL;
	pHeader=NULL;
	pOutput=NULL;	
	pCurl=NULL;
	pFinalLink=NULL;
	pLink=NULL;
	pFileName=NULL;*/
	//wxLogMessage("Init FileDownloader\n");
	//if (link==NULL || filename==NULL || mutex==NULL)
		//return;

	/*pLink = (char*)malloc(strlen(link));
	if (pLink)
	{
		strcpy(pLink,link);
		pLink[strlen(link)]='\0';
	}

	pFileName=(char*)malloc(strlen(filename));
	if (pFileName)
	{
		strcpy(pFileName,filename);
		pFileName[strlen(filename)]='\0';
	}*/

	pLink = link;
	pFileName = filename;


	pMutex = new wxMutex();

	wxLogDebug("Filename : %s",pFileName);

	//Status=FFD_STOP;

	pStatePage.pParent = this;
	pStateHeader.pParent = this;
	pStateData.pParent = this;
	pStateNone.pParent = this;

	iFileSize=-2;	
	iDataPos=0;	
	iTime=GetTickCount();
	Status = FFD_STOP;

	bAlreadyRun=false;
	fMoySpeed=0;
	fSpeed=0;

	pOutput=NULL;	
	pCurl=NULL;
	Manager = manager;

	if ( Create() != wxTHREAD_NO_ERROR )
    {
        wxLogError(wxT("Can't create thread!"));
		Status=FFD_ERROR;
        return;
    }
}

FileDownloader::~FileDownloader(void)
{
	delete pMutex;
}

void *FileDownloader::Entry()
{
	wxLogMessage("Init Download : %s\n",pLink);
	pCurl = curl_easy_init();
	if (pCurl)
	{
		pMutex->Lock();
		CURLcode Res;

		wxLogMessage("Ouverture fichier\n");
		pOutput = fopen(pFileName,"ab");
		pMutex->Unlock();
		if (!pOutput)
			return 0;

		//LockFile(pFilename);
		//wxLogMessage("Debut de téléchargement du fichier\n");
		//fseek (pOutput, 0, SEEK_END);
		//iDataPos = ftell(pOutput);
		//wxLogMessage("Pos : %d\n",iDataPos);

		pMutex->Lock();
		char Range[64];
		Range[0]='\0';
		sprintf_s(Range,64,"%d-",iDataPos);

		wxLogMessage("Commencement dl\n");
		//Début de téléchargement du fichier
		curl_easy_reset(pCurl);
		curl_easy_setopt(pCurl, CURLOPT_URL, pLink);
		//curl_easy_setopt(pCurl, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(pCurl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows; U; Windows NT 5.1; fr; rv:1.8.1.13) Gecko/20080311 Firefox/2.0.0.13");
		//curl_easy_setopt(pCurl, CURLOPT_REFERER, pLink);
		curl_easy_setopt(pCurl, CURLOPT_RANGE,Range); // "38920-"

		curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &pStateData); 
		curl_easy_setopt(pCurl, CURLOPT_WRITEHEADER, &pStateHeader);
		curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, WriteData);

		pMutex->Unlock();

		bStartDL=false;		
		Res = curl_easy_perform(pCurl);
		if (Res != CURLE_OK) {
		  fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(Res));
		  return 0;
		}

		if (Status==FFD_START)
			Status=FFD_FINISH;
	}
	return 0;
}


void FileDownloader::OnExit()
{
	//if (Status != FFD_FINISH)
		//Status = FFD_STOP;
	Manager->UpdateScreen(true);
	
	wxLogMessage("Exit\n");
	wxLogMessage("Fermeture fichier\n");
	if (pOutput)
		fclose(pOutput);
	pOutput=NULL;
	wxLogMessage("Fermeture connection\n");
	if (pCurl)
		curl_easy_cleanup(pCurl);
	pCurl=NULL;
}

void FileDownloader::StartDownload()
{
	if (Status==FFD_START)
		return;
	Status=FFD_START;
	Manager->UpdateScreen(true);
	int error = Run();
	bAlreadyRun=true;
	if ( error == wxTHREAD_RUNNING )
    {
        wxLogError(wxT("wxTHREAD_RUNNING"));
		Status=FFD_ERROR;
    }
}

void FileDownloader::StopDownload()
{
	Status = FFD_STOP;
	Manager->UpdateScreen(true);
}

size_t FileDownloader::WriteData(void *buffer, size_t size, size_t nmemb, void *userp)
{
	StateData *pDT = (StateData*)userp;

	if (!pDT)
		//return -1;
		return size*nmemb;	

	FileDownloader *pFFD = pDT->pParent;

	if (!pFFD)
		//return -1;
		return size*nmemb;	

	if (pFFD->Status!=FFD_START)
		return 0;

	switch (pDT->Type)
	{
	case StateData::DT_NONE:
		return size*nmemb;
		break;
	case StateData::DT_PAGE:
		pFFD->pLinkPageRef.append((char*)buffer,size*nmemb);
		return size*nmemb;		
		break;	
	case StateData::DT_HEADER:	
		pFFD->pHeader.append((char*)buffer,size*nmemb);
		return size*nmemb;	
		break;
	case StateData::DT_FILE_DATA:
		//if (pFFD->iFileSize==-2)
		if (pFFD->bStartDL==false)
		{
			pFFD->bStartDL=true;
			// On récupére la taille du fichier finale
			long size = Parser::GetFileSizeHTTP(pFFD->pHeader);

			/*double size2;
			double size3;
			curl_easy_getinfo(pFFD->pCurl, CURLINFO_SIZE_DOWNLOAD, &size2);
			curl_easy_getinfo(pFFD->pCurl, CURLINFO_REQUEST_SIZE, &size2);
			curl_easy_getinfo(pFFD->pCurl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &size3);			

			wxLogMessage("Size: %d Size2: %f",size,size2+size3);*/
			//CURLINFO_REQUEST_SIZE

			if (pFFD->iDataPos>0)
				if (size!=pFFD->iFileSize)
				{
					pFFD->Status=FFD_ERROR;
					return 0;
				}

			pFFD->iTime=GetTickCount();

			if (size<=0)
				pFFD->iFileSize=-1;
			else
				pFFD->iFileSize=size/*+pFFD->iDataPos*/;
		}
		fseek(pFFD->pOutput,pFFD->iDataPos,SEEK_SET);
		int len = fwrite(buffer,size,nmemb,pFFD->pOutput);

		//fwrite(&pFFD->iFileSize,sizeof(long),1,pFFD->pOutput);
		//fwrite(&pFFD->iDataPos,sizeof(long),1,pFFD->pOutput);

		//fwrite(&HeaderSize,sizeof(int),1,pFFD->pOutput);

		//End of write header

		//float Speed = len*1000/((float)GetTickCount()-pFFD->iTime);
		
		long tick = GetTickCount();
		if (tick!=pFFD->iTime)
		{
			pFFD->iByteDL+=size*nmemb;
			if (tick-pFFD->iTime>=1000)
			{
				pFFD->fSpeed=pFFD->iByteDL*1000.0/(tick-pFFD->iTime);
				//long speed=pFFD->fSpeed/1000;
				//wxLogMessage("Speed ; %d %d %d %d",speed,size*nmemb,GetTickCount(),pFFD->iTime);
				if (pFFD->fMoySpeed==0)
					pFFD->fMoySpeed=pFFD->fSpeed;
				else
					pFFD->fMoySpeed=(pFFD->fMoySpeed+pFFD->fSpeed)/2;

				//curl_easy_getinfo(pFFD->pCurl, CURLINFO_SPEED_DOWNLOAD, &(pFFD->fMoySpeed));
				
				pFFD->iTime=GetTickCount();
				pFFD->iByteDL=0;
			}
		}

		pFFD->iDataPos+=len;

		if (pFFD->iDataPos==pFFD->iFileSize)
			pFFD->Status=FFD_FINISH;

		if (len!=size*nmemb)
		{
			pFFD->Status=FFD_ERROR;
			pFFD->Manager->UpdateScreen(true);
		}
		return len;		
		break;	
	}
	return size*nmemb;

}