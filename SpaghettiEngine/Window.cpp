#include "Window.h"

Window::WindowClass Window::WindowClass::m_wcWinClass;

Window::WindowClass::WindowClass() noexcept
	:
	m_hInst( GetModuleHandle( nullptr ) )
{
	WNDCLASSEX wc = { 0 };
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = Window::HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.cbSize = sizeof( WNDCLASSEX );
	wc.hInstance = GetInstance();
	wc.hCursor = nullptr;
	wc.hIcon = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = nullptr;
	RegisterClassEx( &wc );
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass( GetName(), GetInstance() );
}

const wchar_t* Window::WindowClass::GetName() noexcept
{
	return m_pwcWndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return m_wcWinClass.m_hInst;
}

Window::Window( int iWidth, int iHeight, const wchar_t* wcWndName ) noexcept
{
	RECT wr;
	wr.left = 100;
	wr.right = iWidth + wr.left;
	wr.top = 100;
	wr.bottom = iHeight + wr.top;
	AdjustWindowRect( &wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE );

	m_hWnd = CreateWindowEx(
		0, WindowClass::GetName(),  wcWndName,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);

	ShowWindow( m_hWnd, SW_SHOWDEFAULT );
}

Window::~Window()
{
	DestroyWindow( m_hWnd );
}

bool Window::SetText( const wchar_t* wcWndName )
{
	return SetWindowText( m_hWnd, wcWndName );
}

// Init handler
LRESULT CALLBACK Window::HandleMsgSetup( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if ( msg == WM_NCCREATE )
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd) );
		SetWindowLongPtr( hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk) );

		return pWnd->HandleMsg( hWnd, msg, wParam, lParam );
	}
	
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

// Adapter from win32 to c++
LRESULT CALLBACK Window::HandleMsgThunk( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr( hWnd, GWLP_USERDATA ));
	return pWnd->HandleMsg( hWnd, msg, wParam, lParam );
}

// C++ Handler
LRESULT Window::HandleMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch ( msg )
	{
	case WM_CLOSE:
		PostQuitMessage( 0 );
		break;
	case WM_KEYDOWN:
		KeyBoard::UpdateKeyState( wParam, lParam, true );
		break;
	case WM_KEYUP:
		KeyBoard::UpdateKeyState( wParam, lParam, false );
		break;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}
