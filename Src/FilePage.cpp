#include "FilePage.h"

FilePage::FilePage(wxWindow *parent)
	//:wxWindow(parent,wxID_ANY)
	:wxPanel(parent,wxID_ANY)
{
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(mainSizer);

	mGauge = new wxVistaGauge(this,wxID_ANY,100);
	//mGauge->SetMaxSize(wxSize(250,18));
	mBlocks = new ProgressBlocks(this,wxID_ANY);

	mainSizer->Add(mGauge, 0, wxEXPAND);
	mainSizer->Add(mBlocks, 1, wxEXPAND);
}

void FilePage::SetGaugeValue(float value)
{
	if (mGauge)
	{
		mGauge->SetValue(value);
	}
}

void FilePage::SetFileSize(long size)
{
	if (mBlocks)
	{
		mBlocks->SetTotalData(size);
	}
}

void FilePage::SetDownloadedSize(long size)
{
	if (mBlocks)
		mBlocks->SetProgressData(size);
}

void FilePage::SetBlockSize(int size)
{
	if (mBlocks)
		mBlocks->SetBlockSize(size);
}