/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaControls.h"
#include "tintaLogger.h"
#include <tintaTreeConfig/tintaTreeConfig.h>

using namespace Tinta::TreeConfig;

namespace Tinta { 


tintaControls::tintaControls(unsigned keys_quantity)
    : mKeysConvert(keys_quantity, ML_UNASSIGNED)
    , mKeyState(ML_MAX_KEY)
    , mXPos(0)
    , mYPos(0)
{
   //std::bitset<8> b;
    for (size_t i = 1; i < mKeyState.size(); i++) {
        mKeyState.reset(i);
    }
    
}

tintaControls::tintaControls(unsigned keys_quantity, const String &comPath) :
    tintaControls(keys_quantity)  {
    mBindsPath = comPath;
    mCommands.resize(ML_MAX_KEY);
    parseBinds();

}

tintaControls::~tintaControls(void) {
}

void tintaControls::regKeyCallback(KeyStateCallback* listener){
	mKeyCallbacks.insert(listener);
}
void tintaControls::regMouseCallback(MouseStateCallback* listener){
	mMouseCallbacks.insert(listener);
}
void tintaControls::notifyKeyListeners(tintaControls::EnInnerKeys key, bool pressed){
    auto it = mKeyCallbacks.begin();
	for( ;it != mKeyCallbacks.end(); it++){
		(*it)->OnKeyStateChanged(key, pressed);
	}
}

void tintaControls::notifyMouseWeelListeners(int param) {
    auto it = mMouseCallbacks.begin();
    for (; it != mMouseCallbacks.end(); it++) {
        (*it)->OnMouseWeelStateChanged(param);
    }
}


void tintaControls::notifyMouseListeners(int x_new_pos, int y_new_pos){

    auto it = mMouseCallbacks.begin();
	for( ;it != mMouseCallbacks.end(); it++){
		(*it)->OnMouseStateChanged(x_new_pos, y_new_pos);
	}
}

void tintaControls::delKeyCallback( KeyStateCallback* listener ){

	t_key_callbacks::iterator it = mKeyCallbacks.find(listener);
	if( it != mKeyCallbacks.end() ){
		mKeyCallbacks.erase( it );
	}
}
void tintaControls::delMouseCallback( MouseStateCallback* listener ){

	t_mouse_callbacks::iterator it = mMouseCallbacks.find(listener);
	if( it != mMouseCallbacks.end() ){
		mMouseCallbacks.erase( it );
	}
}


void tintaControls::parseBinds( const String &bindsPath) {

    if ( bindsPath.length() > 0 )
        mBindsPath = bindsPath;

    if ( !Tinta::isPathValid(mBindsPath) )
        return;
       
    TreeConfig::tintaTreeConfig keyConf;  

    if ( !keyConf.parse( mBindsPath ) ){

        String err = keyConf.getError();            
        if ( Tinta::tintaLogger::getPtr() )
            Tinta::tintaLogger::getPtr()->logMsg(err, msg_error);
        else
            stream_out << err << _M("\n");            
    }
    size_t keys = keyConf.childQuantity(_M("Keys"));
    for ( auto i = 0; i < (int)keys; i++ ) {

        String vstr;
        StringStream req;
        req << _M("Keys.[") << i << _M("].[0]");
        keyConf.getValue(req.str(), vstr);

        double v(0.);
        req.str(String());
        req << _M("Keys.[") << i << _M("].[1]");
        keyConf.getValue( req.str(), v );

        tintaControls::EnInnerKeys key = (tintaControls::EnInnerKeys)(int)v;
        if ( key >= ML_UNASSIGNED && key < ML_MAX_KEY ) {
            mCommands.at(key) = { downKey  ,vstr };
        }
    }
    //mCommands

    String error = keyConf.getError();
    if (error.length() > 0 ) {
        if (Tinta::tintaLogger::getPtr())
            Tinta::tintaLogger::getPtr()->logMsg( error, msg_error );
    }    
}

String tintaControls::getCommand(EnInnerKeys key, actionBits bits ) const {

    if ( mCommands.at( key ).mKeyState & bits )
        return  mCommands.at( key ).mCommand;

 return "";
}


void tintaControls::bindCommand(EnInnerKeys key, const String &command, actionBits bits ) {
    mCommands.at(key) = { bits, command };
}



bool tintaControls::regPair( unsigned os_key, EnInnerKeys inner_key ){

	assert(mKeysConvert[os_key] == 0x0);
	if(mKeysConvert[os_key] != 0x0 || os_key >= mKeysConvert.size())
		return false;

	mKeysConvert[os_key] = inner_key;
	return true;
}

bool tintaControls::isKeyPressed( EnInnerKeys key ) const{
	return mKeyState[key];	
}


void tintaControls::setMouseWeelState(int param) {
    mWeelParam = param;
    notifyMouseWeelListeners(mWeelParam);
}

int tintaControls::getMouseWeelState() const {
    return mWeelParam;
}


tintaVector2i tintaControls::getMousePos()const {
    return{ mXPos ,mYPos  };

}
void tintaControls::setMousePos(int x_pos, int y_pos){

	
	mXPos = x_pos;
	mYPos = y_pos;
#if DEBUG_MODE

	//StringStream s;
	//s<<_M("Key mouse state changed: ")<<mXPos<<" "<<mYPos;
    //if ( Tinta::tintaLogger::getPtr() )
    //    Tinta::tintaLogger::getPtr()->logMsg(s.str());
#endif
	notifyMouseListeners(x_pos,y_pos);
}
void tintaControls::setKeyState( unsigned os_key, bool is_pressed ){
	EnInnerKeys key = mKeysConvert[os_key];
   
	//StringStream s;	
	//s<<_M("Key state changed: ")<<key<<" is_pressed: "<<is_pressed;
    //if( Tinta::tintaLogger::getPtr() )
	//    Tinta::tintaLogger::getPtr()->logMsg(s.str());
    
	mKeyState[key] = is_pressed;	
	notifyKeyListeners(key, is_pressed);
}

}