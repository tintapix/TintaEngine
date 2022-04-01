/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaGraphicaPrecompile.h"
#include "tintaWinApp.h"


namespace Graphica {	

	// These are in the VC7.1 basetsd.h, but not the VC6 basetsd.h.
#ifndef IntToPtr
#define IntToPtr(i) ((VOID*)(INT_PTR)((int)i))
#endif

#ifndef PtrToUint
#define PtrToUint(p) ((UINT)(UINT_PTR)(p))
#endif

#ifndef PtrToInt
#define PtrToInt(p)  ((INT)(INT_PTR)(p))
#endif

#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 0x020A
#endif

#ifndef WHEEL_DELTA
#define WHEEL_DELTA 120
#endif


    tintaWinApp*  tintaWinApp::mWinApp = 0;

    tintaWinApp::tintaWinApp(
        const char* winTiltle,
        int iXPosition,
        int iYPosition,
        int iWidth,
        int iHeight,
        const tintaRGBAColor& backColor,        
        const String &configName
    ) :
        mWindowTitle(winTiltle),
        mWinXPosition(iXPosition),
        mWinYPosition(iYPosition),
        mWinWidth(iWidth),
        mWinHeight(iHeight),
        mBgrColor(backColor),      
        mRoot(NULL_M),
        m_pRenderer(NULL_M),        
        mExit(false),
        mbNeedUpdate( false ){        
		
		mWindowID = 0;	
			
		mRoot = NEW_T (tintaRoot)(configName);

        mSceneManager = NEW_T(tintaSceneManager);

        if( mRoot )
            mRoot->setSceneManager( mSceneManager );

 		mFormat = tintaFrameBuffer::FT_FORMAT_RGBA;
 		mDepth = tintaFrameBuffer::DT_DEPTH_24;
 		mStencil = tintaFrameBuffer::ST_STENCIL_8;
 		mBuffering = tintaFrameBuffer::BT_BUFFERED_DOUBLE;
 		mMultisampling = tintaFrameBuffer::MT_SAMPLING_NONE;	
       
		initInputs();

             
	}
	
	tintaWinApp::~tintaWinApp (){


        if ( mSceneManager ) {
            DELETE_T(mSceneManager, tintaSceneManager);
            mSceneManager = NULL_M;
        }

        if ( mRoot ){

            DELETE_T(mRoot, tintaRoot);
            mRoot = NULL_M;
        }


        if (m_pRenderer){
            DELETE_T(m_pRenderer, tintaGLRenderer);
            m_pRenderer = NULL_M;
        }

        DELETE_T(mTimer, tintaTimer);
	}
	

    

