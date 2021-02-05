#include "Debug.h"

void Debug::Log(std::string text)
{
	PWSTR wText = WStrUtils::ConvertString( text );
	OutputDebugString( wText );
	delete[] wText;
}

void Debug::LogF( float number )
{
	Log( std::to_string( number ) + "\n" );
}

void Debug::LogB( bool flag )
{
	if ( flag )
		Log( "true\n" );
	else
		Log( "false\n" );
}
