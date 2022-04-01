/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_WIN_APP_H_
#define _TINTA_WIN_APP_H_



#include "GL/tintaGLPredefine.h"
#include "tintaRGBAColor.h"
#include "GL/tintaWinGLRenderer.h"
#include "tintaApp.h"
#include "tintaRoot.h"
#include "tintaGraphicConfig.h"
#include "tintaSceneManager.h"
#include <clocale>

namespace Graphica {
	
	 class _CoreExport tintaWinApp
		 : public tintaApp
	 {
	 public:
         

 		tintaWinApp ( const char* winTiltle, int iXPosition, int iYPosition, int iWidth, int iHeight,	const tintaRGBAColor& backColor, const String &configNamne );

 		virtual ~tintaWinApp ();	 
 		
 		virtual int Main (int iQuantity, char** apcArgument);   
       
        void SetRenderer(tintaGLRenderer* pkRenderer);       
 		 		
 		int winXPos () const;
 		int winYPos () const;
 		int winWidth () const;
 		int winHeight () const;
  		
  		tintaGLRenderer* GetRenderer ();
 		void SetWindowID (int iWindowID);      
 		int GetWindowID () const;

 		// events
     
 		virtual bool OnPrecreate ();
 		virtual bool onInitialize ();
 		virtual void OnClose ();
 		
 		virtual void OnResize (int iWidth, int iHeight, bool bFullscreen);
 		virtual void OnDisplay ();
 		virtual void OnIdle (); 	
     
        virtual void Show(bool show);
        virtual bool Visible() const;
        virtual void OnMove(int iX, int iY);
 			
		bool initInputs();        	 
 		
 		int KEY_TERMINATE;  // default KEY_ESCAPE, redefine as desired
 		
        static tintaWinApp* mWinApp;        
        bool                mExit;
        bool                mbNeedUpdate;

        virtual bool setCursor( const String &path );

        tintaRoot		 *getRoot();

       

	 protected:	  		
         
 		const char* mWindowTitle;
        int mWinXPosition;
        int mWinYPosition;
        int mWinWidth;
        int mWinHeight;
        bool mFullscreen;
        //bool mFlatDrawMode = false;

        bool mVisible = true;

        HWND mWnd = nullptr;

		tintaRGBAColor mBgrColor;
	 
 		// An identifier for the window (representation is platform-specific).
 		int mWindowID;		

  		tintaFrameBuffer::FormatType         mFormat;
  		tintaFrameBuffer::DepthType          mDepth;
  		tintaFrameBuffer::StencilType        mStencil;
  		tintaFrameBuffer::BufferingType      mBuffering;
  		tintaFrameBuffer::MultisamplingType  mMultisampling;


		tintaRoot		      *mRoot;
  		tintaGLRenderer       *m_pRenderer;		
        tintaSceneManager     *mSceneManager;
      
        HCURSOR mCursor = NULL_M;       
	 };	 
	 
	 

 }
#endif