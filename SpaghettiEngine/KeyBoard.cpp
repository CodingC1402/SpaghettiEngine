#include "KeyBoard.h"
#include "WStrUtils.h"

bool KeyBoard::m_bKeysDown[ NUMBER_OF_KEYS ];
std::map<int, Keys> KeyBoard::m_mVKeyToKey;
std::vector<KeyPress> KeyBoard::m_vKeysPressDown;
std::vector<KeyPress> KeyBoard::m_vKeysPressUp;

KeyBoard::KeyBoard()
{
#pragma region KeyMap
	m_mVKeyToKey[ (int)'~' ] = Keys::TILDE;
	m_mVKeyToKey[ (int)'0' ] = Keys::NUM0;
	m_mVKeyToKey[ (int)'1' ] = Keys::NUM1;
	m_mVKeyToKey[ (int)'2' ] = Keys::NUM2;
	m_mVKeyToKey[ (int)'3' ] = Keys::NUM3;
	m_mVKeyToKey[ (int)'4' ] = Keys::NUM4;
	m_mVKeyToKey[ (int)'5' ] = Keys::NUM5;
	m_mVKeyToKey[ (int)'6' ] = Keys::NUM6;
	m_mVKeyToKey[ (int)'7' ] = Keys::NUM7;
	m_mVKeyToKey[ (int)'8' ] = Keys::NUM8;
	m_mVKeyToKey[ (int)'9' ] = Keys::NUM9;
	m_mVKeyToKey[ (int)'9' ] = Keys::NUM9;

	m_mVKeyToKey[ (int)'Q' ] = Keys::Q;
	m_mVKeyToKey[ (int)'W' ] = Keys::W;
	m_mVKeyToKey[ (int)'E' ] = Keys::E;
	m_mVKeyToKey[ (int)'R' ] = Keys::R;
	m_mVKeyToKey[ (int)'T' ] = Keys::T;
	m_mVKeyToKey[ (int)'Y' ] = Keys::Y;
	m_mVKeyToKey[ (int)'U' ] = Keys::U;
	m_mVKeyToKey[ (int)'I' ] = Keys::I;
	m_mVKeyToKey[ (int)'O' ] = Keys::O;
	m_mVKeyToKey[ (int)'P' ] = Keys::P;
	m_mVKeyToKey[ (int)'A' ] = Keys::A;
	m_mVKeyToKey[ (int)'S' ] = Keys::S;
	m_mVKeyToKey[ (int)'D' ] = Keys::D;
	m_mVKeyToKey[ (int)'F' ] = Keys::F;
	m_mVKeyToKey[ (int)'G' ] = Keys::G;
	m_mVKeyToKey[ (int)'H' ] = Keys::H;
	m_mVKeyToKey[ (int)'J' ] = Keys::J;
	m_mVKeyToKey[ (int)'K' ] = Keys::K;
	m_mVKeyToKey[ (int)'L' ] = Keys::L;
	m_mVKeyToKey[ (int)'Z' ] = Keys::Z;
	m_mVKeyToKey[ (int)'X' ] = Keys::X;
	m_mVKeyToKey[ (int)'C' ] = Keys::C;
	m_mVKeyToKey[ (int)'V' ] = Keys::V;
	m_mVKeyToKey[ (int)'B' ] = Keys::B;
	m_mVKeyToKey[ (int)'N' ] = Keys::N;
	m_mVKeyToKey[ (int)'M' ] = Keys::M;

	m_mVKeyToKey[ (int)'+' ] = Keys::PLUS;
	m_mVKeyToKey[ (int)'_' ] = Keys::MINUS;
	m_mVKeyToKey[ (int)'{' ] = Keys::OPENBRACE;
	m_mVKeyToKey[ (int)'}' ] = Keys::CLOSEBRACE;
	m_mVKeyToKey[ (int)'|' ] = Keys::PIPE;
	m_mVKeyToKey[ (int)':' ] = Keys::COLON;
	m_mVKeyToKey[ (int)'"' ] = Keys::QUOTE;
	m_mVKeyToKey[ (int)'<' ] = Keys::LESSTHAN;
	m_mVKeyToKey[ (int)'>' ] = Keys::GREATERTHAN;
	m_mVKeyToKey[ (int)'?' ] = Keys::QUESTIONMARK;

	m_mVKeyToKey[ VK_ESCAPE ] = Keys::ESC;
	m_mVKeyToKey[ VK_BACK ] = Keys::BACK;
	m_mVKeyToKey[ VK_TAB ] = Keys::TAB;
	m_mVKeyToKey[ VK_CAPITAL ] = Keys::CAPS;
	m_mVKeyToKey[ VK_ACCEPT ] = Keys::ENTER;
	m_mVKeyToKey[ VK_LSHIFT ] = Keys::LSHIFT;
	m_mVKeyToKey[ VK_RSHIFT ] = Keys::RSHIFT;
	m_mVKeyToKey[ VK_LCONTROL ] = Keys::LCTRL;
	m_mVKeyToKey[ VK_RCONTROL ] = Keys::RCTRL;
	m_mVKeyToKey[ VK_LMENU ] = Keys::LALT;
	m_mVKeyToKey[ VK_RMENU ] = Keys::RALT;

	m_mVKeyToKey[ VK_INSERT ] = Keys::INSERT;
	m_mVKeyToKey[ VK_HOME ] = Keys::HOME;
	m_mVKeyToKey[ VK_PRIOR ] = Keys::PAGEUP;
	m_mVKeyToKey[ VK_NEXT ] = Keys::PAGEDOWN;
	m_mVKeyToKey[ VK_END ] = Keys::END;
	m_mVKeyToKey[ VK_DELETE ] = Keys::DEL;

	m_mVKeyToKey[ VK_UP ] = Keys::UP;
	m_mVKeyToKey[ VK_DOWN ] = Keys::DOWN;
	m_mVKeyToKey[ VK_LEFT ] = Keys::LEFT;
	m_mVKeyToKey[ VK_RIGHT ] = Keys::RIGHT;
#pragma endregion
}

KeyBoard::~KeyBoard()
{

}

bool KeyBoard::GetKeyPressDown( Keys kCode )
{
	return false;
}

bool KeyBoard::GetKeyPressUp( Keys kCode )
{
	for ( int i = 0; i < m_vKeysPressUp.size(); i++ )
	{
		if ( m_vKeysPressUp[ i ].key == kCode )
		{
			return kCode;
		}
	}
}

bool KeyBoard::GetKeyDown( Keys kCode )
{
	return m_bKeysDown[ static_cast<int>(kCode) ];
}

void KeyBoard::UpdateKeyState( WPARAM wParam, LPARAM lParam, bool bKeyDown )
{
	if ( bKeyDown )
	{
		if ( !m_bKeysDown[ static_cast<int>(m_mVKeyToKey[ wParam ]) ] )
		{
			//Add key pressed
		}
	}
	m_bKeysDown[ static_cast<int>(m_mVKeyToKey[ wParam ]) ] = bKeyDown;
}