/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaGraphicaPrecompile.h"
#include "tintaGraphicaPredefine.h"
#include <tintaCommon.h>

#include "tintaApp.h"
#include "tintaWinApp.h"

using namespace Graphica;

tintaApp::EntryPoint tintaApp::Run = 0;

//Application* Application::TheApplication = 0;


tintaApp::tintaApp() : 
        mKeyKeeper(0xFF, _M("keys.mfg")) {

    mTimer = NEW_T(tintaTimer)();
    mTimer->reset();
    tintaITimerCallback *c = mTimerObserved.getFirst();
    for (; c; c = mTimerObserved.getNext()) { c->onReset(); }
    mTimerPeriod = 0;

}

tintaApp::~tintaApp()
{
		 // impl mev DELETE_T (Application::TheApplication, tintaGLApp);
}


m_uint32 tintaApp::addOnTimerBuff(m_uint32 elapsed, const StringBasic &execute) {
    
    mTimerScripts.push_back({ ++mTimersKey, elapsed, getTime() , execute });

    return mTimersKey;
}

bool tintaApp::delTimer(m_uint32 pos) {

    onTimer_t::iterator f = std::find_if(mTimerScripts.begin(), mTimerScripts.end(), [&](auto &val) { return val.mIndex == pos;  });
    if (f == mTimerScripts.end())
        return false;

    mTimerScripts.erase(f);

    return true;
}

bool  tintaApp::setTimer(m_uint32 pos, const String &script) {
    
    onTimer_t::iterator f = std::find_if(mTimerScripts.begin(), mTimerScripts.end(), [&](auto &val) { return val.mIndex == pos;  });
    if (f == mTimerScripts.end())
        return false;

    f->mBuffer = script;

    return true;
}

bool  tintaApp::setTimer(m_uint32 pos, m_uint32 elapsed) {
    onTimer_t::iterator f = std::find_if(mTimerScripts.begin(), mTimerScripts.end(), [&](auto &val) { return val.mIndex == pos;  });
    if (f == mTimerScripts.end())
        return false;

    f->mElapsed = elapsed;

    return true;
}

void tintaApp::setManual( int timerPeriod ) {       
    
    mTimerPeriod = (m_uint32)timerPeriod;    
}


void tintaApp::delTimers() {

    mTimerScripts.clear();
}

m_uint32 tintaApp::getTime() const {
    return  mElapsed;
}

size_t tintaApp::timers() const {
    return mTimerScripts.size();
}


void tintaApp::addCallback(tintaITimerCallback * v) {
    mTimerObserved.addCallback(v);
}

tintaControls* tintaApp::GetKeyStates() {
    return &mKeyKeeper;
}



/*
int main (int args, char* apcArgument[]){	

	Application::Run = &tintaWinApp::Run;

	Application::AppInitialize ();
    
    //Application* app = new tintaWinApp();
	int ret = 0;


	   if ( Application::Run ){   
			ret = Application::Run( args,apcArgument );     
       }
	    else {
			ret = INT_MAX;
		}    
	Application::AppRelease();

	Application::TheApplication = 0;

	return ret;
}
*/




//#endif