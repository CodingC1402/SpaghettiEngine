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
#include <vector>

typedef class Window *PWindow;
using Plane2D::Size;

class Window
{
public:
	class Exception : public CornException
	{
	public:
		Exception(int line, const char* file, HRESULT hr) noexcept;
		virtual const wchar_t* GetType() const noexcept override;
		virtual const wchar_t* What() const noexcept override;
		static std::wstring TranslateErrorCode(HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::wstring GetErrorString() const noexcept;
	protected:
		HRESULT m_HResult;
	};
protected:
	// Singleton class
	class WindowClass
	{
	public:
		WindowClass(const WindowClass&) = delete;
		Window& operator=(const WindowClass&) = delete;
		
		static const wchar_t* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	protected:
		WindowClass() noexcept;
		~WindowClass();
		static constexpr const wchar_t* m_pwcWndClassName = L"SpaghettiEngine";
		static WindowClass m_wcWinClass;
		HINSTANCE m_hInst;
	};
public:
	virtual ~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	void ChangeWindowMode(bool isFullScreen);

	void SetHideCursor(bool value) noexcept;
	void Focus() noexcept;
	void SetIsFocusOnClick(bool value) noexcept;
	void SetPos(int x, int y) noexcept;
	void SetWidth(int w) noexcept;
	void SetHeight(int h) noexcept;
	void SetBGBrush(int r, int g, int b) noexcept;
	bool SetName(const wchar_t* wcWndName) noexcept;
	bool SetTempName(const wchar_t* wcTempName) const noexcept;

	const wchar_t* GetName() const noexcept;

	bool IsVisible() const noexcept;
	void Show() noexcept;
	void Hide() noexcept;

	[[nodiscard]] bool		IsHideCursor() const noexcept;
	[[nodiscard]] bool		IsFocusOnClick() const noexcept;
	[[nodiscard]] Point		GetPos() const noexcept;
	[[nodiscard]] Size		GetSize() const noexcept;
	[[nodiscard]] HWND		GetHwnd() const noexcept;
	[[nodiscard]] PKeyBoard GetKeyBoard() const noexcept;
	[[nodiscard]] PMouse	GetMouse() const noexcept;

	static Window* Create(int iWidth = 800, int iHeight = 600, const wchar_t* name = L"DefaultWindow", PWindow parent = nullptr, int x = 0, int y = 0);
	static DWORD ProcessMessages();
protected:
	Window(int iWidth = 800, int iHeight = 600, const wchar_t* name = L"DefaultWindow", PWindow parent = nullptr, int x = 0, int y = 0);
	void Destroy();
	void AddChild(PWindow child);

	void virtual OnSizeChanged(UINT width, UINT height);
	void virtual OnMove(UINT x, UINT y);

	void CreateWnd();

	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
protected:
	Plane2D::Rectangle restoreRect;
	Point wndPos;
	Size wndSize;
	Size wndContentSize;

	bool _isHideCursor = false;
	bool _isFocusOnClick = false;
	bool isFullScreen = false;
	bool isVisible = false;

	HBRUSH bgBrush = nullptr;

	PWindow parent;
	std::vector<PWindow> children;

	std::wstring originalName;
	HWND m_hWnd;
	mutable PKeyBoard m_kbKeyInput;
	mutable PMouse m_mMouseInput;
};

typedef std::shared_ptr<Window> SWindow;
typedef std::unique_ptr<Window> UWindow;

#define CWND_EXCEPT( hr ) Window::Exception(__LINE__,__FILE__,hr)
#define CWND_LAST_EXCEPT() Window::Exception(__LINE__,__FILE__,GetLastError())