#pragma once

#ifdef FULLGRAPHIC
#include <d3d9.h>
typedef class Graphics* PGraphics;
typedef LPDIRECT3D9 Renderer;
typedef LPDIRECT3DDEVICE9 RenderDevice;
typedef D3DPRESENT_PARAMETERS PresentParam;
typedef D3DDISPLAYMODE DisplayMode;
#endif

#include <d3dx9.h>
typedef D3DXVECTOR3 Vector3;
typedef D3DXMATRIX Matrix;