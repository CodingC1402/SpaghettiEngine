#include <d3d9.h>
#include "Window.h"

//Entry point
int WINAPI wWinMain(
	HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	PWSTR		pCmdLine,
	int			nCmdShow
)
{
	Window wndMain( 1600, 900, L"Spaghetti DirectX Engine" );
	//Message pump
	MSG msg;
	BOOL iResult;
	while ( (iResult = GetMessage( &msg, nullptr, 0, 0)) > 0 )
	{
		Time::UpdateTime();

		TranslateMessage( &msg );
		DispatchMessage( &msg );
		//=================================={Where you put your game loop}=======================================

		if ( wndMain.m_kbKeyInput.KeyIsPress( VK_MENU ) )
		{
			MessageBox( nullptr, L"bruh", L"lol", 0 );
		}
		else
		{
			wndMain.SetText( L"lol" );
		}

		//======================================================================================================
	}

	if ( iResult == -1 )
		return -1;
	else
		return msg.wParam;
}