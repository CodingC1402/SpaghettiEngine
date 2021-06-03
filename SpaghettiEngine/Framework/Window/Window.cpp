#include <Windows.h>
#include "Window.h"
#include "Monitor.h"

#define FULLSCREENSTYLE WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP
#define WINDOWSTYLE WS_SYSMENU | WS_MINIMIZEBOX | WS_CAPTION

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
	wc.hIcon = static_cast<HICON>(LoadImage(this->m_hInst, MAKEINTRESOURCE( IDI_ENGINEICON ), IMAGE_ICON, 32, 32, 0));
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = static_cast<HICON>(LoadImage(this->m_hInst, MAKEINTRESOURCE(IDI_ENGINEICON), IMAGE_ICON, 16, 16, 0));
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

Window::Window(int iWidth, int iHeight, const wchar_t* wcWndName, PWindow parent, int x, int y)
	:
	wndPos(x, y),
	wndSize(iWidth, iHeight),
	originalName(wcWndName),
	m_kbKeyInput(KeyBoard::GetInstance()),
	m_mMouseInput(Mouse::GetInstance()),
	parent(parent)
{
	if (parent)
		parent->AddChild(this);
	this->CreateWnd();
}

void Window::Destroy()
{
	DestroyWindow(m_hWnd);
	for (int i = 0; i < children.size(); i++)
	{
		delete children[i];
	}
}

void Window::AddChild(PWindow child)
{
	children.push_back(child);
}

void Window::OnSizeChanged(UINT width, UINT height)
{
	wndSize.width = width;
	wndSize.height = height;
}

void Window::OnMove(UINT x, UINT y)
{
	wndPos.x = x;
	wndPos.y = y;
}


Window::~Window()
{
	DestroyWindow( m_hWnd );
}

void Window::CreateWnd()
{
	if (parent)
	{
		m_hWnd = CreateWindowEx(
			0, WindowClass::GetName(), originalName.c_str(),
			WS_VISIBLE | WS_CHILD,
			wndPos.x, wndPos.y, wndSize.width, wndSize.height, parent->GetHwnd(), nullptr, WindowClass::GetInstance(), this
		);
	}
	else
	{
		Plane2D::Rectangle monitor = Monitor::GetPrimaryMonitorInfo();

		RECT wr;
		wr.left = 100;
		wr.right = wndSize.width + wr.left;
		wr.top = 100;
		wr.bottom = wndSize.height + wr.top;
		AdjustWindowRect(&wr, WINDOWSTYLE, FALSE);

		m_hWnd = CreateWindowEx(
			0, WindowClass::GetName(), originalName.c_str(),
			WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
			wr.left, wr.top, wr.right - wr.left, wr.bottom - wr.top,
			nullptr, nullptr, WindowClass::GetInstance(), this
		);
	}
}

void Window::ChangeWindowMode(bool isFullScreen)
{
	if (this->isFullScreen == isFullScreen)
		return;

	this->isFullScreen = isFullScreen;
	if (isFullScreen)
	{
		restoreRect.x = wndPos.x;
		restoreRect.y = wndPos.y;
		restoreRect.h = wndSize.height;
		restoreRect.w = wndSize.width;

		auto monitor = Monitor::GetCurrentMonitorInfo(GetHwnd());
		RECT wr;
		wr.left = monitor.x;
		wr.right = monitor.w + wr.left;
		wr.top = monitor.y;
		wr.bottom = monitor.h + wr.top;
		//AdjustWindowRect(&wr, style, FALSE);
		SetWindowLongPtrW(GetHwnd(), GWL_STYLE, WS_POPUP | WS_EX_TOPMOST | WS_VISIBLE);
		SetWindowPos(GetHwnd(), HWND_TOPMOST, wr.left, wr.top, wr.right - wr.left, wr.bottom - wr.top, SWP_SHOWWINDOW);
	}
	else
	{
		wndPos.x = restoreRect.x;
		wndPos.y = restoreRect.y;
		wndSize.height = restoreRect.h;
		wndSize.width = restoreRect.w;

		RECT wr;
		wr.left = wndPos.x;
		wr.right = wndSize.width + wr.left;
		wr.top = wndPos.y;
		wr.bottom = wndSize.height + wr.top;
		AdjustWindowRect(&wr, WINDOWSTYLE, FALSE);
		SetWindowLongPtrW(GetHwnd(), GWL_STYLE, WINDOWSTYLE);
		SetWindowPos(GetHwnd(), HWND_NOTOPMOST, wr.left, wr.top, wr.right - wr.left, wr.bottom - wr.top, SWP_SHOWWINDOW);
	}
}

