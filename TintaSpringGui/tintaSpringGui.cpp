#include "tintaGraphicaPrecompile.h"
#include "tintaSpringGui.h"
#include <cctype>

const int appWidth = 600;
const int appHeight = 600;

const int posX = 0;
const int PosY = 0;

const int sysButtonOffset =2;

tintaSpringGui *tintaSpringGui::m_this = 0;
static const char_m *strTexSpringConfigFileW = _M("config.lua");

using namespace Tinta::TreeConfig;

struct Command {
    int mUnit;
    String mCommand;
};


tintaVector2i posFromGl(const tintaVector2f &posGl, const tintaVector2i &window) {

    tintaVector2i pos { (int)((1.f + posGl.mx) * window.mx)/2, (int)((2.f - (1.f + posGl.my)) * window.my)/2 };
    
    return pos;
}

Command parseCommand(const String &buffer) {


    Command rez;

    rez.mUnit = 0;
    rez.mCommand = buffer;

    if (buffer.size() != 0) {


        int it = 0;
        String unitStr;
        char ch;
        while (it < buffer.size()) {

            ch = buffer.at(it);

            if (std::isdigit(static_cast<unsigned char>(ch))) {
                unitStr.push_back(ch);
                it++;
            }
            else
                break;

        }

        if (unitStr.size() > 0) {
            rez.mUnit = std::stoi(unitStr);
            rez.mCommand = buffer.substr(it);
        }
    }
    return rez;
}

namespace tintaWindowFunc {

    _CoreExport int redraw(SCRIPT_STATE *L) {
       
        tintaSpringGui::m_this->setRedraw();
        return 0;
    }

    _CoreExport int setkey(SCRIPT_STATE *L) {

        tintaControls::EnInnerKeys key = (tintaControls::EnInnerKeys)GET_VAL_UINT(L, 1);

        String execute = GET_VAL_STRING(L, 2);

        bool pushDown = true;
        if (IS_VAL_BOOL(L, 3))
            pushDown = GET_VAL_BOOL(L, 3);

        Graphica::tintaApp* api = Graphica::tintaWinApp::mWinApp;

        if (api) {
            tintaControls *keys = api->GetKeyStates();
            keys->bindCommand(key, execute, pushDown ? tintaControls::downKey : tintaControls::upKey);
        }

        return 0;
    }

    _CoreExport int keypressed(SCRIPT_STATE *L) {

        tintaControls::EnInnerKeys key = (tintaControls::EnInnerKeys)GET_VAL_UINT(L, 1);

        Graphica::tintaApp* api = Graphica::tintaWinApp::mWinApp;

        if (api) {

            tintaControls *keys = api->GetKeyStates();

            PUSH_BOOL_ARG(L, keys->isKeyPressed(key)); //	

            return 1;
        }

        return 0;
    }

    _CoreExport int zoom(SCRIPT_STATE *L) {
        tintaVector2f zoom(GET_VAL_FLOAT(L, 1), GET_VAL_FLOAT(L, 2));
        tintaSpringGui::m_this->setZoom(zoom);
        return 0;
    }

}


static const luaL_Reg window[] = {
    { "redraw", tintaWindowFunc::redraw },
    { "zoom", tintaWindowFunc::zoom },
    { "setkey", tintaWindowFunc::setkey },
    { "keypressed", tintaWindowFunc::keypressed },
    { NULL, NULL } };


int luaopen_window(lua_State *L) {
    luaL_newlib(L, window);
    return 1;
}
static const luaL_Reg loadedWindowlibs[] = {
    { "_G", luaopen_base },
    { "window", luaopen_window },
    { NULL, NULL } };

inline StringBasic readInBuffer(const String &command) {

    StringBasic rez;
    String execute = command;
    //String path( toWideChar( command ) );
    if (execute.length() > 0
        && StringUtil::getFileExt(execute) != _M("lua")) {

        execute.append(_M(".lua"));
    }


    if (Tinta::isPathValid(execute)) {
        // is file
        if (isUTF8WithBOM(execute))
            readUTF8Text(execute, rez);
        else
            readUTF8Text(execute, rez);
    }
    //else
    //    rez = toNarrowChar(command);


    return rez;
}

tintaSpringGui::tintaSpringGui(const String &configName)
	:tintaGLApp( "Demo"
	, posX
	, PosY
	,appWidth
	,appHeight
	,tintaRGBAColor( 1.f,1.f,1.f, 0.0f )
    , configName )
	,mbtnName( "ButtonExit" )
	,mfpsLableName( "fpsRate" )	
    ,mSysIconSize(50)    
{
	mFrameCount = 0;	
    m_this = this; 
   
}


