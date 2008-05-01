#include "wxVistagauge.h"

BEGIN_EVENT_TABLE(wxVistagauge, wxGauge)
	EVT_PAINT(wxVistagauge::OnPaint)
END_EVENT_TABLE()

wxVistagauge::wxVistagauge(wxWindow *parent,
              wxWindowID id,
              int range,
              const wxPoint& pos ,
              const wxSize& size ,
              long style ,
              const wxValidator& validator ,
              const wxString& name )
	:wxGauge(parent,id,range,pos,wxSize(size.x,18),style,validator,name)
{
	
}

void wxVistagauge::OnPaint(wxPaintEvent &event)
{
	//wxGauge::OnPaint(event);

	wxBitmap begin("IDB_WXVISTAGAUGE_BEGIN");
	wxBitmap middle("IDB_WXVISTAGAUGE_MIDDLE");
	wxBitmap end("IDB_WXVISTAGAUGE_END");

	wxBitmap blockbegin("IDB_WXVISTAGAUGE_BLOCKBEGIN");
	wxBitmap blockmiddle("IDB_WXVISTAGAUGE_BLOCKMIDDLE");
	wxBitmap blockend("IDB_WXVISTAGAUGE_BLOCKEND");

	int w,h;
	GetSize(&w,&h);

	int maxw=(w-2)*(GetValue()/(float)GetRange());

	middle = wxBitmap(middle.ConvertToImage().Scale(w-20,18));

	blockmiddle = wxBitmap(blockmiddle.ConvertToImage().Scale(maxw-6*2,14));
	

	/*wxImage img = begin.ConvertToImage();
	img.SetMaskColour(240,244,250);
	begin = wxBitmap(img);*/

	wxPaintDC vDc(this);

	PrepareDC(vDc);

	vDc.DrawBitmap(begin,0,0);
	vDc.DrawBitmap(middle,10,0);
	vDc.DrawBitmap(end,w-10,0);

	if (maxw>=12)
	{
		vDc.DrawBitmap(blockbegin,2,2);
		vDc.DrawBitmap(blockmiddle,8,2);
		vDc.DrawBitmap(blockend,maxw-6,2);
	}

	wxString text("");
	text.Printf("%.0f %c",(float)GetValue()/GetRange()*100,'%');
	wxFont font = vDc.GetFont();
	font.SetPointSize(8);
	font.SetWeight(wxFONTWEIGHT_BOLD);
	font.SetFamily(wxFONTFAMILY_MODERN);
	font.SetNoAntiAliasing(false);
	vDc.SetFont(font);

	int width=vDc.GetTextExtent(text).x;
	int height=vDc.GetTextExtent(text).y;
	vDc.DrawText(text,w/2-width/2,h/2-height/2-1);
	
}