	bool tintaWinApp::initInputs(){

		bool rez = true;
		rez = rez && mKeyKeeper.regPair(VK_LBUTTON, tintaControls::ML_MOUSE_LEFT);
		rez = rez && mKeyKeeper.regPair(VK_MBUTTON, tintaControls::ML_MOUSE_MIDDLE);
		rez = rez && mKeyKeeper.regPair(VK_RBUTTON, tintaControls::ML_MOUSE_RIGHT);

		rez = rez && mKeyKeeper.regPair(VK_BACK, tintaControls::ML_BACK);
		rez = rez && mKeyKeeper.regPair(VK_TAB, tintaControls::ML_TAB);
		//mKeyKeeper.regPair(VK_CLEAR, tintaControls::ML_TAB);
		rez = rez && mKeyKeeper.regPair(VK_RETURN, tintaControls::ML_RETURN);
		rez = rez && mKeyKeeper.regPair(VK_LSHIFT, tintaControls::ML_LSHIFT);
		rez = rez && mKeyKeeper.regPair(VK_RSHIFT, tintaControls::ML_RSHIFT);

		rez = rez && mKeyKeeper.regPair(VK_LCONTROL, tintaControls::ML_LCONTROL);
		rez = rez && mKeyKeeper.regPair(VK_RCONTROL, tintaControls::ML_RCONTROL);
		// ALT key
		rez = rez && mKeyKeeper.regPair(VK_LMENU, tintaControls::ML_LMENU);
        rez = rez && mKeyKeeper.regPair(VK_RMENU, tintaControls::ML_RMENU);
		rez = rez && mKeyKeeper.regPair(VK_ESCAPE, tintaControls::ML_ESCAPE);
		rez = rez && mKeyKeeper.regPair(VK_SPACE, tintaControls::ML_SPACE);
		// page up
		rez = rez && mKeyKeeper.regPair(VK_PRIOR, tintaControls::ML_PGUP);
		// page down
		rez = rez && mKeyKeeper.regPair(VK_NEXT, tintaControls::ML_PGDOWN);

		rez = rez && mKeyKeeper.regPair(VK_END, tintaControls::ML_END);
		rez = rez && mKeyKeeper.regPair(VK_HOME, tintaControls::ML_HOME);
		rez = rez && mKeyKeeper.regPair(VK_LEFT, tintaControls::ML_LEFT);
		rez = rez && mKeyKeeper.regPair(VK_UP, tintaControls::ML_UP);
		rez = rez && mKeyKeeper.regPair(VK_RIGHT, tintaControls::ML_RIGHT);
		rez = rez && mKeyKeeper.regPair(VK_DOWN, tintaControls::ML_DOWN);
		rez = rez && mKeyKeeper.regPair(VK_INSERT, tintaControls::ML_INSERT);
        rez = rez && mKeyKeeper.regPair(VK_OEM_MINUS, tintaControls::ML_MINUS);
        rez = rez && mKeyKeeper.regPair(VK_OEM_PLUS, tintaControls::ML_EQUALS);
        

		// digits
		rez = rez && mKeyKeeper.regPair(0x30, tintaControls::ML_0);
		rez = rez && mKeyKeeper.regPair(0x31, tintaControls::ML_1);
		rez = rez && mKeyKeeper.regPair(0x32, tintaControls::ML_2);
		rez = rez && mKeyKeeper.regPair(0x33, tintaControls::ML_3);
		rez = rez && mKeyKeeper.regPair(0x34, tintaControls::ML_4);
		rez = rez && mKeyKeeper.regPair(0x35, tintaControls::ML_5);
		rez = rez && mKeyKeeper.regPair(0x36, tintaControls::ML_6);
		rez = rez && mKeyKeeper.regPair(0x37, tintaControls::ML_7);
		rez = rez && mKeyKeeper.regPair(0x38, tintaControls::ML_8);
		rez = rez && mKeyKeeper.regPair(0x39, tintaControls::ML_9);
		// chars
		// A ...
		rez = rez && mKeyKeeper.regPair(0x41, tintaControls::ML_A);
		rez = rez && mKeyKeeper.regPair(0x42, tintaControls::ML_B);
		rez = rez && mKeyKeeper.regPair(0x43, tintaControls::ML_C);
		rez = rez && mKeyKeeper.regPair(0x44, tintaControls::ML_D);
		rez = rez && mKeyKeeper.regPair(0x45, tintaControls::ML_E);
		rez = rez && mKeyKeeper.regPair(0x46, tintaControls::ML_F);
		rez = rez && mKeyKeeper.regPair(0x47, tintaControls::ML_G);
		rez = rez && mKeyKeeper.regPair(0x48, tintaControls::ML_H);
		rez = rez && mKeyKeeper.regPair(0x49, tintaControls::ML_I);
		rez = rez && mKeyKeeper.regPair(0x4A, tintaControls::ML_J);
		rez = rez && mKeyKeeper.regPair(0x4B, tintaControls::ML_K);
		rez = rez && mKeyKeeper.regPair(0x4C, tintaControls::ML_L);
		rez = rez && mKeyKeeper.regPair(0x4D, tintaControls::ML_M);
		rez = rez && mKeyKeeper.regPair(0x4E, tintaControls::ML_N);
		rez = rez && mKeyKeeper.regPair(0x4F, tintaControls::ML_O);
		rez = rez && mKeyKeeper.regPair(0x50, tintaControls::ML_P);
		rez = rez && mKeyKeeper.regPair(0x51, tintaControls::ML_Q);
		rez = rez && mKeyKeeper.regPair(0x52, tintaControls::ML_R);
		rez = rez && mKeyKeeper.regPair(0x53, tintaControls::ML_S);
		rez = rez && mKeyKeeper.regPair(0x54, tintaControls::ML_T);
		rez = rez && mKeyKeeper.regPair(0x55, tintaControls::ML_U);
		rez = rez && mKeyKeeper.regPair(0x56, tintaControls::ML_V);
		rez = rez && mKeyKeeper.regPair(0x57, tintaControls::ML_W);
		rez = rez && mKeyKeeper.regPair(0x58, tintaControls::ML_X);
		rez = rez && mKeyKeeper.regPair(0x59, tintaControls::ML_Y);
		rez = rez && mKeyKeeper.regPair(0x5A, tintaControls::ML_Z);

		// numpad
		rez = rez && mKeyKeeper.regPair(VK_NUMPAD0, tintaControls::ML_NUMPAD0);
		rez = rez && mKeyKeeper.regPair(VK_NUMPAD1, tintaControls::ML_NUMPAD1);
		rez = rez && mKeyKeeper.regPair(VK_NUMPAD2, tintaControls::ML_NUMPAD2);
		rez = rez && mKeyKeeper.regPair(VK_NUMPAD3, tintaControls::ML_NUMPAD3);
		rez = rez && mKeyKeeper.regPair(VK_NUMPAD4, tintaControls::ML_NUMPAD4);
		rez = rez && mKeyKeeper.regPair(VK_NUMPAD5, tintaControls::ML_NUMPAD5);
		rez = rez && mKeyKeeper.regPair(VK_NUMPAD6, tintaControls::ML_NUMPAD6);
		rez = rez && mKeyKeeper.regPair(VK_NUMPAD7, tintaControls::ML_NUMPAD7);
		rez = rez && mKeyKeeper.regPair(VK_NUMPAD8, tintaControls::ML_NUMPAD8);
		rez = rez && mKeyKeeper.regPair(VK_NUMPAD9, tintaControls::ML_NUMPAD9);
		// upper row
		rez = rez && mKeyKeeper.regPair(VK_MULTIPLY, tintaControls::ML_MULTIPLY);
		rez = rez && mKeyKeeper.regPair(VK_ADD, tintaControls::ML_ADD);
		rez = rez && mKeyKeeper.regPair(VK_SEPARATOR, tintaControls::ML_SLASH);
		rez = rez && mKeyKeeper.regPair(VK_SUBTRACT, tintaControls::ML_SUBTRACT);
		rez = rez && mKeyKeeper.regPair(VK_DECIMAL, tintaControls::ML_PERIOD);
		rez = rez && mKeyKeeper.regPair(VK_DIVIDE, tintaControls::ML_DIVIDE);
		// F..
		rez = rez && mKeyKeeper.regPair(VK_F1, tintaControls::ML_F1);
		rez = rez && mKeyKeeper.regPair(VK_F2, tintaControls::ML_F2);
		rez = rez && mKeyKeeper.regPair(VK_F3, tintaControls::ML_F3);
		rez = rez && mKeyKeeper.regPair(VK_F4, tintaControls::ML_F4);
		rez = rez && mKeyKeeper.regPair(VK_F5, tintaControls::ML_F5);
		rez = rez && mKeyKeeper.regPair(VK_F6, tintaControls::ML_F6);
		rez = rez && mKeyKeeper.regPair(VK_F7, tintaControls::ML_F7);
		rez = rez && mKeyKeeper.regPair(VK_F8, tintaControls::ML_F8);
		rez = rez && mKeyKeeper.regPair(VK_F9, tintaControls::ML_F9);
		rez = rez && mKeyKeeper.regPair(VK_F10, tintaControls::ML_F10);
		rez = rez && mKeyKeeper.regPair(VK_F11, tintaControls::ML_F11);
		rez = rez && mKeyKeeper.regPair(VK_F12, tintaControls::ML_F12);
		rez = rez && mKeyKeeper.regPair(VK_NUMLOCK, tintaControls::ML_NUMLOCK);
		rez = rez && mKeyKeeper.regPair(VK_SCROLL, tintaControls::ML_SCROLL);
		rez = rez && mKeyKeeper.regPair(VK_PAUSE, tintaControls::ML_PAUSE);
        rez = rez && mKeyKeeper.regPair(VK_DELETE, tintaControls::ML_DELETE);
		return rez;

	}
	
