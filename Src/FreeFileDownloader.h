#include <curl\curl.h>
#include <wx/wx.h>

#include "FileDownloader.h"

#include "StateData.h"

#ifndef _FREEFILEDOWNLOADER_
#define _FREEFILEDOWNLOADER_

class FreeFileDownloader : public FileDownloader
{
public:
	FreeFileDownloader(const char *link,const char *filename,DLManager *manager);
	FreeFileDownloader(FileDownloader& copy);

	wxString GetClass() { return wxString("FreeFileDownloader"); }

	//void SetLink();

	//void StartDownload();	// Commence ou reprend le téléchargement
	//void StopDownload();	// Stop (met en pause) le téléchargement

	//long GetFileSize();
	//long GetDownloadedSize();

	//static size_t WriteData(void *buffer, size_t size, size_t nmemb, void *userp);

	void *Entry(); //Thread
	//virtual void OnExit();

	//char *GetLink() { return pLink; }

	//FFD_State GetStatus();

private:
	/*char *pLink;			// Adresse du fichier free (exemple : http://dl.free.fr/2f5g3ds)
	char *pLinkPageRef;		// Page du lien free de reférence (exemple : <html>...)
	char *pHeader;			// Header du fichier à télécharger (pour récupérer la taille du fichier, ...)
	char *pFinalLink;		// Adresse finale du fichier à télécharger
	char *pFilename;		// Nom du fichier
	//byte *data;			// Données du fichier à télécharger
	FILE *pOutput;			// Fichier de sortie
	CURL *pCurl;			// Connection

	long iFileSize;			// Taille du fichier à télécharger
	long iDataPos;			// Taille des données télécharger
	long iMaxSpeed;			// Vitesse max pour télécharger ce fichier

	Page		pStatePage;
	Header		pStateHeader; 
	FileData	pStateData;
	StateData	pStateNone;

	long iTime;

	char	Buffer[2048];
	int		BufferPos;	

	FFD_State Status;	*/
};

#endif