#include "DirectX9Graphic.h"
#include "Setting.h"
#include "Monitor.h"

void DirectX9Graphic::Init(D3DFORMAT colorFormat, UINT backBufferWidth, UINT backBufferHeight, HWND windowHandler)
{
	_renderer = Direct3DCreate9(D3D_SDK_VERSION);
	_colorFormat = colorFormat;
	_wndHandler = windowHandler;

	ZeroMemory(&_presentParam, sizeof(_presentParam));

	_presentParam.Windowed = TRUE;
	_presentParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
	_presentParam.BackBufferFormat = colorFormat;
	_presentParam.BackBufferCount = 1;
	_presentParam.BackBufferWidth = backBufferWidth;
	_presentParam.BackBufferHeight = backBufferHeight;
	_presentParam.hDeviceWindow = windowHandler;

	CreateResource();
}

void DirectX9Graphic::CreateResource()
{
	_renderer->CreateDevice(
		_videoAdapter,
		D3DDEVTYPE_HAL,
		_wndHandler,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&_presentParam,
		&_renderDevice
	);

	HRESULT result = D3DXCreateSprite(_renderDevice, &_spriteHandler);
	//if (FAILED(result))
	//	throw GRAPHICS_EXCEPT_CODE(result);

	result = D3DXCreateLine(_renderDevice, &_lineHandler);
	//if (FAILED(result))
	//	throw GRAPHICS_EXCEPT_CODE(result);
	if (!Setting::IsResolutionPixelPerfect())
	{
		_lineHandler->SetAntialias(TRUE);
	}
}

void DirectX9Graphic::ReleaseResource()
{
	if (_renderDevice)
		_renderDevice->Release();
	if (_spriteHandler)
		_spriteHandler->Release();
	if (_lineHandler)
		_lineHandler->Release();
	_renderDevice = nullptr;
	_lineHandler = nullptr;
	_spriteHandler = nullptr;
}

void DirectX9Graphic::SetRenderTransform(const D3DXMATRIX& matrix) noexcept
{
	_spriteHandler->SetTransform(&matrix);
}

void DirectX9Graphic::RenderSprite(LPDIRECT3DTEXTURE9 texture, const RECT& sprite, const D3DXVECTOR3& center, const D3DXVECTOR3& position, const D3DCOLOR& color)
{
	_spriteHandler->Draw(
		texture,
		&sprite,
		&center,
		&position,
		color
	);
}

void DirectX9Graphic::RenderPolygon(const D3DXVECTOR2 vertexes[], const unsigned& size, const float& width, const D3DCOLOR& color)
{
	_lineHandler->SetWidth(width);

	_lineHandler->Draw(
		vertexes,
		static_cast<DWORD>(size),
		color
	);
}

bool DirectX9Graphic::StartRender()
{
	auto result = _renderDevice->BeginScene();
	if (result == D3D_OK)
		return true;
	else
	{
		// Could be not ready yet but also could be an error so add exception here.
		return false;
	}
}

bool DirectX9Graphic::EndRender()
{
	if (!_isDeviceLost)
	{
		_renderDevice->EndScene();
	}
	if (const HRESULT hr = _renderDevice->TestCooperativeLevel(); hr != D3D_OK)
	{
		if (hr == D3DERR_DEVICELOST)
		{
			_isDeviceLost = true;
			_spriteHandler->OnLostDevice();
			_lineHandler->OnLostDevice();
		}
		else if (hr == D3DERR_DRIVERINTERNALERROR)
		{
			PostQuitMessage(0);
			// PLEASE ADD BACK EXCEPTION EHERE
		}
		return false;
	}
	return true;
}

bool DirectX9Graphic::ResetRender()
{
	if (_renderDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		if (SUCCEEDED(_renderDevice->Reset(&_presentParam)))
		{
			// reset success
			_spriteHandler->OnResetDevice();
			_lineHandler->OnResetDevice();
			_isDeviceLost = false;
			return true;
		}
	}
	// failed to reset
	return false;
}

void DirectX9Graphic::Present()
{
	_renderDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

void DirectX9Graphic::StartRenderLine()
{
	_lineHandler->End();
}

void DirectX9Graphic::EndRenderLine()
{
	_lineHandler->Begin();
}

void DirectX9Graphic::StartRenderSprite()
{
	_spriteHandler->Begin(ALPHABLEND);
}

void DirectX9Graphic::EndRenderSprite()
{
	_spriteHandler->End();
}

DirectX9Graphic::~DirectX9Graphic()
{
	if (_renderer)
		_renderer->Release();
	_renderer = nullptr;
	ReleaseResource();
}

void DirectX9Graphic::LoadTexture(LPDIRECT3DTEXTURE9& rTexture, const std::string& path, const D3DCOLOR& keyColor)
{
	const std::wstring wPath = StringConverter::StrToWStr(path);
	ImageInfo info;

	HRESULT result = D3DXGetImageInfoFromFile(wPath.c_str(), &info);

	// Add exception here

	result = D3DXCreateTextureFromFileEx(
		_renderDevice,
		wPath.c_str(),
		info.Width,
		info.Height,
		1,
		D3DPOOL_DEFAULT,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		keyColor,
		&info,
		nullptr,
		&rTexture
	);
}

void DirectX9Graphic::Clear(const DWORD& backGround)
{
	_renderDevice->Clear(0, nullptr, D3DCLEAR_TARGET, backGround, 1.0f, 0);
}

void DirectX9Graphic::UpdateCurrentVideoAdapter()
{
	const auto monitor = Monitor::GetCurrentMonitor(_wndHandler);
	const auto format = _colorFormat;
	_adapterMode.clear();
	const UINT adapterCount = _renderer->GetAdapterCount();
	for (UINT i = 0; i < adapterCount; i++)
	{
		if (_renderer->GetAdapterMonitor(i) == monitor)
		{
			_videoAdapter = i;
			break;
		}
	}

	const UINT modeCount = _renderer->GetAdapterModeCount(_videoAdapter, format);
	for (UINT i = 0; i < modeCount; i++)
	{
		DisplayMode mode;
		if (_renderer->EnumAdapterModes(_videoAdapter, format, i, &mode) == D3D_OK)
		{
			_adapterMode.push_back(mode);
		}
	}
}