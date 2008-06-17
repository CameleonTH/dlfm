#include "Config.h"

Config::Config(wxString Filename)
{
	mFilename=Filename;

	Values.Clear();
}

bool Config::ExistFile()
{
	FILE *file = fopen(mFilename,"rb");
	if (file)
	{
		fclose(file);
		return true;
	}
	return false;
}

void Config::Load()
{
	Values.Clear();

	FILE *file=fopen(mFilename,"r");
	if (file)
	{
		wxString Name;
		wxString Value;

		int State = STATE_NONE;
		char preccar=' ';
		char car=fgetc(file);
		if (file)
		{
			while (!feof(file))
			{
				switch (State)
				{
				case STATE_NONE:
					if (car=='#')
					{
						Name="";
						State=STATE_COM;
					}else if (car!='\t' || car!='\n' || car!='\r')
					{
						State=STATE_PRENAME;
					}
					break;
				case STATE_COM:
					if (car=='\n' || car=='\r')
					{
						ValueSlot *slot = new ValueSlot();
						slot->Name = Name;
						slot->Value = "";
						slot->IsCom=true;
						Values.Add(slot);
						State=STATE_NONE;
					}else
						Name+=car;					
					break;
				case STATE_PRENAME:
					if (car=='\t' || car=='\n' || car=='\r')
					{
						State=STATE_NONE;
					}else if (car!=' ')
					{
						State=STATE_NAME;
						Name="";
						Name+=preccar;
						Name+=car;
					}
					break;
				case STATE_NAME:
					if (car==' ' || car=='=')
					{
						State=STATE_PREVALUE;
					}else if (car=='\t' || car=='\n' || car=='\r')
					{
						State=STATE_NONE;
					}else
						Name+=car;
					break;
				case STATE_PREVALUE:
					if (car=='\t' || car=='\n' || car=='\r')
					{
						State=STATE_NONE;
					}else if (car!=' ')
					{
						State=STATE_VALUE;
						Value="";
						Value+=car;
					}
					break;
				case STATE_VALUE:
					if (car=='\t' || car=='\n' || car=='\r' /*|| car==' '*/ || car=='=')
					{
						//wxLogMessage("Name : %s, Value : %s",Name,Value);
						ValueSlot *slot = new ValueSlot();
						slot->Name = Name;
						slot->Value = Value;
						slot->IsCom=false;
						Values.Add(slot);
						State=STATE_NONE;
					}else
						Value+=car;
					break;
				}
				preccar=car;
				car = fgetc(file);
			}

			fclose(file);

			if (State==STATE_VALUE)
			{
				//wxLogMessage("Name : '%s', Value : '%s'",Name,Value);
				ValueSlot *slot = new ValueSlot();
				slot->Name = Name;
				slot->Value = Value;
				slot->IsCom=false;
				Values.Add(slot);
			}
		}
	}
}

void Config::Save()
{
	FILE *file=fopen(mFilename,"w");
	if (file)
	{
		for (int i=0;i<Values.Count();i++)
		{
			if (Values[i]->IsCom)
				fprintf(file,"#%s\n",Values[i]->Name);
			else
				fprintf(file,"%s=%s\n",Values[i]->Name,Values[i]->Value);
		}
	}			  
	fclose(file);
}


bool Config::ValueExist(wxString ValueName)
{
	for (int i=0;i<Values.Count();i++)
	{

		if (Values[i]->IsCom==false && Values[i]->Name == ValueName)
		{
			return true;
		}
	}
	return false;
}

wxString Config::ReadStringValue(wxString ValueName,wxString Default,bool WriteDefaultValue)
{
	if (ValueExist(ValueName))
	{
		for (int i=0;i<Values.Count();i++)
		{
			if (Values[i]->IsCom==false && Values[i]->Name == ValueName)
			return Values[i]->Value;
		}
	}else{
		if (WriteDefaultValue)
			WriteStringValue(ValueName,Default);
		return Default;
	}
}

int Config::ReadIntValue(wxString ValueName,int Default,bool WriteDefaultValue)
{
	if (ValueExist(ValueName))
	{
		return atoi(ReadStringValue(ValueName).c_str());
	}else{
		if (WriteDefaultValue)
			WriteIntValue(ValueName,Default);
		return Default;
	}
}

float Config::ReadFloatValue(wxString ValueName,float Default,bool WriteDefaultValue)
{
	if (ValueExist(ValueName))
	{
		return atof(ReadStringValue(ValueName).c_str());
	}else{
		if (WriteDefaultValue)
			WriteFloatValue(ValueName,Default);
		return Default;
	}
}

void Config::WriteStringValue(wxString ValueName,wxString Value)
{
	if (ValueExist(ValueName))
	{
		for (int i=0;i<Values.Count();i++)
		{
			if (Values[i]->Name == ValueName)
			{
				Values[i]->Value = Value;
				break;
			}
		}
	}
	else{
		ValueSlot *slot = new ValueSlot();
		if (slot)
		{
			slot->Name = ValueName;
			slot->Value = Value;
			slot->IsCom=false;
			Values.Add(slot);
		}
	}
}

void Config::WriteIntValue(wxString ValueName,int Value)
{
	wxString text;
	text.Printf("%d",Value);
	WriteStringValue(ValueName,text);
}

void Config::WriteFloatValue(wxString ValueName,float Value)
{
	wxString text;
	text.Printf("%f",Value);
	WriteStringValue(ValueName,text);
}