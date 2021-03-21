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
	MSG msg;
	BOOL iResult;
	while ( (iResult = GetMessage( &msg, nullptr, 0, 0)) > 0 )
	{

		TranslateMessage( &msg );
		DispatchMessage( &msg );
		//=================================={Where you put your game loop}=======================================

		//======================================================================================================
	}

	if ( iResult == -1 )
		return -1;
	else
		return msg.wParam;
}