void Window::SetHeight(int h) noexcept
{
	wndSize.height = h;

	if (isFullScreen)
	{
		restoreRect.h = h;
	}
	else
	{
		RECT wr;
		wr.left = 0;
		wr.right = wndSize.width + wr.left;
		wr.top = 0;
		wr.bottom = wndSize.height + wr.top;
		
		if (parent)
		{
			AdjustWindowRect(&wr, FULLSCREENSTYLE, FALSE);
		}
		else
		{
			AdjustWindowRect(&wr, WINDOWSTYLE, FALSE);
		}

		SetWindowPos(GetHwnd(), HWND_TOP, wndPos.x, wndPos.y, wr.right - wr.left, wr.bottom - wr.top, SWP_DRAWFRAME);
	}
}

void Window::SetBGBrush(int r, int g, int b) noexcept
{
	COLORREF bgColor = RGB(r % 256, g % 256, b % 256);
	if (bgBrush)
		DeleteObject(bgBrush);
	bgBrush = CreateSolidBrush(bgColor);
	SetClassLongPtr(m_hWnd, GCLP_HBRBACKGROUND, (LONG)bgBrush);
	InvalidateRect(m_hWnd, NULL, TRUE);
}

void Window::SetPos(int x, int y) noexcept
{
	int dx = x - wndPos.x;
	int dy = y - wndPos.y;

	wndPos.x = x;
	wndPos.y = y;

	SetWindowPos(GetHwnd(), HWND_TOP, wndPos.x, wndPos.y, wndSize.width, wndSize.height, SWP_DRAWFRAME);
}

void Window::SetWidth(int w) noexcept
{
	wndSize.width = w;

	if (isFullScreen)
	{
		restoreRect.w = w;
	}
	else
	{
		RECT wr;
		wr.left = 0;
		wr.right = wndSize.width + wr.left;
		wr.top = 0;
		wr.bottom = wndSize.height + wr.top;

		if (parent)
		{
			AdjustWindowRect(&wr, FULLSCREENSTYLE, FALSE);
		}
		else
		{
			AdjustWindowRect(&wr, WINDOWSTYLE, FALSE);
		}
		SetWindowPos(GetHwnd(), HWND_TOP, wndPos.x, wndPos.y, wr.right - wr.left, wr.bottom - wr.top, SWP_DRAWFRAME);
	}
}

bool Window::SetName(const wchar_t* wcWndName) noexcept
{
	originalName = wcWndName;
	return SetWindowText(m_hWnd, wcWndName);
}

bool Window::SetTempName(const wchar_t* wcTempName) const noexcept
{
	return SetWindowText(m_hWnd, wcTempName);
}

const wchar_t *Window::GetName() const noexcept
{
	return originalName.c_str();
}

bool Window::IsVisible() const noexcept
{
	return isVisible;
}

void Window::Show() noexcept
{
	if (isVisible)
		return;
	ShowWindow(m_hWnd, SW_SHOWDEFAULT);
	isVisible = true;
}

void Window::Hide() noexcept
{
	if (!isVisible)
		return;
	ShowWindow(m_hWnd, SW_HIDE);
	isVisible = false;
}

Point Window::GetPos() const noexcept
{
	return wndPos;
}

Size Window::GetSize() const noexcept
{
	return wndSize;
}

HWND Window::GetHwnd() const noexcept
{
	return m_hWnd;
}

PKeyBoard Window::GetKeyBoard() const noexcept
{
	return m_kbKeyInput;
}

PMouse Window::GetMouse() const noexcept
{
	return m_mMouseInput;
}

