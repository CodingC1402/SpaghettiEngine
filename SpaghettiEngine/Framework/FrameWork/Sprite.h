#pragma once
#include <d3d9.h>
#include <d3dx9.h>

#define SPRITE_PATH L"Mr.G.png"

class Sprite
{
protected:
	LPDIRECT3DTEXTURE9 _Image;		// The “container”
	LPD3DXSPRITE _SpriteHandler;
	D3DXIMAGE_INFO info;

	int _Index; 		// Current sprite index
	int _Width;   	// Sprite width
	int _Height;		// Sprite height
	int _Count;		// Number of sprites in the container
	int _SpritePerRow;	// Number of sprites per row
public:
	Sprite(LPD3DXSPRITE SpriteHandler, char* Path, int Width, int Height, int Count, int SpritePerRow);

	// Advance to next sprite in the list
	void Next();

	// Render current sprite at location (X,Y) at the target surface
	void Render(LPDIRECT3DSURFACE9 Target, int X, int Y);
};

