#include "Sprite.h"

Sprite::Sprite(LPD3DXSPRITE SpriteHandler, char* Path, int Width, int Height, int Count, int SpritePerRow)
{
	D3DXIMAGE_INFO info;
	HRESULT result;

	_Image = NULL;
	_SpriteHandler = SpriteHandler;

	_Width = Width;
	_Height = Height;
	_Count = Count;
	_SpritePerRow = SpritePerRow;
	_Index = 0;

	result = D3DXGetImageInfoFromFile(SPRITE_PATH, &info);
	result = D3DXGetImageInfoFromFile(SPRITE_PATH, &info);
		LPDIRECT3DDEVICE9 d3ddv; SpriteHandler->GetDevice(&d3ddv);
	result = D3DXCreateTextureFromFileEx(
		d3ddv, SPRITE_PATH,
		info.Width, info.Height,
		1,				//Mipmap levels
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(0, 0, 0),		// Transparent color
		&info,				// Image information
		NULL,
		&_Image);			// Result
}

void Sprite::Next()
{
}

void Sprite::Render(LPDIRECT3DSURFACE9 Target, int X, int Y)
{
	RECT srect;

	srect.left = (_Index % _SpritePerRow) * (_Width);
	srect.top = (_Index / _SpritePerRow) * (_Height);
	srect.right = srect.left + _Width - 1;
	srect.bottom = srect.top + _Height - 1;

	_SpriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
	D3DXVECTOR3 position((float)X, (float)Y, 0);
	_SpriteHandler->Draw(
		_Image,
		&srect,
		NULL,
		&position,
		D3DCOLOR_XRGB(255, 255, 255)	// Hardcode!
	);
	_SpriteHandler->End();

}
