#pragma once
#include "CornWnd.h"
#include "Mouse.h"
#include "KeyBoard.h"
#include "Debug.h"
#include "Timer.h"
#include "CornException.h"
#include "StringConverter.h"
#include "..\..\resource.h"
#include <memory>

typedef class Window *PWindow;
using Plane2D::Size;

class Window
{
public:
	enum class WindowMode
	{
		FullScreen,
		Window
	};

	class Exception : public CornException
	{
	public:
		Exception(int line, const char* file, HRESULT hr) noexcept;
		virtual const wchar_t* GetType() const noexcept override;
		virtual const wchar_t* What() const noexcept override;
		static std::wstring TranslateErrorCode(HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::wstring GetErrorString() const noexcept;
	private:
		HRESULT m_HResult;
	};
private:
	// Singleton class
	class WindowClass
	{
	public:
		static const wchar_t* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		Window& operator=(const WindowClass&) = delete;
		static constexpr const wchar_t* m_pwcWndClassName = L"SpaghettiEngine";
		static WindowClass m_wcWinClass;
		HINSTANCE m_hInst;
	};
public:
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	void ChangeWindowMode(WindowMode mode) noexcept;

	bool SetName(const wchar_t* wcWndName) noexcept;
	bool SetTempName(const wchar_t* wcTempName) const noexcept;
	const wchar_t* GetName() const noexcept;

	bool IsVisible() const noexcept;
	void Show() const noexcept;
	void Hide() const noexcept;

	Size GetSize() const noexcept;
	HWND GetHwnd() const noexcept;
	PKeyBoard GetKeyBoard() const noexcept;
	PMouse GetMouse() const noexcept;

	static Window* Create(int iWidth = 800, int iHeight = 600, WindowMode mode = WindowMode::Window, const wchar_t* name = L"DefaultWindow", int x = 0, int y = 0);
	static DWORD ProcessMessages();
private:
	Window(int x, int y,int iWidth, int iHeight, WindowMode mode, const wchar_t* iName) noexcept;
	void Destroy();

	void CreateWnd();

	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	Point wndPos;
	Size wndSize;
	RECT restoreRect;
	WindowMode mode;
	bool isVisible = false;

	std::wstring originalName;
	HWND m_hWnd;
	mutable PKeyBoard m_kbKeyInput;
	mutable PMouse m_mMouseInput;
};

typedef std::shared_ptr<Window> SWindow;
typedef std::unique_ptr<Window> UWindow;

#define CWND_EXCEPT( hr ) Window::Exception(__LINE__,__FILE__,hr)
#define CWND_LAST_EXCEPT() Window::Exception(__LINE__,__FILE__,GetLastError())