	bool tintaWinApp::onInitialize (){
		if(m_pRenderer)
 		    m_pRenderer->setClearColor(mBgrColor);
		return true;
	}
	
	void tintaWinApp::OnClose (){		        
	}
	
	void tintaWinApp::OnMove (int iX, int iY)
	{
		mWinXPosition = iX;
		mWinYPosition = iY;
	}
	
	void tintaWinApp::OnResize( int iWidth, int iHeight, bool bFullscreen ){
		if ( iWidth > 0 && iHeight > 0 ){
            mWinWidth = iWidth;
            mWinHeight = iHeight;
 			if ( m_pRenderer ){
                m_pRenderer->resizeView(mWinWidth, mWinHeight, bFullscreen );
 			}			
		}
	}

    

    tintaRoot		 *tintaWinApp::getRoot() {
        return mRoot;
    }

    void tintaWinApp::OnDisplay() {
    }

    WPARAM MapLeftRightKeys(WPARAM vk, LPARAM lParam)
    {
        WPARAM ret = vk;
        UINT scancode = (lParam & 0x00ff0000) >> 16;
        int extended = (lParam & 0x01000000) != 0;

        switch (vk) {
        case VK_SHIFT:
            ret = MapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX);
            break;
        case VK_CONTROL:
            ret = extended ? VK_RCONTROL : VK_LCONTROL;
            break;
        case VK_MENU:
            ret = extended ? VK_RMENU : VK_LMENU;
            break;
        default:            
            ret = vk;
            break;
        }

