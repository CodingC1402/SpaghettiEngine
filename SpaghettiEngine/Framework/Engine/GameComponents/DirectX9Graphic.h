#pragma once

#include <d3dx9.h>
#include <d3d9.h>
#include <vector>
#include <string>
#include "CornDirectX.h"


class DirectX9Graphic
{
	friend class Graphics;
	friend class Texture;
	friend class DebugRenderer;
public:
	~DirectX9Graphic();
protected:
	void Init(D3DFORMAT colorFormat, UINT backBufferWidth, UINT backBufferHeight, HWND windowHandler);

	void CreateResource();
	void ReleaseResource();
	void LoadTexture(LPDIRECT3DTEXTURE9& outTexture, const std::string& path, const D3DCOLOR& keyColor);
	void Clear(const DWORD& backGround);

	void SetRenderTransform(const D3DXMATRIX& matrix) noexcept;
	void RenderSprite(LPDIRECT3DTEXTURE9 texture, const RECT& sprite, const D3DXVECTOR3& center, const D3DXVECTOR3& position, const D3DCOLOR& color);

	void RenderPolygon(const D3DXVECTOR2 vertexes[], const unsigned& size, const float& width, const D3DCOLOR& color);

	bool StartRender();
	bool EndRender();
	bool ResetRender();

	void Present();

	void StartRenderLine();
	void EndRenderLine();

	void StartRenderSprite();
	void EndRenderSprite();

	void UpdateCurrentVideoAdapter();
protected:
	LPD3DXSPRITE _spriteHandler = nullptr;

	D3DXMATRIX _lineRenderMatrix;
	LPD3DXLINE _lineHandler = nullptr;

	bool _isDeviceLost = false; // Currently unused cause I'm not setting resolution
	LPDIRECT3D9 _renderer = nullptr;
	LPDIRECT3DDEVICE9 _renderDevice = nullptr;
	D3DPRESENT_PARAMETERS _presentParam;

	
	D3DFORMAT _colorFormat = D3DFORMAT::D3DFMT_A8R8G8B8;
	HWND _wndHandler = nullptr;

	// Unused
	UINT _videoAdapter = D3DADAPTER_DEFAULT;
	std::vector<D3DDISPLAYMODE> _adapterMode;
};