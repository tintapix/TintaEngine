/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef TINTA_CONTROLS_H_
#define TINTA_CONTROLS_H_

#include "tintaPredefine.h"
#include "tintaSingleton.h"
#include "tintaException.h"
#include <Math/tintaVector2.h>


namespace Tinta {

struct KeyStateCallback;
struct MouseStateCallback;
	
class tintaControls	
{

public:

	enum EnInnerKeys{
		/// from OIS keys
		ML_UNASSIGNED  = 0x00,
		ML_ESCAPE      = 0x01,
		ML_1           = 0x02,
		ML_2           = 0x03,
		ML_3           = 0x04,
		ML_4           = 0x05,
		ML_5           = 0x06,
		ML_6           = 0x07,
		ML_7           = 0x08,
		ML_8           = 0x09,
		ML_9           = 0x0A,
		ML_0           = 0x0B,
		ML_MINUS       = 0x0C,    // - on main keyboard
		ML_EQUALS      = 0x0D,
		ML_BACK        = 0x0E,    // backspace
		ML_TAB         = 0x0F,
		ML_Q           = 0x10,
		ML_W           = 0x11,
		ML_E           = 0x12,
		ML_R           = 0x13,
		ML_T           = 0x14,
		ML_Y           = 0x15,
		ML_U           = 0x16,
		ML_I           = 0x17,
		ML_O           = 0x18,
		ML_P           = 0x19,
		ML_LBRACKET    = 0x1A,
		ML_RBRACKET    = 0x1B,
		ML_RETURN      = 0x1C,    // Enter on main keyboard
		ML_LCONTROL    = 0x1D,
		ML_A           = 0x1E,
		ML_S           = 0x1F,
		ML_D           = 0x20,
		ML_F           = 0x21,
		ML_G           = 0x22,
		ML_H           = 0x23,
		ML_J           = 0x24,
		ML_K           = 0x25,
		ML_L           = 0x26,
		ML_SEMICOLON   = 0x27,
		ML_APOSTROPHE  = 0x28,
		ML_GRAVE       = 0x29,    // accent
		ML_LSHIFT      = 0x2A,
		ML_BACKSLASH   = 0x2B,
		ML_Z           = 0x2C,
		ML_X           = 0x2D,
		ML_C           = 0x2E,
		ML_V           = 0x2F,
		ML_B           = 0x30,
		ML_N           = 0x31,
		ML_M           = 0x32,
		ML_COMMA       = 0x33,
		ML_PERIOD      = 0x34,    // . on main keyboard
		ML_SLASH       = 0x35,    // / on main keyboard
		ML_RSHIFT      = 0x36,
		ML_MULTIPLY    = 0x37,    // * on numeric keypad
		ML_LMENU       = 0x38,    // left Alt
		ML_SPACE       = 0x39,
		ML_CAPITAL     = 0x3A,
		ML_F1          = 0x3B,
		ML_F2          = 0x3C,
		ML_F3          = 0x3D,
		ML_F4          = 0x3E,
		ML_F5          = 0x3F,
		ML_F6          = 0x40,
		ML_F7          = 0x41,
		ML_F8          = 0x42,
		ML_F9          = 0x43,
		ML_F10         = 0x44,
		ML_NUMLOCK     = 0x45,
		ML_SCROLL      = 0x46,    // Scroll Lock
		ML_NUMPAD7     = 0x47,
		ML_NUMPAD8     = 0x48,
		ML_NUMPAD9     = 0x49,
		ML_SUBTRACT    = 0x4A,    // - on numeric keypad
		ML_NUMPAD4     = 0x4B,
		ML_NUMPAD5     = 0x4C,
		ML_NUMPAD6     = 0x4D,
		ML_ADD         = 0x4E,    // + on numeric keypad
		ML_NUMPAD1     = 0x4F,
		ML_NUMPAD2     = 0x50,
		ML_NUMPAD3     = 0x51,
		ML_NUMPAD0     = 0x52,
		ML_DECIMAL     = 0x53,    // . on numeric keypad
		ML_OEM_102     = 0x56,    // < > | on UK/Germany keyboards
		ML_F11         = 0x57,
		ML_F12         = 0x58,
		ML_F13         = 0x64,    //                     (NEC PC98)
		ML_F14         = 0x65,    //                     (NEC PC98)
		ML_F15         = 0x66,    //                     (NEC PC98)
		ML_KANA        = 0x70,    // (Japanese keyboard)
		ML_ABNT_C1     = 0x73,    // / ? on Portugese (Brazilian) keyboards
		ML_CONVERT     = 0x79,    // (Japanese keyboard)
		ML_NOCONVERT   = 0x7B,    // (Japanese keyboard)
		ML_YEN         = 0x7D,    // (Japanese keyboard)
		ML_ABNT_C2     = 0x7E,    // Numpad . on Portugese (Brazilian) keyboards
		ML_NUMPADEQUALS= 0x8D,    // = on numeric keypad (NEC PC98)
		ML_PREVTRACK   = 0x90,    // Previous Track (ML_CIRCUMFLEX on Japanese keyboard)
		ML_AT          = 0x91,    //                     (NEC PC98)
		ML_COLON       = 0x92,    //                     (NEC PC98)
		ML_UNDERLINE   = 0x93,    //                     (NEC PC98)
		ML_KANJI       = 0x94,    // (Japanese keyboard)
		ML_STOP        = 0x95,    //                     (NEC PC98)
		ML_AX          = 0x96,    //                     (Japan AX)
		ML_UNLABELED   = 0x97,    //                        (J3100)
		ML_NEXTTRACK   = 0x99,    // Next Track
		ML_NUMPADENTER = 0x9C,    // Enter on numeric keypad
		ML_RCONTROL    = 0x9D,
		ML_MUTE        = 0xA0,    // Mute
		ML_CALCULATOR  = 0xA1,    // Calculator
		ML_PLAYPAUSE   = 0xA2,    // Play / Pause
		ML_MEDIASTOP   = 0xA4,    // Media Stop
		ML_VOLUMEDOWN  = 0xAE,    // Volume -
		ML_VOLUMEUP    = 0xB0,    // Volume +
		ML_WEBHOME     = 0xB2,    // Web home
		ML_NUMPADCOMMA = 0xB3,    // , on numeric keypad (NEC PC98)
		ML_DIVIDE      = 0xB5,    // / on numeric keypad
		ML_SYSRQ       = 0xB7,
		ML_RMENU       = 0xB8,    // right Alt
		ML_PAUSE       = 0xC5,    // Pause
		ML_HOME        = 0xC7,    // Home on arrow keypad
		ML_UP          = 0xC8,    // UpArrow on arrow keypad
		ML_PGUP        = 0xC9,    // PgUp on arrow keypad
		ML_LEFT        = 0xCB,    // LeftArrow on arrow keypad
		ML_RIGHT       = 0xCD,    // RightArrow on arrow keypad
		ML_END         = 0xCF,    // End on arrow keypad
		ML_DOWN        = 0xD0,    // DownArrow on arrow keypad
		ML_PGDOWN      = 0xD1,    // PgDn on arrow keypad
		ML_INSERT      = 0xD2,    // Insert on arrow keypad
		ML_DELETE      = 0xD3,    // Delete on arrow keypad
		ML_LWIN        = 0xDB,    // Left Windows key
		ML_RWIN        = 0xDC,    // Right Windows key
		ML_APPS        = 0xDD,    // AppMenu key
		ML_POWER       = 0xDE,    // System Power
		ML_SLEEP       = 0xDF,    // System Sleep
		ML_WAKE        = 0xE3,    // System Wake
		ML_WEBSEARCH   = 0xE5,    // Web Search
		ML_WEBFAVORITES= 0xE6,    // Web Favorites
		ML_WEBREFRESH  = 0xE7,    // Web Refresh
		ML_WEBSTOP     = 0xE8,    // Web Stop
		ML_WEBFORWARD  = 0xE9,    // Web Forward
		ML_WEBBACK     = 0xEA,    // Web Back
		ML_MYCOMPUTER  = 0xEB,    // My Computer
		ML_MAIL        = 0xEC,    // Mail
		ML_MEDIASELECT = 0xED,     // Media Select
		ML_MOUSE_LEFT  = 0xEE,     
		ML_MOUSE_MIDDLE = 0xEF,   
		ML_MOUSE_RIGHT  = 0xF0,    
        ML_MOUSE_WEEL   = 0xF1,

