#include <Windows.h>
#include <d3d9.h>
#include <iostream>
#include "Game.h"

LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch ( msg )
	{
	case WM_CLOSE:
		PostQuitMessage( 0 );
		break;
	case WM_KEYDOWN:
		break;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

//Entry point
int WINAPI wWinMain(
	HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	PWSTR		pCmdLine,
	int			nCmdShow
)
{
	const LPCWSTR pClassName = L"SpaghettiEngine";
	//Register window class;
	WNDCLASSEX wc = { 0 };
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.cbSize = sizeof( WNDCLASSEX );
	wc.hInstance = hInstance;
	wc.hCursor = nullptr;
	wc.hIcon = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = pClassName;
	wc.hIconSm = nullptr;

	RegisterClassEx( &wc );
	//Create window instance;
	HWND hWnd = CreateWindowEx(
		0, pClassName,
		L"Spaghetti Engine",
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
		nullptr, nullptr, hInstance, nullptr
	);

	if ( hWnd == nullptr )
	{
		OutputDebugString( L"Window didn't created properly" );
		return 1;
	}

	ShowWindow( hWnd, SW_SHOW );

	//Message pump
	MSG msg;
	BOOL iResult;
	while ( (iResult = GetMessage( &msg, nullptr, 0, 0)) > 0 )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	if ( iResult == -1 )
		return -1;
	else
		return msg.wParam;
}