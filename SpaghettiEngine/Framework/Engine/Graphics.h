#pragma once
#include "CornDirectX.h"
#include "CornException.h"
#include "GameWnd.h"
#include "SpriteRenderer.h"
#include "Camera.h"
#include <vector>

/// <summary>
/// Singleton directx9 wrapper
/// </summary>

typedef class Camera* PCamera;
typedef class SpriteRenderer* PSpriteRenderer;
typedef class Graphics* PGraphics;

class Graphics
{
	friend class App;
public:
	enum class ColorFormat
	{
		DEFAULT = 0,

		ARGB32Bit = 21,
		RGB32Bit = 22
	};

	class GraphicException : public CornDiscriptionException
	{
	public:
		GraphicException(int line, const char* file, std::wstring discription) noexcept;
		virtual const wchar_t* GetType() const noexcept override;
	};
	class GraphicCodeException : public CornException
	{
	public:
		GraphicCodeException(int line, const char* file, HRESULT code) noexcept;
		virtual const wchar_t* GetType() const noexcept override;
		virtual const wchar_t* What() const noexcept override;
		virtual const wchar_t* Translate() const noexcept;
		virtual const HRESULT GetErrorCode() noexcept;
	private:
		HRESULT code;
	};
public:
	static PGraphics GetInstance();
	static void ToFullScreenMode();
	static void ToWindowMode();
	static void Draw(const PSpriteRenderer renderScript); // Render Sprite
	static void LoadTexture(PDx9Texture& rTexture, const std::string& path, const D3DCOLOR& keyColor);
	static void AddCamera(PCamera camera);
	static void RemoveCamera(PCamera camera);
protected:
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;

	void CreateResource();
	void ReleaseResource();

	bool FullScreen();
	void Window();
	SGameWnd GetCurrentWindow() const noexcept;

	void Init(STimer timer, int fps, ColorFormat colorFormat);

	HRESULT Begin() noexcept;
	void Render();
	bool End();
	bool Reset();

	void UpdateCurrentVideoAdapter();

	Graphics() noexcept;
	~Graphics() noexcept;
protected:
	SGameWnd wnd;
	RECT restoreRec;
	bool isFullScreen = false;
	Size resolution;

	Renderer renderer = NULL;
	RenderDevice renderDevice = NULL;
	PresentParam presentParam;
	SpriteHandler spriteHandler = NULL;
	ColorFormat colorFormat = ColorFormat::RGB32Bit;
	UINT videoAdapter = D3DADAPTER_DEFAULT;
	std::vector<DisplayMode> adapterMode;

	bool isDeviceLost = false;

	STimer timer;
	double delayPerFrame;
	double timeSinceLastFrame;

	std::list<PCamera> cameraList;
	std::list<PSpriteRenderer> renderBuffer;

	// Temp
	int index = 2;
	int delta = -1;
	int rgb[3] = { 255, 0, 1 };
	bool jump = true;
	// EndTemp

	static PGraphics __instance;
};

#define GRAPHICS_EXCEPT(discription) Graphics::GraphicException(__LINE__,__FILE__,discription)
#define GRAPHICS_EXCEPT_CODE(code) Graphics::GraphicCodeException(__LINE__,__FILE__,code)