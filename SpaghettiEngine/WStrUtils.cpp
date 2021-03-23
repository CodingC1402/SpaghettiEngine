#include "WStrUtils.h"
#include <iterator>

PWSTR WStrUtils::ToString( float number )
{ 
    std::string text = std::to_string( number );
    return ConvertString( text );
}

LPWSTR WStrUtils::ConvertString( const std::string& instr )
{
    int bufferlen = MultiByteToWideChar( CP_ACP, 0, instr.c_str(), instr.size(), NULL, 0 );

    if ( bufferlen == 0 )
    {
        return 0;
    }

    LPWSTR widestr = new WCHAR[ bufferlen + 1 ];

    MultiByteToWideChar( CP_ACP, 0, instr.c_str(), instr.size(), widestr, bufferlen );

    widestr[ bufferlen ] = 0;

    return widestr;
}

LPWSTR WStrUtils::Clone( LPCWSTR str )
{
    std::vector<wchar_t> thecopy( wcslen( str ) + 1 );
    wcscpy_s( thecopy.data(), thecopy.size(), str );

    return thecopy.data();
}

bool WStrUtils::ConvertString( std::string &s, const LPWSTR pw, UINT codepage = CP_ACP )
{
    bool res = false;
    char *p = 0;
    int bsz;

    bsz = WideCharToMultiByte( codepage,
        0,
        pw, -1,
        0, 0,
        0, 0 );
    if ( bsz > 0 ) {
        p = new char[bsz];
        int rc = WideCharToMultiByte( codepage,
            0,
            pw, -1,
            p, bsz,
            0, 0 );
        if ( rc != 0 ) {
            p[bsz - 1] = 0;
            s = p;
            res = true;
        }
    }
    delete[] p;
    return res;
}


