#include "WStrUtils.h"

PWSTR WStrUtils::ToString( int number )
{
	PWSTR wsReturn;
	bool bMinus = number < 0 ? true : false;
	int iNumOfChar = 1;
	int iCopyNumber = abs(number);

	while ( iCopyNumber > 9 )
	{
		iCopyNumber /= 10;
		iNumOfChar++;
	}

	iNumOfChar += bMinus;
	wsReturn = new WCHAR[iNumOfChar];
	int i = bMinus;
	if ( bMinus )
	{
		wsReturn[ 0 ] = L"";
	}

	return wsReturn;
}

PWSTR WStrUtils::ToString( float number )
{ 

	return PWSTR();
}
