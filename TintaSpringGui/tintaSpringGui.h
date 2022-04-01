#ifndef _TINTA_SPRING_GUI_H_
#define _TINTA_SPRING_GUI_H_

#include <GL/tintaGLApp.h>

#include "tintaIndexBuffer.h"
#include "tintaVertexBuffer.h"
#include <tintaCuller.h>
#include "tintaTriMesh.h"
#include <tintaControls.h>
#include <tintaImgStack.h>
#include <tintaTexSpringMain.h>


#include "tintaTimer.h"

using namespace Graphica;
using namespace Tinta;


class molyWinConsoleCmd : public tintaConsoleOutput {
public:
    ~molyWinConsoleCmd() {
    }

    virtual void printMsg(const Tinta::String &msg, msgLevel stat) {
#if CORE_PLATFORM  == CORE_PLATFORM_WIN32       
        tintaWString print = StringUtil::createUTF16String(msg);
        if (print.length() != 0) {
            std::wcout << StringUtil::createUTF16String(msg) << std::endl;
            std::wcout.clear();
        }
        else {
            stream_out << msg << _M("\n");
            stream_out.clear();
        }
#else
        stream_out << msg << _M("\n");
        stream_out.clear();
        //      stream_out.clear();
#endif

    }

    virtual void clear() {

        system("cls");

    }

    void registrate() {
        if (Tinta::tintaLogger::getPtr())
            Tinta::tintaLogger::getPtr()->addConsole(this);
    }

    void remove() {
        if (Tinta::tintaLogger::getPtr())
            Tinta::tintaLogger::getPtr()->removeConsole(this);
    }
    virtual void setTitle(const String &title) {
        //SetConsoleTitle(_M("TexSpring Console"));
    }
};

namespace tintaWindowFunc {

    _CoreExport int redraw(SCRIPT_STATE *L);

}

class tintaSpringGui
	: public tintaGLApp
	, public KeyStateCallback
	, public MouseStateCallback	
	
{
public:
    tintaSpringGui(const String &configName);

	virtual ~tintaSpringGui(void);

	virtual bool onInitialize ();
	virtual void OnClose ();

	void CreateScene ();

	tintaCuller mCuller;

    static tintaSpringGui *m_this;

	float mTimeOld;
	virtual void OnIdle ();
    virtual void OnResize(int iWidth, int iHeight, bool bFullscreen);

	// from KeyStateCallback
	virtual void OnKeyStateChanged(tintaControls::EnInnerKeys key, bool pressed);
	// from MouseStateCallback
	virtual void OnMouseStateChanged(int x, int y );
    virtual void OnMouseWeelStateChanged(int param);	
    virtual void executeCommands();
    void redraw();
    void setRedraw();
    void setZoom(tintaVector2f zoom);
	
private:
    void drawObjects( tintaDrawable* ent );
	void CalcFps();
	m_uint32 mSecondsElapsed = 0;
    m_uint32 mSecondsOld = 0;
    m_uint32 mFrameCount = 0;
    m_uint32 mFpsVal;

    float mZoomStep = 0.05f;
    float mZoomStepMin = 0.05f;

	StringBasic mbtnName;
	StringBasic mfpsLableName;
    bool        mPerspective = true;
    
    
	int mSysIconSize;

    tintaTexSpringMain *mConsole = NULL;
    tintaScriptContext* mContext = NULL_M;

    THREAD_TYPE      mCommands;

    molyWinConsoleCmd      mCmdOut;
    tintaVector2f mZoom = {1.f, 1.f};
    tintaVector2i mMousePos = { 0, 0 };
    tintaVector2i mCamera = { 0, 0 };

	
	static void onExitClickHndl(double paramFirst, double paramSecond);	
	void onExitClick(double paramFirst, double paramSecond);	
};




#endif