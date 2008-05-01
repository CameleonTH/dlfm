#include "StateData.h"

StateData::StateData(void)
{
	Type=DT_NONE;
}

Header::Header(void)
{
	Type=DT_HEADER;
}


Page::Page(void)
{
	Type=DT_PAGE;
}

FileData::FileData(void)
{
	Type=DT_FILE_DATA;
}
