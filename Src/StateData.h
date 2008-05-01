class FileDownloader;

#ifndef _STATEDATA_
#define _STATEDATA_

class StateData
{
public:
	enum Data_type
	{
		DT_NONE,
		DT_PAGE,
		DT_HEADER,
		DT_FILE_DATA,
		DT_MAX
	};

	StateData(void);

	FileDownloader *pParent;

	Data_type Type;	
};

class Page : public StateData
{
public:
	Page(void);
};

class Header : public StateData
{
public:
	Header(void);
};

class FileData : public StateData
{
public:
	FileData(void);
};

#endif