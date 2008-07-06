#include <string>
#include "Parser.h"
#include "wx/wx.h"

enum
{
	STATE_NONE,
	STATE_SIZE,
};


bool Parser::IsValidLink(wxString link)
{
	if (!strncmp(link.c_str(),"http://",7))
		return true;
	else if (!strncmp(link.c_str(),"ftp://",6))
		return false;
	else
		return false;
}
bool Parser::IsFreeLink(wxString link)
{
	if (!strncmp(link.c_str(),"http://dl.free.fr/",18))
		return true;

	return false;
}

long Parser::GetFileSizeHTTP(wxString header)
{
	// On récupére la taille du fichier finale

	std::string temp((char*)header.c_str());

	int pos = temp.rfind("Content-Range:");
	int max = pos;
	int min = pos;

	if (pos==-1)
		return -1;

	while (max<=temp.length())
	{
		if (temp[max]=='/')
			min=max+1;

		if (temp[max]=='\n' || temp[max]=='\r')
			break;
		max++;
	}
	
	if (max-min<=0)
		return -1;

	long iFileSize=atoi(temp.substr(min,max-min).c_str());

	return iFileSize;
}

wxString Parser::GetFilenameFromURL(wxString url)
{
	std::string temp((char*)url.c_str());
	int min,max,pos;
	min=0;	

	max=temp.length();

	wxLogDebug("min %d max %d",min,max);

	min = temp.find_last_of("/")+1;
	wxLogDebug("min %d max %d",min,max);
	wxLogDebug("\\\\ %d",temp.find_last_of("\\"));
	pos = temp.find_last_of("\\");
	if (pos!=-1 && pos+1>min)
		min = pos;

	wxLogDebug("min %d max %d",min,max);

	pos = temp.find_last_of("?");
	if (pos!=-1 && pos>min && pos < max)
		max = pos;

	temp = temp.substr(min,max-min);
	//char *filename = (char*)malloc(temp.length());
	//strcpy(filename,temp.c_str());
	//wxLogDebug("File name : %s",filename);

	return wxString(temp);
}

wxString Parser::FirstLine(wxString text)
{
	std::string temp(text);
	int pos1=temp.find_first_of("\n");
	int pos2=temp.find_first_of("\r");
	//wxLogMessage("FirstLine %d %d",pos1,pos2);
	if (pos1==-1 && pos2==-1)
		return text;
	else if (pos1==-1)
	{
		//wxLogMessage("FirstLine %s",temp.substr(0,pos2));
		return temp.substr(0,pos2);
	}else if (pos2==-1)
	{
		//wxLogMessage("FirstLine %s",temp.substr(0,pos1));
		return temp.substr(0,pos1);
	}else if (pos2<pos1)
	{
		//wxLogMessage("FirstLine %s",temp.substr(0,pos2));
		return temp.substr(0,pos2);
	}else{
		//wxLogMessage("FirstLine %s",temp.substr(0,pos1));
		return temp.substr(0,pos1);
	}
}