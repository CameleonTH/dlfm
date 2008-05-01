#include "DialogNewDL.h"
#include "wx/dataobj.h"
#include "wx/clipbrd.h"
#include "Parser.h"


BEGIN_EVENT_TABLE(DialogNewDL,wxDialog)
	//EVT_CLOSE(MAp::OnClose)
	EVT_BUTTON(ID_DNDL_OK,DialogNewDL::OnButtonOkClick)
	EVT_BUTTON(ID_DNDL_CANCEL,DialogNewDL::OnButtonCancelClick)
	EVT_TEXT(wxID_ANY,DialogNewDL::OnText)
END_EVENT_TABLE()

DialogNewDL::DialogNewDL(wxWindow *parent)
	: wxDialog(parent,wxID_ANY,wxString("New download"),wxDefaultPosition,wxSize(250,400))
{
	mLink=NULL;
	mFilename=NULL;
	mOK = new wxButton(this,ID_DNDL_OK,wxString("OK"),wxPoint(10,330),wxSize(75,25));

	mCancel = new wxButton(this,ID_DNDL_CANCEL,wxString("Cancel"),wxPoint(95,330),wxSize(75,25));

	mLink = new wxTextCtrl(this,ID_DNDL_LINK,wxString(""),wxPoint(10,10),wxSize(220,25));
	//mLink->SetValue(_T("http://dl.free.fr/getfile.pl?file=/mceva576/Final.rar"));

	mFilename = new wxTextCtrl(this,ID_DNDL_FILENAME,wxString(""),wxPoint(10,50),wxSize(220,25));

	wxTheClipboard->UsePrimarySelection();

	if (wxTheClipboard->Open())
	{
		wxTextDataObject data;

		if (wxTheClipboard->IsSupported( data.GetFormat() ))
		{
			if (wxTheClipboard->GetData( data ))
			{
				if (wxTheClipboard->IsSupported(wxDF_TEXT))
				{
					//wxLogMessage(data.GetText());
					if (Parser::IsValidLink((char*)data.GetText().c_str()))					
						mLink->SetValue(data.GetText());
					else
						mLink->SetValue("http://");
				}
			}
		}
		wxTheClipboard->Close();
	}
}

int DialogNewDL::ShowModal()
{
	Center();
	return wxDialog::ShowModal();
}

void DialogNewDL::OnButtonOkClick(wxCommandEvent &event)
{
	bool FileExist=false;
	if (Parser::IsValidLink((char*)mLink->GetValue().c_str()) && mFilename->GetValue()!="" && FileExist==false)
	{
		Link = mLink->GetValue();
		Filename = mFilename->GetValue();
		EndModal(0);
	}else{
		if (FileExist)
			wxLogMessage("Un fichier porte déja ce nom");
		else if (mFilename->GetValue()=="")
			wxLogMessage("Nom de fichier invalide");
		else
			wxLogMessage("URL invalide");
	}
}

void DialogNewDL::OnButtonCancelClick(wxCommandEvent &event)
{
	//width = WxSpinCtrlWidth->GetValue();
	//height = WxSpinCtrHeight->GetValue();
	Link = "";
	EndModal(1);
}

wxString DialogNewDL::GetLink()
{
	return Parser::FirstLine(Link);
}

wxString DialogNewDL::GetFilename()
{
	return Parser::FirstLine(Filename);
}

void DialogNewDL::OnFocus(wxCommandEvent &event)
{
	return;
	wxLogDebug("Focus");
	if (mLink->GetValue()!=Link)
	{
		Link=Parser::FirstLine(mLink->GetValue());
		mFilename->SetValue(Parser::GetFilenameFromURL(Link));
	}
}

void DialogNewDL::OnText(wxCommandEvent& event)
{
	wxLogDebug("Text");
	if (event.GetId()==ID_DNDL_LINK && mLink!=NULL && mFilename!=NULL)
	{
		wxString temp(Parser::FirstLine(mLink->GetValue()));
		wxLogDebug(temp);
		if (temp!=Link)
		{	
			Link=temp;
			mFilename->SetValue(Parser::GetFilenameFromURL((char*)Link.c_str()));
		}
	}
}