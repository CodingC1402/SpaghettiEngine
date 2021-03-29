#pragma once
#include <d3d9.h>

#pragma comment (lib, "d3d9.lib")

struct CUSTOMVERTEX { FLOAT X, Y, Z, RHW; DWORD COLOR; };
#define CUSTOMFVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

class Graphics
{
public:
    Graphics(int iWidth, int iHeight, HWND hWnd) noexcept;
    ~Graphics();
    void RenderFrame();
    void InitGraphics();
protected:
    LPDIRECT3D9 d3d;    // the pointer to our Direct3D interface
    LPDIRECT3DDEVICE9 d3ddev;    // the pointer to the device class
    LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL;
};