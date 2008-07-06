#include "wx/wx.h"

class DialogNewDL : public wxDialog
{
public:	 
	DialogNewDL(wxWindow *parent);

	int ShowModal();

	void OnButtonOkClick(wxCommandEvent &event);
	void OnButtonCancelClick(wxCommandEvent &event);

	void OnFocus(wxCommandEvent &event);

	void OnText(wxCommandEvent& event);

	void SetLink(wxString URL);
	wxString GetLink();
	wxString GetFilename();
	
private:
	DECLARE_EVENT_TABLE()

	enum
	{
		ID_DNDL_OK,
		ID_DNDL_CANCEL,
		ID_DNDL_LINK,
		ID_DNDL_FILENAME,
		ID_DNDL_MAX
	};

	wxButton *mOK;
	wxButton *mCancel;
	wxTextCtrl *mLink;
	wxTextCtrl *mFilename;
	//wxLabel *mLinkLabel;
	//wxLabel *mFIlenameLabel;

	wxString Link;
	wxString Filename;
};