        return ret;
    }

 	LRESULT CALLBACK MsWindowEventHandler (HWND hWnd, UINT uiMsg, WPARAM wParam,
 		LPARAM lParam)
 	{
        tintaWinApp* pkTheApp = tintaWinApp::mWinApp;
 
 		if (!pkTheApp || !pkTheApp->GetWindowID())
 		{
 			return DefWindowProc(hWnd,uiMsg,wParam,lParam);
 		}
 

       
 		switch (uiMsg) 
 		{
 		case WM_PAINT:
 			{
 				PAINTSTRUCT kPS;
 				BeginPaint(hWnd,&kPS);
 				pkTheApp->OnDisplay();
 				EndPaint(hWnd,&kPS);                
 				return 0;
 			}
 		case WM_ERASEBKGND:
 			{
 				// This tells Windows not to erase the background (and that the
 				// application is doing so).
 				return 1;
 			}
  		case WM_MOVE:
  			{
  				int iXPos = int(LOWORD(lParam));
  				int iYPos = int(HIWORD(lParam));
  				pkTheApp->OnMove(iXPos,iYPos);
  				return 0;
  			}
  		case WM_SIZE:
  			{
  				int iWidth = int(LOWORD(lParam));
  				int iHeight = int(HIWORD(lParam));
                
  				pkTheApp->OnResize(iWidth,iHeight, false);
  				return 0;
  			}
  		case WM_KEYDOWN:
  			{
                int iVirtKey = MapLeftRightKeys(wParam, lParam);
  				//int iVirtKey = int(wParam);				
                pkTheApp->GetKeyStates()->setKeyState((unsigned)iVirtKey, true);

  				return 0;
  			}
  		case WM_KEYUP:
  			{
                int iVirtKey = MapLeftRightKeys(wParam, lParam);
                pkTheApp->GetKeyStates()->setKeyState((unsigned)iVirtKey, false);
  				return 0;
  			}
        case WM_SYSKEYDOWN:
        {
            /*UINT scancode = (lParam & 0x00ff0000) >> 16;
            int extended = (lParam & 0x01000000) != 0;
            cout << scancode << " " << extended << " \n";
            */
            int iVirtKey = MapLeftRightKeys(wParam, lParam);

            pkTheApp->GetKeyStates()->setKeyState((unsigned)iVirtKey, true);

            return 0;
        }
        case WM_SYSKEYUP:
        {
            int iVirtKey = MapLeftRightKeys(wParam, lParam);
            pkTheApp->GetKeyStates()->setKeyState((unsigned)iVirtKey, false);
            return 0;
        }
  		case WM_LBUTTONDOWN:
  			{
            pkTheApp->GetKeyStates()->setKeyState(VK_LBUTTON, true);
  				return 0;
  			}
        case WM_RBUTTONDOWN:
            {
            pkTheApp->GetKeyStates()->setKeyState(VK_RBUTTON, true);
                return 0;
            }
        case WM_MBUTTONDOWN:
            {
            pkTheApp->GetKeyStates()->setKeyState(VK_MBUTTON, true);
                return 0;
            }
        case WM_LBUTTONUP:
        {
            pkTheApp->GetKeyStates()->setKeyState(VK_LBUTTON, false);
            return 0;
        }
        case WM_RBUTTONUP:
        {
            pkTheApp->GetKeyStates()->setKeyState(VK_RBUTTON, false);
            return 0;
        }
        case WM_MBUTTONUP:
        {
            pkTheApp->GetKeyStates()->setKeyState(VK_MBUTTON, false);
            return 0;
        }
        case WM_MOUSEMOVE:
            {
            pkTheApp->GetKeyStates()->setMousePos(LOWORD(lParam), HIWORD(lParam));
                return 0;
            }
 		case WM_DESTROY:
 			{
 				PostQuitMessage(0);
 				return 0;
 			}
        case WM_MOUSEWHEEL: 
            pkTheApp->GetKeyStates()->setMouseWeelState(GET_WHEEL_DELTA_WPARAM(wParam));
            //cout<<" weel test "<<GET_WHEEL_DELTA_WPARAM(wParam)<<"\n";
 		}
 
 		return DefWindowProc(hWnd,uiMsg,wParam,lParam);
 	}
	

    

    void tintaWinApp::Show( bool show ) {
        if (mWnd) {
            mVisible = show;
            ShowWindow(mWnd, (int)mVisible);
        }

    }

    bool tintaWinApp::Visible() const {
        return mVisible;
    }

    bool tintaWinApp::setCursor( const String &path ) {

        
        String fullPath;
        if ( Tinta::isPathValid( path ) )
            fullPath = path;
        else {
            StringStream path_;
            path_ << Tinta::getRootPath();
            path_ << _M("/");
            path_ << path;
            fullPath = path_.str();                
        }

        if ( Tinta::isPathValid(fullPath) ) {

            mCursor = LoadCursorFromFile(fullPath.c_str());

            if ( !mCursor )
                return false;

            SetCursor(mCursor);
            SetClassLong(mWnd, GCL_HCURSOR, (DWORD)mCursor);
                
        }
        else
            return false;
        
        return true;
    }

    int tintaWinApp::Main(int, char**) {

        setlocale(LC_CTYPE, "rus");

        try {
            if ( mRoot )
                if ( !mRoot->initialize() )
                    return -1;
        }
        catch (const std::exception &ex) {
            OnClose();
            std::cout << ex.what() << "\n";
            return 0;
        }


        const tintaGraphicConfig *config = mRoot->getConfigurator();

        CoreAssert(config, "config");

#if CORE_PLATFORM  == CORE_PLATFORM_WIN32
        const tintaRoot* r = tintaRoot::getPtr();
        if (r && r->getConfigurator() && !r->getConfigurator()->showConsole()) {
            FreeConsole();
        }
#endif
        

        // Register the window class.
        DWORD dwStyle = 0;     //WS_VISIBLE | WS_CLIPCHILDREN|WS_POPUP; //| CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        DWORD dwStyleEx = 0;   //WS_EX_TOPMOST;
        bool isFullScreen  = config->isFullScreen();
        bool titled = config->withTitle();
        if (isFullScreen) {
            dwStyle = (WS_VISIBLE | WS_CLIPCHILDREN | WS_POPUP);
            //dwStyleEx = WS_EX_TOPMOST;
        }
        else {
            //dwStyle = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; //WS_OVERLAPPEDWINDOW;
            if( !titled )
                dwStyle = (WS_VISIBLE | WS_CLIPCHILDREN | WS_POPUP ); // remove close key | WS_SYSMENU);// | WS_MINIMIZEBOX | WS_MAXIMIZEBOX );  
            else
                dwStyle = (WS_OVERLAPPED | WS_THICKFRAME); // remove close key | WS_SYSMENU);// | WS_MINIMIZEBOX | WS_MAXIMIZEBOX );  
        }

        static char_m windowClass[] = _M("Main window");
        WNDCLASS wc;
        wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;// | WS_POPUP;
        wc.lpfnWndProc = MsWindowEventHandler;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = 0;
        wc.hIcon = LoadIcon(0, IDI_APPLICATION);
        wc.hCursor = LoadCursor(0, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        wc.lpszClassName = windowClass;
        wc.lpszMenuName = 0;
        RegisterClass(&wc);

        RECT kRect = { 0, 0, winWidth() - 1, winHeight() - 1 };

        AdjustWindowRect(&kRect, dwStyle, false);

        // Create the application window.
        mWnd = CreateWindowEx(dwStyleEx, windowClass, mWindowTitle,
            dwStyle, winXPos(), winYPos(),
            kRect.right - kRect.left + 1, kRect.bottom - kRect.top + 1, 0, 0, 0, 0);

        // Get monitor info	

        HMONITOR hMonitor = MonitorFromWindow(mWnd, MONITOR_DEFAULTTONEAREST);
        MONITORINFO monitorInfo;

        memset(&monitorInfo, 0, sizeof(MONITORINFO));
        monitorInfo.cbSize = sizeof(MONITORINFO);
        GetMonitorInfo(hMonitor, &monitorInfo);
        
        int top = monitorInfo.rcMonitor.top;
        int left = monitorInfo.rcMonitor.left;
        mWinHeight = monitorInfo.rcMonitor.bottom;
        mWinWidth = monitorInfo.rcMonitor.right;



        if ( !isFullScreen ) {

            if ( config->width() )
                mWinWidth = config->width();
            if ( config->height() )
                mWinHeight = config->height();       
            const tintaVector2i* pos = config->windowPos();
            if (pos) {
                top = pos->mx;
                left = pos->my;
            }
            else {
                top = monitorInfo.rcMonitor.bottom / 2 - mWinHeight / 2;
                left = monitorInfo.rcMonitor.right / 2 - mWinWidth / 2;
            }
        } 
     

        SetWindowPos(mWnd, NULL, left, top, mWinWidth, mWinHeight, SWP_NOACTIVATE);
        if (titled) {
            RECT rc = { 0, 0, mWinWidth, mWinHeight };
            AdjustWindowRectEx(&rc, dwStyle, TRUE, dwStyle);
            mWinWidth += rc.left;
            mWinHeight += rc.top;
        }

		SetWindowID(PtrToInt(mWnd));
		int iPixelFormat = 0;	

		tintaGLRenderer* rend = NEW_T ( tintaWinGLRenderer )(mWnd, mFormat, mDepth, mStencil, mBuffering, mMultisampling, mWinWidth, mWinHeight, mBgrColor , iPixelFormat);
        addCallback(rend);
        mRoot->setRenderer(rend);
     


        

        //SetCursor(customCursor);


     
		SetRenderer(rend);
		bool initialized = onInitialize();
		if(!initialized)
			return 0;
			
		ShowWindow(mWnd, (int)mVisible );

		UpdateWindow(mWnd);

        

		bool bApplicationRunning = true;

		while ( bApplicationRunning )
		{
            if ( mExit )
                bApplicationRunning = false;
            
            
            MSG kMsg;
            if (PeekMessage(&kMsg, 0, 0, 0, PM_REMOVE))
            {
                if (kMsg.message == WM_QUIT)
                {

                    bApplicationRunning = false;                        
                    continue;
                }

                HACCEL hAccel = 0;
                if (!TranslateAccelerator(mWnd, hAccel, &kMsg))
                {
                    TranslateMessage(&kMsg);
                    DispatchMessage(&kMsg);
                }
            }
            else {
                OnIdle();
            }
            
		}
		
	
		OnClose();
		return 0;
	}
    
    void tintaWinApp::OnIdle() {

        mElapsed = mTimerPeriod > 0  ? (mElapsed + mTimerPeriod) : mTimer->getMilliseconds();

        //cout << elapsed << "\n";
        tintaITimerCallback *c = mTimerObserved.getFirst();
        for (; c; c = mTimerObserved.getNext()) { c->onProcess(mElapsed); }
              

        for ( tintaTimerScript &i : mTimerScripts ) {

            m_uint32 v = mElapsed - i.mLast;
            if ( v >= i.mElapsed ) {
                mRoot->getCommands()->push(i.mBuffer);
                i.mLast = mElapsed;
            }
        }

    }

    int tintaWinApp::winXPos() const {
        return mWinXPosition;
    }

    int tintaWinApp::winYPos() const {
        return mWinYPosition;
    }

    int tintaWinApp::winWidth() const {
        return mWinWidth;
    }

    int tintaWinApp::winHeight() const {
        return mWinHeight;
    }

    tintaGLRenderer* tintaWinApp::GetRenderer()
    {
        return m_pRenderer;
    }

    void tintaWinApp::SetRenderer(tintaGLRenderer* pRenderer)
    {
        m_pRenderer = pRenderer;
        if (mSceneManager)
            mSceneManager->setRenderer(m_pRenderer);

    }

    void tintaWinApp::SetWindowID(int iWindowID) {
        mWindowID = iWindowID;
    }

    int tintaWinApp::GetWindowID() const {
        return mWindowID;
    }

    bool tintaWinApp::OnPrecreate() {

        return true;
    }

    
}