void tintaSpringGui::CalcFps(){
   
    m_uint32 cur = mTimer->getMilliseconds();
    mSecondsElapsed += (cur - mSecondsOld);
    //std::cout << mSecondsElapsed<<" "<< mFpsVal <<"\n";

    if ( mSecondsElapsed > 1000 ) {

        mSecondsElapsed = 0;
        mFpsVal = mFrameCount;
        mFrameCount = 0;             
    }
    else {
        mFrameCount++;
    }

    mSecondsOld = cur;
}


 void tintaSpringGui::onExitClickHndl(double paramFirst, double paramSecond){
	m_this->onExitClick(paramFirst, paramSecond); 		
 }

 void tintaSpringGui::onExitClick(double paramFirst, double paramSecond){
	 
	//OnClose();
 }


 void tintaSpringGui::drawObjects(tintaDrawable* ent) {

     if ( ent ) {
        
     }
 }


 void tintaSpringGui::redraw() {

     if ( tintaTexSpringMain::getPtr() && tintaSpringGui::m_this ) {

        const Tinta::tintaImgStack *images = tintaTexSpringMain::getPtr()->getImageStack();
        const auto img = images->getImgSel();

         if ( images->size() > 0 && tintaSpringGui::m_this->GetRenderer() ) {

             tintaSpringGui::m_this->GetRenderer()->clearBuffers( false );

             int w = img->getWidth();
             int h = img->getHeight();

             float xPos = min((float)w / (float)mWinWidth, 1.f);
             float yPos = min((float)h / (float)mWinHeight, 1.f);

             tintaSpringGui::m_this->GetRenderer()->drawImage( *img, { -xPos , yPos  }, mZoom);

             tintaSpringGui::m_this->GetRenderer()->displayBackBuffer();
         }
     }
}

 void tintaSpringGui::setZoom(tintaVector2f zoom) {
     mZoom = zoom;
 }

 void tintaSpringGui::setRedraw() {
     mbNeedUpdate = true;
 }


void tintaSpringGui::OnIdle (){

     tintaWinApp::OnIdle();              
     
    executeCommands();  

    if ( mbNeedUpdate ) {
        redraw();
    }
    mbNeedUpdate = false;
}



void tintaSpringGui::executeCommands() {
    while (mRoot->getCommands()->quantity() > 0) {

        StringBasic command = mRoot->getCommands()->popFirst();
        if (command.length() > 0) {

                Command c = parseCommand(command);

                if (c.mCommand == _M("help")) {
                    stream_out << _M("(") << THREAD_CURRENT_ID << _M(")") << _M("\n") << helpCommand << _M("\n"); //<<_M("\r")<< flush;                 
                }
                else {


                    if (StringUtil::getFileExt(c.mCommand) == "lua") {
                        StringStream full;
                        full << "main.f(\"" << c.mCommand << "\")";
                        c.mCommand = full.str();
                    }

                    tintaTexSpringMain::getPtr()->executeCommand(c.mUnit, c.mCommand, tintaExecutingTask::enLocalTask);
                    tintaTexSpringMain::getPtr()->addCommand(c.mCommand);
                }
                c = Command();           

        }
    }
}



void tintaSpringGui::OnKeyStateChanged(tintaControls::EnInnerKeys key, bool pressed){

    if ( pressed ) {

        String command = mKeyKeeper.getCommand( key, pressed ? tintaControls::downKey : tintaControls::upKey );

        if ( command.length() > 0 ) {   
                try {

                    StringBasic buffexec = command;
                    tintaScriptContext* execute = mRoot->getContext();

                    execute->executeBuffer( buffexec.c_str(), buffexec.length() );

                    size_t error_count( 0 );
                    const String *errors = execute->getErrors( error_count );

                    for ( unsigned i = 0; i < error_count; i++, errors++ ) {
                        if ( Tinta::tintaLogger::getPtr() )
                            Tinta::tintaLogger::getPtr()->logMsg( *errors );
                        else
                            stream_out << *errors;
                    }
                    execute->resetErrors();
                }
                catch (const tintaException &e) {
                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(e.getDescr(), msg_error);
                }
            
        }
    }
    /*
	if( key == tintaControls::ML_LCONTROL && pressed == true ){
        

        Tinta::tintaImgStack *images = tintaTexSpringMain::getPtr()->getImageStack();
        tintaUInt8Image* img = images->getImgSelEx();

        if (  img  ) {

            m_uint32 idx = images->getIndexImgSel();
            
            tintaVector2f pos = images->getPosition(idx);
            tintaVector2i offset = posFromGl( pos, { mWinWidth, mWinHeight } );

            float xPix =  ((float)img->getWidth() * mZoom.mx ) / (float)img->getWidth() ;
            float yPix =  ((float)img->getHeight() * mZoom.my ) / (float)img->getHeight() ;

            float x = (float)(mMousePos.mx - offset.mx);
            float y = (float)(mMousePos.my - offset.my);

            float xPos = floor(x * xPix);
            float yPos = floor(y * yPix);

            img->setPixel( { (int)xPos, (int)yPos }, { 0, 0, 0 } );
            
            //cout << mMousePos.mx - offset.mx << " " << mMousePos.my - offset.my << "\n";
            mbNeedUpdate = true;
        }
    }
    */
}

