#include "ProgressBlocks.h"

BEGIN_EVENT_TABLE(ProgressBlocks, wxScrolledWindow)
	EVT_PAINT(ProgressBlocks::OnPaint)
	EVT_SIZE(ProgressBlocks::OnSize)
	EVT_SCROLLWIN(ProgressBlocks::OnScroll)
	EVT_ERASE_BACKGROUND(ProgressBlocks::EraseBackGround)
END_EVENT_TABLE()

ProgressBlocks::ProgressBlocks(wxWindow *parent,
              wxWindowID id,
              const wxPoint& pos,
              const wxSize& size)
			  : wxScrolledWindow(parent,id,pos,size)
{
	 TotalData=0;
	 ProgressData=0;
	 LastProgressData=0;
	 BlockSize=20480;
	 SetScrollbars(1,18,1,18,1,1);
	 ScrollUdpate=true;
	 SetBackgroundColour(*wxWHITE);
}

void ProgressBlocks::SetTotalData(long total)
{
	if (TotalData!=total) 
	{
		ScrollUdpate=true;
		TotalData = total; 
	}
}

void ProgressBlocks::EraseBackGround(wxEraseEvent &event) // Fonction vide
{

}

void ProgressBlocks::OnPaint(wxPaintEvent &event)
{
	wxScrolledWindow::OnPaint(event);

	wxBitmap blue("IDB_BLOCK_BLUE");
	wxBitmap grey("IDB_BLOCK_GREY");

	wxPaintDC vDc(this);

	PrepareDC(vDc);

	wxPen mGrayPen ( wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE), 1, wxSOLID );
	vDc.SetPen(mGrayPen);

	wxBrush mGrayBrush( wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE), wxSOLID );
	vDc.SetBrush(mGrayBrush);

	int w,h;
	GetSize(&w,&h);

	int lw,lh,lht;

	/*float temp=((float)w)/blue.GetWidth()-w/blue.GetWidth();
	wxLogMessage("%f",temp);

	if (temp!=0)
		lw = w/blue.GetWidth()-2;
	else*/
		lw = w/blue.GetWidth()-1;

	lh = h/blue.GetHeight();

	long blocks = TotalData/BlockSize;

	lht=blocks/lw;

	wxLogDebug("W %d H %d",w,h);
	wxLogDebug("W %d H %d",lw,lh);

	long scroll = this->GetScrollPos(1);
	wxLogDebug("Scroll %d",scroll);
	long Data=scroll*(lw);
	bool exit=false;
	for (int i=0;i<lh+1 && !exit;i++)		 //+1 en cas de scroll progressif
	{
		for (int j=0;j<lw && !exit;j++)
		{
			if (Data*BlockSize>=TotalData)
			{
				vDc.DrawRectangle(blue.GetWidth()*j,blue.GetHeight()*i+blue.GetHeight()*scroll,blue.GetWidth(),blue.GetHeight());
				//exit=true;
				//break;
				//break;
				wxLogDebug("Pos %d",blue.GetHeight()*i+blue.GetHeight()*scroll);
			}else{
				if ((Data)*BlockSize<=ProgressData)
					vDc.DrawBitmap(blue,blue.GetWidth()*j,blue.GetHeight()*i+blue.GetHeight()*scroll);
				else
					vDc.DrawBitmap(grey,blue.GetWidth()*j,blue.GetHeight()*i+blue.GetHeight()*scroll);				
			}
			Data++;
		}
	}

	if (ScrollUdpate==true)
	{
		this->SetScrollbars(1,blue.GetHeight(),1,lht+1,0,0);
		ScrollUdpate=false;
	}
}

void ProgressBlocks::OnSize( wxSizeEvent &event )
{
	wxLogDebug("On Size");
	event.Skip();
	ScrollUdpate=true;
	OnPaint(wxPaintEvent());
}

void ProgressBlocks::OnScroll(wxScrollWinEvent& event)
{
	wxLogDebug("On Scroll");
	HandleOnScroll(event);
	OnPaint(wxPaintEvent());
}