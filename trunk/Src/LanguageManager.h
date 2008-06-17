#include "Config.h"

#ifndef _LANGUAGEMANAGER_
#define _LANGUAGEMANAGER_

#define INIT_LOC extern LanguageManager *Language = new LanguageManager();

#define DECLARE_SETUP_LOC void SetupLocalisationCaption();
#define BEGIN_SETUP_LOC void LanguageManager::SetupLocalisationCaption() { Values.Clear();
#define ADD_CAPTION(Name,Default) Values.Add(new ValueSlot(Name,Default,false));
#define END_SETUP_LOC }

#define SETLANGUAGE(Language) (/*Language!=NULL ? */Language->LoadLanguage(Language))
#define GETLOCALISATION(Value) (Language!=NULL ? Language->GetValue(Value) : "")

#define ADDDOWNLOAD GETLOCALISATION("AddDownload")
#define STARTDOWNLOAD GETLOCALISATION("StartDownload")
#define STOPDOWNLOAD GETLOCALISATION("StopDownload")
#define DELETEDOWNLOAD GETLOCALISATION("DeleteDownload")

#define FILENAME GETLOCALISATION("Filename")
#define SIZE GETLOCALISATION("Size")
#define DOWNLOADED GETLOCALISATION("Downloaded")
#define STATUS GETLOCALISATION("Status")
#define SPEED GETLOCALISATION("Speed")
#define DOWNLOADSLIST GETLOCALISATION("DownloadsList")

class LanguageManager
{
public:
	LanguageManager::LanguageManager();

	wxArrayString AvailableLanguage();
	bool LoadLanguage(wxString Language);
	wxString GetValue(wxString Name);

	DECLARE_SETUP_LOC

private:
	wxArrayValueSlot Values;
};

extern LanguageManager *Language;

#endif