Window* Window::Create(int iWidth, int iHeight, const wchar_t* name, PWindow parent, int x, int y)
{
	return new Window(iWidth, iHeight, name, parent, x, y);
}
DWORD Window::ProcessMessages()
{
	MSG msg;

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) > 0)
	{
		if (msg.message == WM_QUIT || msg.message == WM_DESTROY)
		{
			return WM_QUIT;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
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
#pragma region MouseMessageHandler
	case WM_MOUSEMOVE:
	{
		const Point ptPos = MAKEPOINTS( lParam );
		if (ptPos.x >= 0 && ptPos.x < wndSize.width && ptPos.y >= 0 && ptPos.y < wndSize.height)
		{
			m_mMouseInput->OnMove( ptPos );
			if (!m_mMouseInput->IsInside())
			{
				SetCapture( hWnd );
				m_mMouseInput->OnEnter();
			}
		}
		else
		{
			if (wParam & (VK_LBUTTON | VK_RBUTTON))
			{
				m_mMouseInput->OnMove( ptPos );
			}
			else
			{
				ReleaseCapture();
				m_mMouseInput->OnLeave();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		const Point ptPos = MAKEPOINTS( lParam );
		m_mMouseInput->OnLeftPress( ptPos );
		m_kbKeyInput->OnKeyPressed( VK_LBUTTON, 0 );
		break;
	}
	case WM_LBUTTONUP:
	{
		const Point ptPos = MAKEPOINTS( lParam );
		m_mMouseInput->OnLeftRelease( ptPos );
		m_kbKeyInput->OnKeyRelease( VK_LBUTTON, 0 );
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const Point ptPos = MAKEPOINTS( lParam );
		m_mMouseInput->OnRightPress( ptPos );
		m_kbKeyInput->OnKeyPressed( VK_RBUTTON, 0 );
		break;
	}
	case WM_RBUTTONUP:
	{
		const Point ptPos = MAKEPOINTS( lParam );
		m_mMouseInput->OnRightRelease( ptPos );
		m_kbKeyInput->OnKeyRelease( VK_RBUTTON, 0 );
		break;
	}
	case WM_MBUTTONDOWN:
	{
		const Point ptPos = MAKEPOINTS( lParam );
		m_mMouseInput->OnMiddlePress( ptPos );
		m_kbKeyInput->OnKeyPressed( VK_MBUTTON, 0 );
		break;
	}
	case WM_MBUTTONUP:
	{
		const Point ptPos = MAKEPOINTS( lParam );
		m_mMouseInput->OnMiddleRelease( ptPos );
		m_kbKeyInput->OnKeyRelease( VK_MBUTTON, 0 );
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const Point ptPos = MAKEPOINTS( lParam );
		if (GET_WHEEL_DELTA_WPARAM( wParam ) > 0)
		{
			m_mMouseInput->OnWheelUp();
		}
		else if (GET_WHEEL_DELTA_WPARAM( wParam ) > 0)
		{
			m_mMouseInput->OnWheelDown();
		}
		break;
	}
#pragma endregion

#pragma region KeyBoardMessageHandler
	//Key board message
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		m_kbKeyInput->OnKeyPressed( static_cast<unsigned char>(wParam), lParam );
		break;
	case WM_SYSKEYUP:
	case WM_KEYUP:
		m_kbKeyInput->OnKeyRelease( static_cast<unsigned char>(wParam), lParam );
		break;
	case WM_CHAR:
		m_kbKeyInput->OnChar( static_cast<wchar_t>(wParam) );
		break;
	//End key board message
	case WM_CLOSE:
		PostQuitMessage( 0 );
		break;
	case WM_KILLFOCUS:
		m_kbKeyInput->OnLostFocus();
		break;
#pragma endregion
	case WM_MOVE:
		OnMove((short)LOWORD(lParam), (short)HIWORD(lParam));
		break;
	case WM_SIZE:
		OnSizeChanged(LOWORD(lParam), HIWORD(lParam));
		break;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

Window::Exception::Exception( int line, const char *file, HRESULT hr ) noexcept
	:
	CornException( line, file ),
	m_HResult( hr )
{}

const wchar_t *Window::Exception::What() const noexcept
{
	std::wostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const wchar_t *Window::Exception::GetType() const noexcept
{
	return L"Corn Window Exception (´-ω-`( _ _ )";
}

std::wstring Window::Exception::TranslateErrorCode( HRESULT hr ) noexcept
{
	LPWSTR msgBuf = nullptr;
	DWORD msgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
		reinterpret_cast<LPWSTR>(&msgBuf), 0, nullptr
	);
	if ( msgLen == 0 )
	{
		return L"Unidentified error code"; 
	}
	std::wstring errorString = msgBuf;
	LocalFree( msgBuf );
	return errorString;
}

HRESULT Window::Exception::GetErrorCode() const noexcept
{
	return m_HResult;
}

std::wstring Window::Exception::GetErrorString() const noexcept
{
	return TranslateErrorCode( m_HResult );
}
