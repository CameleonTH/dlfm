#include "wx/wx.h"

#ifndef _DLMANAGER_
#define _DLMANAGER_

class FileDownloader;
class MainWindow;

WX_DEFINE_ARRAY_PTR(FileDownloader*, wxArrayFileDownloader);

class DLManager
{
public:
	DLManager();
	~DLManager();

	bool AddDownload(wxString link,wxString filename);

	void UpdateScreen(bool force=false);

	void StartDownload(wxString link);
	void StopDownload(wxString link);
	void DeleteDownload(wxString link);

	void LoadDownloads();
	void SaveDownloads();

	wxArrayFileDownloader List;

	MainWindow *mMain;

	long Time;
	enum
	{
		DL_HTTP,
		DL_FREE,
		DL_MAX
	};
};

#endif