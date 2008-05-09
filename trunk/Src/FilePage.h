#include "wx/wx.h"
#include "wxVistaGauge.h"
#include "ProgressBlocks.h"

class FilePage : public wxPanel//wxWindow
{
public:
	FilePage(wxWindow *parent);

	void SetGaugeValue(float value);

	void SetFileSize(long size);
	void SetDownloadedSize(long size);
	void SetBlockSize(int size);

private:
	wxVistaGauge *mGauge;
	ProgressBlocks *mBlocks;

};