#include "wx/wx.h"
#include "wx/gauge.h"


class wxVistagauge : public wxGauge
{
public:
	wxVistagauge(wxWindow *parent,
              wxWindowID id,
              int range,
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize,
              long style = wxGA_HORIZONTAL,
              const wxValidator& validator = wxDefaultValidator,
              const wxString& name = wxGaugeNameStr);

	void OnPaint(wxPaintEvent &event);

	DECLARE_EVENT_TABLE()
};