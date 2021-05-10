#include "Debug.h"
#include "CornWnd.h"
#include <sstream>

void Debug::Log(const wchar_t* text)
{
	OutputDebugString( text );
}

void Debug::LogB( bool flag )
{
	if ( flag )
		Log( L"true\n" );
	else
		Log( L"false\n" );
}
