#pragma once

#include "Color.h"
#include "CornException.h"
#include "Texture.h"
#include "Matrix.h"
#include "Vector3.h"
#include "Timer.h"

#include <vector>

#define D3DADAPTER_DEFAULT 0

/// <summary>
/// Singleton directx9 wrapper
/// </summary>
class GameWnd;
typedef std::shared_ptr<GameWnd> SGameWnd;

class DirectX9Graphic;
typedef class std::shared_ptr<DirectX9Graphic> SDirectX9Graphic;

typedef class LineRendererBase* PLineRendererBase;
typedef class Render2DScriptBase* PRender2DScriptBase;
typedef class Camera* PCamera;
typedef class Graphics* PGraphics;
typedef class Canvas* PCanvas;


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

	class GraphicException : public CornDescriptionException
	{
	public:
		GraphicException(int line, const char* file, std::wstring description) noexcept;
		virtual const wchar_t* GetType() const noexcept override;
	};
	class GraphicCodeException : public CornException
	{
	public:
		GraphicCodeException(const int line, const char* file, long code) noexcept;
		virtual const wchar_t* GetType() const noexcept override;
		virtual const wchar_t* What() const noexcept override;
		virtual const wchar_t* Translate() const noexcept;
		virtual long GetErrorCode() noexcept;
	private:
		long code;
	};
public:
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;

	static SDirectX9Graphic GetDirectXGfx();

	static void FullScreen();
	static void Window();

	static void AddRender2D(PRender2DScriptBase renderScript);
	static void RemoveRender2D(PRender2DScriptBase renderScript);

	static void AddUIRender(PCanvas canvas);
	static void RemoveUIRender(PCanvas canvas);

	static void SetSpriteTransform(const Matrix4& matrix);
	static void ResetSpriteTransform();
	static void DrawSprite(const SSprite& sprite, const Vector3& center = { 0, 0, 0 }, const Vector3& position = { 0, 0, 0 }, const Color& color = WHITE);
	static void DrawUI(const SSprite& sprite, const Vector3& position = Vector3(), const Color& color = WHITE);
	
	static void AddCamera(PCamera camera);
	static void RemoveCamera(PCamera camera);

	static void SetActiveCamera(PCamera setCamera);
	[[nodiscard]] static PCamera GetActiveCamera();
protected:
	static void ClearRenderBuffer2D();
	static void ClearRenderBuffer(); // for later if we want to add a 3D renderer

	[[nodiscard]] static SGameWnd GetCurrentWindow() noexcept;
	
	static void Init(const ColorFormat& colorFormat, SGameWnd window);
	static void Render();

protected:
	static inline SGameWnd _wnd;
	static inline bool _isFullScreen;
	static inline unsigned _width;
	static inline unsigned _height;

	static inline STimer _timer;
	static inline float _delayPerFrame;
	static inline float _timeSinceLastFrame;

	static inline std::list<PCanvas> _canvasList;
	static inline std::list<PCamera> _cameraList;
	static inline std::vector<std::list<PRender2DScriptBase>> _renderBuffer2D = std::move(std::vector<std::list<PRender2DScriptBase>>(32));

	static SDirectX9Graphic _turdGraphic;
};

#define GRAPHICS_EXCEPT(description) Graphics::GraphicException(__LINE__,__FILE__,description)
#define GRAPHICS_EXCEPT_CODE(code) Graphics::GraphicCodeException(__LINE__,__FILE__,code)