void tintaSpringGui::OnMouseStateChanged( int xPos, int yPos ){

    mMousePos.mx = xPos;
    mMousePos.my = yPos;
}

void tintaSpringGui::OnMouseWeelStateChanged(int param) {
    
    if ( param > 0 ) {
        if (mZoom.mx <= 1.f || mZoom.my <= 1.f) {
            mZoom.mx += mZoomStepMin;
            mZoom.my += mZoomStepMin;
        }
        else {
            mZoom.mx += mZoomStep;
            mZoom.my += mZoomStep;
        }
    }

    if ( param < 0 ) {
        if ( mZoom.mx <= 1.f || mZoom.my <= 1.f) {
            mZoom.mx -= mZoomStepMin;
            mZoom.my -= mZoomStepMin;
        }
        else {
            mZoom.mx -= mZoomStep;
            mZoom.my -= mZoomStep;
        }
    }

    if (mZoom.mx <= 0.f)
        mZoom.mx = mZoomStepMin;
    if (mZoom.my <= 0.f)
        mZoom.my = mZoomStepMin;
        
    mbNeedUpdate = true;
}


void tintaSpringGui::OnResize(int iWidth, int iHeight, bool bFullscreen) {
    tintaWinApp::OnResize(iWidth, iHeight, bFullscreen);
    mbNeedUpdate = true;    
}

bool tintaSpringGui::onInitialize ()
{
	if (!tintaGLApp::onInitialize())
	{
		return false;
	}   

	mTimer->reset();
    
    mSecondsOld = mTimer->getMilliseconds();
    const tintaGraphicConfig *config = mRoot->getConfigurator();

    if ( config )
        mpCamera->setPerspective( config->isPerspective() );

    mPerspective = config->isPerspective();

    tintaVector3f camPos( 0.f, 0.f, 0.0f );
    tintaVector3f dir( 0.f, 0.0f, -1.0f );
    tintaVector3f camUp( 0.0f, 1.0f, 0.0f );
    tintaVector3f right = dir.cross( camUp );

    mpCamera->setFrame( camPos, dir, camUp, right );
    mpCamera->onUpdate();

    try {
	    CreateScene();
    }
    catch( const std::exception &ex ){
        OnClose();
        std::cout << ex.what() << "\n";
        return false;
    }

	// initial culling of scene
	mCuller.setCamera(mpCamera);
    if ( mSceneManager && mSceneManager->getRoot() )
        mCuller.ComputeVisibleSet(mSceneManager->getRoot());

	InitializeCamera();
    SetRootObject(mSceneManager->getRoot());

	mKeyKeeper.regKeyCallback(this);
	mKeyKeeper.regMouseCallback(this);

    mCommands = THREAD_TYPE( [=]() {
        
        if (tintaLogger::getPtr())
            tintaLogger::getPtr()->logMsg(_M("Enter lua file name or lua command: \nexecute, execute.lua, urok1/execute.lua, urok1/execute, init - to recreate lua context, fps - to get fps, period - to get main timer period(for screenshots sequence),extensions - to print out GL extensions, entities - to print entities lists"), msg_info, true);

        while (tintaSpringGui::m_this ) {

            String command;
            stream_in >> command;                                  

            StringBasic commandBuff = readInBuffer(command);
            if ( commandBuff.length() == 0 )
                commandBuff = command;
            if ( commandBuff.length() > 0 && mRoot )
                mRoot->getCommands()->push(commandBuff);            
        }
    });


    

    /*if ( mpCamera ) {
       
        if ( mRoot ) {
            Graphica::tintaGLRenderer * rend = mRoot->getRenderer();
            tintaVector2f aRatio = rend->getAspectRatio();
           mpCamera->setFrustum(-aRatio.mx, aRatio.mx, -aRatio.my, aRatio.my, 2.5f, 10000.f);           
        }
    }*/
        
    mConsole = NEW_T(tintaTexSpringMain)();
   
    mConsole->setServMode( true );

    mContext = NEW_T(tintaScriptContext)();

    bool inited = mConsole->initialize(strTexSpringConfigFileW, &mCmdOut, mContext);
    
    const luaL_Reg *lib;
    
    for (lib = loadedWindowlibs; lib->func; lib++) {
        luaL_requiref(mContext->getStateEx(), lib->name, lib->func, 1);
        lua_pop(mContext->getStateEx(), 1);
    }
    

	return true;
}

void tintaSpringGui::CreateScene (){

   

}

void tintaSpringGui::OnClose(){

	mKeyKeeper.delKeyCallback(this);
	mKeyKeeper.delMouseCallback(this);

}

tintaSpringGui::~tintaSpringGui(void){

    DELETE_T(mContext, tintaScriptContext);
    DELETE_T(mConsole, tintaTexSpringMain);
}

