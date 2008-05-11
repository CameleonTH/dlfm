#include "wx/wx.h"


class ProgressBlocks : public wxScrolledWindow
{
public:
	ProgressBlocks(wxWindow *parent,
              wxWindowID id,
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize);
	
	void OnPaint(wxPaintEvent &event);
	void OnSize( wxSizeEvent &event );
	void OnScroll(wxScrollWinEvent& event);
	void EraseBackGround(wxEraseEvent &event);

	void SetBlockSize(int size) { BlockSize = size; }
	void SetTotalData(long total);
	void SetProgressData( long progress) { if (ProgressData != progress) ProgressData = progress; }

	DECLARE_EVENT_TABLE()
private:
	long	TotalData;
	long	ProgressData;
	long	LastProgressData;
	int		BlockSize;

	bool ScrollUdpate;
};