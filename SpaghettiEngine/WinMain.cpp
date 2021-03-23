#include "Window.h"
#include <string>

//Entry point
int WINAPI wWinMain(
	HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	PWSTR		pCmdLine,
	int			nCmdShow
)
{
	//Message pump
	try
	{
		MSG msg;
		BOOL iResult;
		while ( (iResult = GetMessage( &msg, nullptr, 0, 0 )) > 0 )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		if ( iResult == -1 )
			return -1;
		else
			return msg.wParam;
	}
	catch ( const CornException& e )
	{
		
	}
	return 1;
}