#pragma once
#include <Windows.h>
#include <vector>
#include <map>
#include "Time.h"

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
	bool m_bIsKeyDown = false;
	float m_fRecordedKeyDownTime;
	float m_fRecordedKeyUpTime;
	Keys key;
public:
	KeyPress( Keys key)
	{
		m_fRecordedKeyDownTime = Time::GetSystemTime();
		m_fRecordedKeyUpTime = Time::GetSystemTime();
		this->key = key;
	}
	void SetKeyState( bool bIsKeyDown );
	bool GetKeyHold();
	bool GetKeyDown();
	bool GetKeyUp();
	friend class KeyBoard;
};

static class KeyBoard
{
protected:
	static KeyPress* m_kpKeys[ NUMBER_OF_KEYS ];
	static std::map<int, Keys> m_mVKeyToKey;
public:
	KeyBoard();
	~KeyBoard();

	static bool GetKeyPressDown( Keys kCode );
	static bool GetKeyPressUp( Keys kCode );
	static bool GetKeyDown( Keys kCode );
	static void UpdateKeyState( WPARAM wParam, LPARAM lParam, bool bKeyDown );
};