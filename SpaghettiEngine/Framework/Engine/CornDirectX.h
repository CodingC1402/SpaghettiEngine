#pragma once
#include <d3dx9.h>
#include <d3d9.h>

#define ALPHABLEND  D3DXSPRITE_ALPHABLEND 
#define NOTAVAILABLE D3DERR_NOTAVAILABLE
#define OUTOFVIDEOMEMORY D3DERR_OUTOFVIDEOMEMORY
#define INVALIDCALL D3DERR_INVALIDCALL
#define INVALIDDATA D3DXERR_INVALIDDATA
#define OUTOFMEMORY E_OUTOFMEMORY

#define XRGB D3DCOLOR_XRGB
#define RGB XRGB
#define ARGB(r, g, b, alpha) static_cast<Color>((alpha<<24)|(r<<16)|(g<<8)|b)

// decrease drag time but increase memory use so use wisely :v,
// jk like I know shit :v
#define BLACK	XRGB(0, 0, 0)
#define WHITE	XRGB(255, 255, 255)
#define MAGENTA XRGB(255, 0, 255)
#define BLUE	XRGB(0, 0, 255)
#define GREEN	XRGB(0, 255, 0)
#define RED		XRGB(255, 0, 0)

typedef class Graphics* PGraphics;
typedef LPDIRECT3D9 Renderer;
typedef LPDIRECT3DDEVICE9 RenderDevice;
typedef D3DPRESENT_PARAMETERS PresentParam;
typedef D3DDISPLAYMODE DisplayMode;
typedef LPD3DXSPRITE SpriteHandler;
typedef D3DCOLOR Color;
typedef D3DXIMAGE_INFO ImageInfo;
typedef LPD3DXFONT FontHandler;

typedef D3DXVECTOR3 Vector3;
typedef D3DXVECTOR4 Vector4;
typedef D3DXMATRIX Matrix;
typedef LPD3DXMATRIX PMatrix;
