#include "WStrUtils.h"

PWSTR WStrUtils::ToString( float number )
{ 
    std::string text = std::to_string( number );
    return ConvertString( text );
}

LPWSTR WStrUtils::ConvertString( const std::string& instr )
{
    int bufferlen = ::MultiByteToWideChar( CP_ACP, 0, instr.c_str(), instr.size(), NULL, 0 );

    if ( bufferlen == 0 )
    {
        return 0;
    }

    LPWSTR widestr = new WCHAR[ bufferlen + 1 ];

    ::MultiByteToWideChar( CP_ACP, 0, instr.c_str(), instr.size(), widestr, bufferlen );

    widestr[ bufferlen ] = 0;

    return widestr;
}

LPWSTR WStrUtils::Clone( LPCWSTR str )
{
    std::vector<wchar_t> thecopy( wcslen( str ) + 1 );
    wcscpy_s( thecopy.data(), thecopy.size(), str );

    return thecopy.data();
}
