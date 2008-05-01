#include <wx/wx.h>
#include <curl\curl.h>
//#include "DLManager.h"

#include "StateData.h"
#include "Parser.h"
#include "error.h"

//#include "StateData.h"

#ifndef _FILEDOWNLOADER_
#define _FILEDOWNLOADER_

class DLManager;

class FileDownloader : public wxThread
{
public:
	FileDownloader(wxString link,wxString filename,DLManager *manager);
	FileDownloader(FileDownloader& copy);
	~FileDownloader(void);

	virtual wxString GetClass() { return wxString("FileDownloader"); }

	virtual void StartDownload();	// Commence ou reprend le téléchargement
	virtual void StopDownload();	// Stop (met en pause) le téléchargement

	virtual long GetFileSize() { return iFileSize;}
	virtual long GetDownloadedSize() { return iDataPos;}
	inline wxString GetFilename() { pMutex->Lock();wxString tem = pFileName;pMutex->Unlock();return tem; }
	inline wxString GetLink() { pMutex->Lock();wxString tem = pLink;pMutex->Unlock();return tem; }
	virtual float GetSpeed() { return fSpeed; }
	virtual float GetMoySpeed() { return fMoySpeed; }

	static size_t WriteData(void *buffer, size_t size, size_t nmemb, void *userp);

	//Thread
	virtual void *Entry();
	/*{
		wxLogMessage("Entry\n");
		return 0;
	}*/
	virtual void OnExit();
	/*{
		wxLogMessage("Exit\n");
	}*/


	void SetLink(wxString link) { pLink=link; }
	void SetFilename(wxString file) { pFileName=file; }
	void SetFileSize(long size) { iFileSize=size; }
	void SetDownloadedSize(long size) { iDataPos=size; }

	//FFD_State GetStatus();
	int GetStatus() { return Status; }

	bool AlreadyRun() { return bAlreadyRun; }

	enum FFD_State
	{
		FFD_STOP,
		FFD_START,
		FFD_FINISH,
		FFD_ERROR,
		FFD_CHECK,
		FFD_MAX
	};

	wxMutex *pMutex;
protected:
	DLManager	*Manager;
	wxString	pLink;				// Adresse du fichier free (exemple : http://dl.free.fr/2f5g3ds)
	wxString	pFileName;			// Nom du fichier
	wxString	pLinkPageRef;		// Page du lien free de reférence (exemple : <html>...)
	wxString	pHeader;			// Header du fichier à télécharger (pour récupérer la taille du fichier, ...)
	wxString	pFinalLink;			// Adresse finale du fichier à télécharger
	//byte *data;					// Données du fichier à télécharger
	FILE		*pOutput;			// Fichier de sortie
	CURL		*pCurl;				// Connection

	long		iFileSize;			// Taille du fichier à télécharger
	long		iDataPos;			// Taille des données télécharger
	//long iMaxSpeed;				// Vitesse max pour télécharger ce fichier

	Page		pStatePage;
	Header		pStateHeader; 
	FileData	pStateData;
	StateData	pStateNone;

	FFD_State	Status;
	ERROR_CODE	Error;

	bool		bAlreadyRun;
	bool		bStartDL;

	long		iTime;
	long		iByteDL;
	float		fSpeed;
	float		fMoySpeed;

	char		Buffer[2048];
	int			BufferPos;
};

#endif