#include "wx/wx.h"

#ifndef _CONFIG_
#define _CONFIG_

class ValueSlot
{
public:
	ValueSlot(wxString name=wxString(""), wxString value=wxString(""),bool Com=false) { Name=name; Value=value; IsCom=Com; }
	wxString Name;
	wxString Value;
	bool IsCom;
};

WX_DEFINE_ARRAY_PTR(ValueSlot*, wxArrayValueSlot);

class Config
{
public:
	Config(wxString Filename);

	void		Load();
	void		Save();

	bool		ValueExist(wxString ValueName);
	wxString	ReadStringValue(wxString ValueName,wxString Default="",bool WriteDefaultValue=true);
	int			ReadIntValue(wxString ValueName,int Default=0,bool WriteDefaultValue=true);
	float		ReadFloatValue(wxString ValueName,float Default=0,bool WriteDefaultValue=true);

	void		WriteStringValue(wxString ValueName,wxString Value);
	void		WriteIntValue(wxString ValueName,int Value);
	void		WriteFloatValue(wxString ValueName,float Value);

	void		SetFilename(wxString filename) { mFilename=filename; }
	wxString	GetFilename() { return mFilename; }
	bool		ExistFile();

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