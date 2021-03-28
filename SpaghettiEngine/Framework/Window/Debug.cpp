#include "Debug.h"
#include "CornWnd.h"

void Debug::Log(const wchar_t* text)
{
	OutputDebugString( text );
}

void Debug::LogF( float number )
{
	Log( std::to_wstring(number).c_str() + L'\n');
}

void Debug::LogB( bool flag )
{
	if ( flag )
		Log( L"true\n" );
	else
		Log( L"false\n" );
}
