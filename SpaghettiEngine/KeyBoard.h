#pragma once
#include <Windows.h>
#include <vector>
#include <map>

#define NUMBER_OF_KEYS 68

static enum Keys
{
	ESC,
	TILDE, NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9, NUM0, MINUS, PLUS, BACK,
	TAB, Q, W, E, R, T, Y, U, I, O, P, OPENBRACE, CLOSEBRACE, LEFTDASH, PIPE,
	CAPS, A, S, D, F, G, H, J, K, L, COLON, QUOTE, ENTER,
	LSHIFT, Z, X, C, V, B, N, M, LESSTHAN, GREATERTHAN, QUESTIONMARK, RSHIFT,
	LCTRL, LALT, RALT, RCTRL,
	UP, DOWN, LEFT, RIGHT,
	INSERT, HOME, PAGEUP, PAGEDOWN, END, DEL
};

class KeyPress
{
protected:
	bool bPressed = false;
	bool bHandled = false;
public:
	void Handled()
	{
		bHandled = true;
	}
	bool GetKeyState()
	{
		return bPressed;
	}
	friend class KeyBoard;
};

static class KeyBoard
{
protected:
	static bool m_bKeysDown[ NUMBER_OF_KEYS ];
	static KeyPress** m_kpKeysPress;
	static std::map<int, Keys> m_mVKeyToKey;
public:
	KeyBoard();
	~KeyBoard();

	static KeyPress* GetKeyPress( Keys kCode );
	static bool GetKeyDown( Keys kCode );
	static void UpdateKeyState( WPARAM wParam, LPARAM lParam, bool bKeyDown );
};