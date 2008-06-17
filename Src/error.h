// Fichier à include dans tout les fichiers cpp

#ifndef _ERRORCODE_
#define _ERRORCODE_

#ifndef APSTUDIO_READONLY_SYMBOLS
#include "LanguageManager.h"

enum ERROR_CODE
{
	ERROR_NONE,
	ERROR_FILE_ALREADY_EXIST,
	ERROR_FILE_WRITE_ERROR,
	ERROR_FILE_NOT_OPEN,
	ERROR_CONNECTION_FAILED,
	ERROR_HOST_NOT_FOUND,
	ERROR_THREAD_NOT_CREATE,
	ERROR_DIFFERENT_FILESIZE,
	ERROR_FREE_SERVER,
	ERROR_FREE_DATA_CORRUPT,
	ERROR_UNKNOWN,
	ERROR_MAX
};

extern char *ERROR_DESC[];

#define TITLE wxString(NAME)+wxString(" ")+wxString(VERSION)+wxString(" By CameleonTH")

#endif

#define VERSIONID 0,4
#define VERSION "0.4 Beta"
#define NAME "DL.Free Manager"


#endif
