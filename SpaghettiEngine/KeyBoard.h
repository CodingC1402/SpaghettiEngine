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
	bool bHandled = false;
	Keys key;
public:
	KeyPress( Keys key)
	{
		this->key = key;
	}
	friend class KeyBoard;
};

static class KeyBoard
{
protected:
	static bool m_bKeysDown[ NUMBER_OF_KEYS ];
	static std::vector<KeyPress> m_vKeysPressDown;
	static std::vector<KeyPress> m_vKeysPressUp;
	static std::map<int, Keys> m_mVKeyToKey;
public:
	KeyBoard();
	~KeyBoard();

	static bool GetKeyPressDown( Keys kCode );
	static bool GetKeyPressUp( Keys kCode );
	static bool GetKeyDown( Keys kCode );
	static void UpdateKeyState( WPARAM wParam, LPARAM lParam, bool bKeyDown );
};