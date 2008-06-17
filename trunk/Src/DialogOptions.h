#include "wx/wx.h"
#include "Config.h"

class DialogOptions : public wxDialog
{
public:	 
	DialogNewDL(wxWindow *parent,Config *config);

	int DialogOptions();

	void OnButtonOkClick(wxCommandEvent &event);
	void OnButtonCancelClick(wxCommandEvent &event);

	void OnFocus(wxCommandEvent &event);

	void OnText(wxCommandEvent& event);

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

	Config *mConfig;

	wxButton *mOK;
	wxButton *mApply;
	wxButton *mCancel;
	wxTextCtrl *mLink;
	wxTextCtrl *mFilename;



	wxString Link;
	wxString Filename;
};