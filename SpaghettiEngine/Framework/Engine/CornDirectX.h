#pragma once
#include <d3dx9.h>
#include <d3d9.h>

#define ALPHABLEND D3DXSPRITE_ALPHABLEND 
#define NOTAVAILABLE D3DERR_NOTAVAILABLE
#define OUTOFVIDEOMEMORY D3DERR_OUTOFVIDEOMEMORY
#define INVALIDCALL D3DERR_INVALIDCALL
#define INVALIDDATA D3DXERR_INVALIDDATA
#define OUTOFMEMORY E_OUTOFMEMORY

#define XRGB D3DCOLOR_XRGB
#define ARGB(r, g, b, alpha) static_cast<Color>(0|(alpha<<24)|(r<<16)|(g<<8)|b)

typedef class Graphics* PGraphics;
typedef LPDIRECT3D9 Renderer;
typedef LPDIRECT3DDEVICE9 RenderDevice;
typedef D3DPRESENT_PARAMETERS PresentParam;
typedef D3DDISPLAYMODE DisplayMode;
typedef LPD3DXSPRITE SpriteHandler;
typedef D3DCOLOR Color;
typedef D3DXIMAGE_INFO ImageInfo;
typedef LPD3DXFONT FontHandler;

typedef D3DXVECTOR4 Vector4;
typedef D3DXVECTOR3 Vector3;
typedef D3DXMATRIX Matrix;
typedef LPD3DXMATRIX PMatrix;