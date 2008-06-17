#include "wx/wx.h"
#include "Config.h"

#ifndef _DLMANAGER_
#define _DLMANAGER_

class FileDownloader;
class MainWindow;

WX_DEFINE_ARRAY_PTR(FileDownloader*, wxArrayFileDownloader);

class DLManager
{
public:
	DLManager(Config *config);
	~DLManager();

	bool AddDownload(wxString link,wxString filename);

	void UpdateScreen(bool force=false);
	void UpdateBlocks(FileDownloader *file);
	void UpdateOnce(FileDownloader *dl,bool grey);

	void StartDownload(wxString link);
	void StartDownload(FileDownloader *dl,bool ResetRetyCount=false,bool force=false);
	void StopDownload(wxString link,bool update=true);
	void DeleteDownload(wxString link);

	void LoadDownloads();
	void SaveDownloads();

	wxArrayFileDownloader List;

	MainWindow *mMain;

	Config *mConfig;
	long Time;

	int MaxRetry;
	int RetryTime;
	int MaxDL;

	enum
	{
		DL_HTTP,
		DL_FREE,
		DL_MAX
	};
};

#endif