		ML_MAX_KEY	   = 0xF2   

	};


	public:
		tintaControls( unsigned keys_quantity );

        tintaControls( unsigned keys_quantity, const String &comPath );

	virtual ~tintaControls(void);

	/// Fills platform depending parameters
	bool regPair( unsigned os_key, EnInnerKeys inner_key );
	
	bool isKeyPressed(EnInnerKeys key) const;

	//void getMousePos(int &x_val, int &y_val)const ;

    tintaVector2i getMousePos()const ;

	void setKeyState(unsigned os_key, bool is_pressed);

    void setMouseWeelState(int param );

    int getMouseWeelState() const ;

	void setMousePos(int x_pos, int y_pos);

	void regKeyCallback( KeyStateCallback* listener );
	void regMouseCallback( MouseStateCallback* listener );
	
	void delKeyCallback( KeyStateCallback* listener);
	void delMouseCallback( MouseStateCallback* listener);

    enum actionBits {        
        noBits              = 0x0,
        downKey     = 0x1,
        upKey       = 0x2,
    };

    //return command by options
    String getCommand( EnInnerKeys key, actionBits bits = noBits )const;

    void   bindCommand( EnInnerKeys key, const String &command, actionBits bits = noBits );

    /// parsing keys.mfg file
    void parseBinds( const String &bindsPath = _M("") );

    typedef struct keyCommand {
        // if true - handle on down or will forward
        actionBits   mKeyState;
        String       mCommand;
    }keyCommand_t;

	protected:
		typedef std::vector<EnInnerKeys> t_keys_converter;
		typedef std::bitset<ML_MAX_KEY> t_keys_states;
		typedef std::set<KeyStateCallback*> t_key_callbacks;
		typedef std::set<MouseStateCallback*> t_mouse_callbacks;
        typedef std::vector<keyCommand_t> commands_t;


	t_keys_converter  mKeysConvert;
	t_keys_states     mKeyState;
	t_key_callbacks   mKeyCallbacks;
	t_mouse_callbacks mMouseCallbacks;
    commands_t        mCommands;
    String            mBindsPath;

	void notifyKeyListeners( tintaControls::EnInnerKeys key, bool pressed );
	void notifyMouseListeners( int x_pos, int y_pos );
    void notifyMouseWeelListeners(int param);

	private:
		tintaControls(void) = delete;
		int mXPos;
		int mYPos;
        int mWeelParam;

};

struct KeyStateCallback {
    /*
        pressed - if true button was down, false - up
    */
    virtual void OnKeyStateChanged(tintaControls::EnInnerKeys key, bool pressed) = 0;
};

struct MouseStateCallback {
    virtual void OnMouseStateChanged(int x_pos, int y_pos) = 0;
    virtual void OnMouseWeelStateChanged(int param ) = 0;
};

}


#endif
