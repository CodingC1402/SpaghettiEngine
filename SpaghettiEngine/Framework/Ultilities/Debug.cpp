#include "Debug.h"
#include "CornWnd.h"
#include <sstream>

void Debug::Log(const wchar_t* text)
{
	OutputDebugString( text );
}

void Debug::LogF( float number )
{
	std::wostringstream oss;
	oss << number << std::endl;
	Log(oss.str().c_str());
}

void Debug::LogB( bool flag )
{
	if ( flag )
		Log( L"true\n" );
	else
		Log( L"false\n" );
}
