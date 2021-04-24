#pragma once
#include "CornDirectX.h"
#include "CornException.h"
#include "GameWnd.h"
#include "Base2DRenderScript.h"
#include "Camera.h"
#include <vector>

/// <summary>
/// Singleton directx9 wrapper
/// </summary>

typedef class Base2DRenderScript* PBase2DRenderScript;
typedef const Base2DRenderScript* CPBase2DRenderScript;
typedef class Camera* PCamera;
typedef class SpriteRenderer* PSpriteRenderer;
typedef class Graphics* PGraphics;

class Graphics
{
	friend class App;
	friend class SceneManager;
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
		GraphicException(int line, const char* file, std::wstring description) noexcept;
		virtual const wchar_t* GetType() const noexcept override;
	};
	class GraphicCodeException : public CornException
	{
	public:
		GraphicCodeException(const int line, const char* file, HRESULT code) noexcept;
		virtual const wchar_t* GetType() const noexcept override;
		virtual const wchar_t* What() const noexcept override;
		virtual const wchar_t* Translate() const noexcept;
		virtual HRESULT GetErrorCode() noexcept;
	private:
		HRESULT code;
	};
public:
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	
	static PGraphics GetInstance();
	static void ToFullScreenMode();
	static void ToWindowMode();
	static void Draw(SpriteRenderer* renderScript); // Render Sprite
	static void LoadTexture(PDx9Texture& rTexture, const std::string& path, const Color& keyColor);
	static void AddCamera(PCamera camera);
	static void RemoveCamera(PCamera camera);

	static void SetActiveCamera(PCamera setCamera);
	[[nodiscard]] static PCamera GetActiveCamera();
protected:
	void ClearRenderBuffer();

	void CreateResource();
	void ReleaseResource();

	void FullScreen();
	void Window();
	[[nodiscard]] SGameWnd GetCurrentWindow() const noexcept;
	
	void Init(const STimer& timer, const ColorFormat& colorFormat);

	HRESULT Begin() const noexcept;
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

	Renderer renderer = nullptr;
	RenderDevice renderDevice = nullptr;
	PresentParam presentParam;
	SpriteHandler spriteHandler = nullptr;
	ColorFormat colorFormat = ColorFormat::RGB32Bit;
	UINT videoAdapter = D3DADAPTER_DEFAULT;
	std::vector<DisplayMode> adapterMode;

	bool isDeviceLost = false;

	STimer timer;
	double delayPerFrame;
	double timeSinceLastFrame;

	bool isPixelPerfect = false;
	std::list<PCamera> cameraList;
	std::list<PSpriteRenderer> renderBuffer;

	static PGraphics __instance;
private:
#ifdef _DEBUG
#include "Timer.h"
	float fps = 0;
	PTimer fpsTimer = Timer::Create();
	RECT fpsRect;
	FontHandler fpsFont = nullptr;

	int index = 2;
	int delta = -1;
	int rgb[3] = { 255, 0, 1 };
	bool jump = true;

	void UpdateFPS() {
		fpsTimer->Mark();
		fps = fps * 0.8 + 0.2 * (1 / fpsTimer->GetDeltaTime());
	}
#endif
};

#define GRAPHICS_EXCEPT(discription) Graphics::GraphicException(__LINE__,__FILE__,discription)
#define GRAPHICS_EXCEPT_CODE(code) Graphics::GraphicCodeException(__LINE__,__FILE__,code)