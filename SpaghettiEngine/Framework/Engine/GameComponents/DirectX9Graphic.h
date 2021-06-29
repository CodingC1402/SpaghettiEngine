#pragma once

#include <d3dx9.h>
#include <d3d9.h>
#include <vector>
#include "CornDirectX.h"


class DirectX9Graphic
{
public:
	void SetRenderTransform(const D3DXMATRIX& matrix);
	void RenderSprite(LPDIRECT3DTEXTURE9 texture, const RECT& sprite, const D3DXVECTOR3& center, const D3DXVECTOR3& position, const D3DCOLOR& color);

	void SetPolygonTransform(const D3DXMATRIX& matrix);
	void RenderPolygon(const std::vector<D3DXVECTOR2>& vertexes);
protected:
	LPD3DXSPRITE _spriteHandler;

	D3DXMATRIX _lineRenderMatrix;
	LPD3DXLINE _lineHandler;

	LPD3DXFONT
};