#include "CornDirectX.h"
#include "Canvas.h"
#include "DirectX9Graphic.h"
#include "DebugRenderer.h"
#include "Graphics.h"
#include "GameWnd.h"
#include "json.hpp"
#include "Monitor.h"
#include "Render2DScriptBase.h"
#include "Setting.h"
#include "Setting.h"
#include "Sprite.h"

#include <fstream>
#include <DirectXMath.h>

SDirectX9Graphic Graphics::_turdGraphic = std::make_shared<DirectX9Graphic>();

Graphics::GraphicException::GraphicException(const int line, const char* file, std::wstring description) noexcept
	: 
	CornDescriptionException(line, file, std::move(description))
{}

const wchar_t* Graphics::GraphicException::GetType() const noexcept
{
	return L"∑(O_O;) Graphic Exception";
}

void Graphics::AddRender2D(PRender2DScriptBase renderScript)
{
	_renderBuffer2D[renderScript->GetDrawLayer()].emplace_back(renderScript);
}

void Graphics::RemoveRender2D(PRender2DScriptBase renderScript)
{
	_renderBuffer2D[renderScript->GetDrawLayer()].remove(renderScript);
}

void Graphics::AddUIRender(PCanvas canvas)
{
	_canvasList.emplace_back(canvas);
}

void Graphics::RemoveUIRender(PCanvas canvas)
{
	_canvasList.remove(canvas);
}

void Graphics::SetSpriteTransform(const Matrix4& matrix)
{
	auto dxMatrix = matrix.ConvertToDxMatrix();
	if (Setting::IsWorldPointPixelPerfect())
	{
		dxMatrix._41 = std::round(dxMatrix._41);
		dxMatrix._42 = std::round(dxMatrix._42);
	}
	_turdGraphic->SetRenderTransform(dxMatrix);
}

void Graphics::ResetSpriteTransform()
{
	_turdGraphic->SetRenderTransform(Matrix4::GetDiagonalMatrix().ConvertToDxMatrix());
}

void Graphics::DrawSprite(const SSprite& sprite, const Vector3& center, const Vector3& position, const Color& color)
{
	RECT srcRect = sprite->GetSourceRect();
	auto dxCenter = center.ConvertToDxVector();
	auto dxPos = position.ConvertToDxVector();

	_turdGraphic->RenderSprite(
		sprite->GetSource()->GetImage(),
		srcRect,
		dxCenter,
		dxPos,
		color
	);
}

void Graphics::DrawUI(const SSprite& sprite, const Vector3& position, const Color& color)
{
	RECT srcRect = sprite->GetSourceRect();
	auto dxCenter = Vector3(0, 0, 0).ConvertToDxVector();
	auto dxPos = position.ConvertToDxVector();

	_turdGraphic->RenderSprite(
		sprite->GetSource()->GetImage(),
		srcRect,
		dxCenter,
		dxPos,
		color
	);
}

void Graphics::AddCamera(PCamera camera)
{
	_cameraList.push_back(camera);
}

void Graphics::RemoveCamera(PCamera camera)
{
	_cameraList.remove(camera);
}

void Graphics::SetActiveCamera(PCamera setCamera)
{
	_cameraList.remove(setCamera);
	_cameraList.emplace_front(setCamera);
}

void Graphics::ClearRenderBuffer2D()
{
	for (auto& layer : _renderBuffer2D)
		layer.clear();
}

void Graphics::ClearRenderBuffer()
{
	ClearRenderBuffer2D();
}

SGameWnd Graphics::GetCurrentWindow() noexcept
{
	return _wnd;
}

SDirectX9Graphic Graphics::GetDirectXGfx()
{
	return _turdGraphic;
}

void Graphics::FullScreen()
{
	if (_isFullScreen)
		return;

	_isFullScreen = true;
	_wnd->ChangeWindowMode(true);
}

void Graphics::Window()
{
	if (!_isFullScreen)
		return;

	_isFullScreen = false;
	_wnd->ChangeWindowMode(false);
}

PCamera Graphics::GetActiveCamera()
{
	return *_cameraList.begin();
}

void Graphics::Init(const ColorFormat& colorFormat, SGameWnd window)
{
	_timer.reset(Timer::Create());
	_wnd = window;

	auto resolution = Setting::GetResolution();
	_width  = resolution.width;
	_height = resolution.height;
	_turdGraphic->Init(
		static_cast<D3DFORMAT>(colorFormat),
		_width,
		_height,
		_wnd->GetContentWndHandler()
	);
}


void Graphics::Render()
{ 
	if (_cameraList.empty())
		return;

	_timer->Mark();
	_timeSinceLastFrame += _timer->GetDeltaTime();
	if (_timeSinceLastFrame < _delayPerFrame)
	{
		return;
	}

	if (_turdGraphic->StartRender())
	{
		_timeSinceLastFrame -= _delayPerFrame * static_cast<int>(_timeSinceLastFrame / _delayPerFrame);
		_turdGraphic->Clear(Color(0, 0, 0, 0));


		_turdGraphic->StartRenderSprite();
		const auto cameraScript = *_cameraList.begin();
		for (auto& layer : _renderBuffer2D)
			for (auto renderScript2D = layer.begin(); renderScript2D != layer.end(); ++renderScript2D)
				(*renderScript2D)->Draw(cameraScript);

		// Only available in debugMode cause why would you
		// use this shit out side debug.
		ResetSpriteTransform();
		if constexpr (Setting::IsDebugMode())
		{
			_turdGraphic->EndRenderSprite();
			// The debug renderer will begin the line render session itself.
			DebugRenderer::Render(_turdGraphic, cameraScript);
			_turdGraphic->StartRenderSprite();
		}

		for (auto& canvas : _canvasList)
			canvas->Draw();
		_turdGraphic->EndRenderSprite();

		if (!_turdGraphic->EndRender())
			_turdGraphic->ResetRender();
		_turdGraphic->Present();
	}
}
#pragma region  Exception
Graphics::GraphicCodeException::GraphicCodeException(int line, const char* file, HRESULT code) noexcept
	:
	CornException(line, file),
	code(code)
{}
const wchar_t* Graphics::GraphicCodeException::GetType() const noexcept
{
	return L"∑(O_O;) Graphic Exception";
}
const wchar_t* Graphics::GraphicCodeException::What() const noexcept
{
	std::wostringstream os;
	os << GetType() << std::endl;
	os << "[Description] " << Translate() << std::endl;
	os << GetOriginString();
	whatBuffer = os.str();
	return whatBuffer.c_str();
}
const wchar_t* Graphics::GraphicCodeException::Translate() const noexcept
{
	switch (code)
	{
	case NOTAVAILABLE:
		return L"File is not available";
	case OUTOFVIDEOMEMORY:
		return L"Out of video memory(gpu)";
	case INVALIDCALL:
		return L"Invalid call";
	case INVALIDDATA:
		return L"Invalid data";
	case OUTOFMEMORY:
		return L"Out of memory(ram)";
	default:
		return L": ^)";
	}
}
HRESULT Graphics::GraphicCodeException::GetErrorCode() noexcept
{
	return code;
}
#pragma endregion