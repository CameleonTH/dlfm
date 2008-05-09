#include "wx/wx.h"

#ifndef _CONFIG_
#define _CONFIG_

class ValueSlot
{
public:
	wxString Name;
	wxString Value;
};

WX_DEFINE_ARRAY_PTR(ValueSlot*, wxArrayValueSlot);

class Config
{
public:
	Config(wxString Filename);

	void		Load();
	void		Save();

	bool		ValueExist(wxString ValueName);
	wxString	ReadStringValue(wxString ValueName,wxString Default="");
	int			ReadIntValue(wxString ValueName,int Default=0);
	float		ReadFloatValue(wxString ValueName,float Default=0);

	void		WriteStringValue(wxString ValueName,wxString Value);
	void		WriteIntValue(wxString ValueName,int Value);
	void		WriteFloatValue(wxString ValueName,float Value);

private:
	enum
	{
		STATE_NONE,
		STATE_COM,
		STATE_PRENAME,
		STATE_NAME,
		STATE_PREVALUE,
		STATE_VALUE,
		STATE_MAX
	};
	wxString mFilename;

	wxArrayValueSlot Values;
};

#endif