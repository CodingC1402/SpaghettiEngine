#pragma once
#include <d3dx9.h>
#include <d3d9.h>

#define ALPHABLEND  D3DXSPRITE_ALPHABLEND 
#define NOTAVAILABLE D3DERR_NOTAVAILABLE
#define OUTOFVIDEOMEMORY D3DERR_OUTOFVIDEOMEMORY
#define INVALIDCALL D3DERR_INVALIDCALL
#define INVALIDDATA D3DXERR_INVALIDDATA
#define OUTOFMEMORY E_OUTOFMEMORY

typedef class Graphics* PGraphics;
typedef LPDIRECT3D9 Renderer;
typedef LPDIRECT3DDEVICE9 RenderDevice;
typedef D3DPRESENT_PARAMETERS PresentParam;
typedef D3DDISPLAYMODE DisplayMode;
typedef LPD3DXSPRITE SpriteHandler;
typedef D3DXIMAGE_INFO ImageInfo;
typedef LPD3DXFONT FontHandler;
typedef LPDIRECT3DTEXTURE9 PImage;
typedef LPD3DXLINE LineHandler;

typedef D3DXVECTOR2 Vector2;
