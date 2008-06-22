#include "ArgumentParser.h"

ArgumentParser::ArgumentParser(wxString Arguments)
{

}

void ArgumentParser::AttachManager(DLManager *manager)
{
	Manager = manager;
}