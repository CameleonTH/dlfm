#include "FreeFileDownloader.h"
#include <string>

FreeFileDownloader::FreeFileDownloader(const char *link,const char *filename,DLManager *manager)
	: FileDownloader(link,filename,manager)
{
}

FreeFileDownloader::FreeFileDownloader(FileDownloader& copy)
	: FileDownloader(copy)
{
}

void *FreeFileDownloader::Entry()
{
	wxLogMessage("Free File Downloader\nInit Download : %s\n",pLink);
	wxLogMessage("File name : %s",Parser::GetFilenameFromURL((char*)pLink.c_str()));
	
	if (pLink=="" || pFileName=="")
		return NULL;

	Status = FFD_START;

	pCurl = curl_easy_init();
	if (pCurl)
	{
		//Status = FFD_START;
		CURLcode Res;

		curl_easy_setopt(pCurl, CURLOPT_URL, pLink);
		//curl_easy_setopt(pCurl, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(pCurl, CURLOPT_COOKIEFILE, "");

		curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &pStatePage); 
		curl_easy_setopt(pCurl, CURLOPT_WRITEHEADER, &pStateHeader);
		curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, WriteData);
		
		wxLogMessage("pLinkPageRef : %d\n",pLinkPageRef);
		Res = curl_easy_perform(pCurl);
		if (Res != CURLE_OK) {
		  wxLogMessage("Curl perform failed: %s\n", curl_easy_strerror(Res));
		  Status = FFD_ERROR;
		  Error = ERROR_HOST_NOT_FOUND;
		  return 0;
		}

		wxLogMessage("Header\n\n%s",pHeader);
		wxLogMessage("Body\n\n%s",pLinkPageRef);

		if (pHeader=="" || pLinkPageRef=="")
		{
			Status = FFD_ERROR;
			Error = ERROR_FREE_SERVER;
			return NULL;
		}

		std::string temp(pHeader);

		// Si on a une redirection alors on suit le lien
		int pos = temp.rfind("Location:");
		if (pos!=-1)
		{
			//wxLogMessage("Redirection\nPos %d",pos);
			int max = pos;
			int min = pos;

			while (max<=temp.length())
			{
				if (temp[max]==' ')
					min=max;

				if (temp[max]=='\n' || temp[max]=='\r')
					break;
				max++;
			}

			
			//pLink = (char*)realloc(pLink,temp.substr(min,max-min-1).length());
			//strcpy(pLink,temp.substr(min+1,max-min-1).c_str());
			pLink = temp.substr(min+1,max-min-1);
			wxLogMessage("Link : %s\n",pLink);

			//pLink = (char *)malloc(strlen(page.substr(pos+1,max-pos).c_str()));
			//strcpy(pLink,page.substr(pos+1,max-pos).c_str());

			curl_easy_reset(pCurl);
			curl_easy_setopt(pCurl, CURLOPT_URL, pLink);
			//curl_easy_setopt(pCurl, CURLOPT_VERBOSE, 1);
			curl_easy_setopt(pCurl, CURLOPT_COOKIEFILE, "");

			curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &pStatePage); 
			curl_easy_setopt(pCurl, CURLOPT_WRITEHEADER, 0);
			curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, WriteData);
			
			//wxLogMessage("pLinkPageRef : %d\n",pLinkPageRef);
			Res = curl_easy_perform(pCurl);
			if (Res != CURLE_OK) {
				if (Status!=FFD_STOP)
				{
					wxLogMessage( "Curl perform failed: %s\n", curl_easy_strerror(Res));
					Status = FFD_ERROR;
					Error = ERROR_HOST_NOT_FOUND;
					return 0;
				}
			}

		}

		wxLogMessage("Fin de la redirection\n");

		//free(pHeader);
		pHeader="";


		//Récupére le lien de téléchargement

		std::string page(pLinkPageRef);
		std::string link(pLink);

		pos = page.rfind(link.substr( link.rfind("/")));
		int max = pos + link.substr( link.rfind("/")).length()-1;

		while(pos>0)
		{
			if (pLinkPageRef[pos]=='\"')
				break;
			pos--;
		}

		wxLogMessage("Test\n\n%s\n",pLinkPageRef);

		if (max-pos>0)
		{
			//pFinalLink = (char *)malloc(strlen(page.substr(pos+1,max-pos).c_str()));
			//strcpy(pFinalLink,page.substr(pos+1,max-pos).c_str());
			pFinalLink = page.substr(pos+1,max-pos);
		}else{
			Status = FFD_ERROR;
			Error = ERROR_FREE_DATA_CORRUPT;
			return NULL;
		}


		wxLogMessage("FinalLink : %s\n",pFinalLink);

		// On récupére le nom du fichier
		std::string filename = page.substr(pos+1,max-pos).substr(page.substr(pos+1,max-pos).rfind("/")+1);

		//wxLogMessage("%s\n",filename.c_str());
		//pFilename = (char*)malloc(filename.length());
		//strcpy(pFilename,filename.c_str());

		wxLogMessage("Récupération de l'image\n");
		//Télécharge le fichier im.pl pour initialiser le téléchargement
		curl_easy_reset(pCurl);
		//wxLogMessage("Charge /im.pl\n\n");
		curl_easy_setopt(pCurl, CURLOPT_URL, "http://dl.free.fr/im.pl");
		//curl_easy_setopt(pCurl, CURLOPT_VERBOSE, 1);

		//curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, pStateNone); 
		//curl_easy_setopt(pCurl, CURLOPT_WRITEHEADER, pStateNone);
		//curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, WriteData);

		Res = curl_easy_perform(pCurl);
		if (Res != CURLE_OK) {
			if (Status!=FFD_STOP)
			{
				wxLogError("Curl perform failed: %s\n", curl_easy_strerror(Res));
				Status = FFD_ERROR;
				Error = ERROR_HOST_NOT_FOUND;
				return 0;
			}
		}

		wxLogMessage("Ouverture fichier\n");
		pOutput = fopen(pFileName,"ab");
		if (!pOutput)
		{
			Status = FFD_ERROR;
			Error = ERROR_FILE_NOT_OPEN;
			return 0;
		}

		//LockFile(pFilename);
		//wxLogMessage("Debut de téléchargement du fichier\n");
		fseek (pOutput, 0, SEEK_END);
		iDataPos = ftell(pOutput);
		//wxLogMessage("Pos : %d\n",iDataPos);

		char Range[64];
		Range[0]='\0';
		sprintf_s(Range,64,"%d-",iDataPos);

		wxLogMessage("Commencement dl\n");
		//Début de téléchargement du fichier
		curl_easy_reset(pCurl);
		curl_easy_setopt(pCurl, CURLOPT_URL, pFinalLink);
		//curl_easy_setopt(pCurl, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(pCurl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows; U; Windows NT 5.1; fr; rv:1.8.1.13) Gecko/20080311 Firefox/2.0.0.13");
		curl_easy_setopt(pCurl, CURLOPT_REFERER, pLink);
		curl_easy_setopt(pCurl, CURLOPT_RANGE,Range); // "38920-"

		curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &pStateData); 
		curl_easy_setopt(pCurl, CURLOPT_WRITEHEADER, &pStateHeader);
		curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, FileDownloader::WriteData);


		pOutput = fopen(pFileName,"ab");

		Res = curl_easy_perform(pCurl);
		if (Res != CURLE_OK) {
			if (Status!=FFD_STOP)
			{
				wxLogMessage("Curl perform failed: %s\n", curl_easy_strerror(Res));
				Status = FFD_ERROR;
				Error = ERROR_HOST_NOT_FOUND;
				return 0;
			}
		}
		
		if (Status==FFD_START)
			Status=FFD_FINISH;

		//wxLogMessage("Header\n\n%s",pHeader);

		//wxLogMessage("File Size : %d",Parser::GetFileSizeHTTP(pHeader));
	}
	return 0;
}
/*void FreeFileDownloader::OnExit()
{
	wxLogMessage("Exit\n");
	if (pOutput)
		fclose(pOutput);
	pOutput=NULL;
	if (pCurl)
		curl_easy_cleanup(pCurl);
	pCurl=NULL;
}

void FreeFileDownloader::StartDownload()
{
	wxLogMessage("Start download\n");
	wxLogMessage("Start download");
	if (!pLink)
		return;

	Run();
}

void FreeFileDownloader::StopDownload()
{
	wxLogMessage("Stop Download\n");
	Delete();
	Status = FFD_STOP;	
}*/