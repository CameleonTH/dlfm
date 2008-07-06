#include "LanguageManager.h"
#include <wx/dir.h>

LanguageManager::LanguageManager()
{
}

wxArrayString LanguageManager::AvailableLanguage()
{
	wxArrayString Strings;
	wxString name;
	wxString tem;
	int Count=0;
	//fopen(
	//Strings.Add("French");
	//Strings.Add("English");

	wxDir *dir = new wxDir("\lang");
	if (dir)
	{
		while(dir->GetNext(&name))
		{
			if (name.AfterLast('.')=="lang")
			{
				Strings.Add(name.BeforeLast('.'));
			}
		}
	}
	return Strings;
}

bool LanguageManager::LoadLanguage(wxString Language)
{
	if (Language=="")
		return false;

	Config *config = new Config("Lang\\"+Language+".lang");

	if (config)
	{
		config->Load();
		Values.Clear();

		SetupLocalisationCaption();

		for (int i=0;i<Values.Count();i++)
		{
			if (Values[i])
				Values[i]->Value = config->ReadStringValue(Values[i]->Name,Values[i]->Value,false);
		}
		
		if (config->ExistFile())
			return true;
	}

	return false;

}

wxString LanguageManager::GetValue(wxString Name)
{
	//Values.
	//wxArrayString Values;

	for (int i=0;i<Values.Count();i++)
	{
		if (Values[i]->Name == Name)
			return Values[i]->Value;
	}

	return "";
}