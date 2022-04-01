    /*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaGraphicaPrecompile.h"
#include "tintaGLApp.h"

namespace Graphica {


	


	tintaGLApp::tintaGLApp (const char* winTiltle,
		int iXPosition, int iYPosition, int iWidth, int iHeight,
		const tintaRGBAColor& backColor, const String &configName)
		:
	tintaWinApp(winTiltle,iXPosition,iYPosition,iWidth,iHeight,
		backColor, configName)
	{
		// tintaCamera motion
		mWorldAxis[0] = tintaVector3f::origin;
		mWorldAxis[1] = tintaVector3f::origin;
		mWorldAxis[2] = tintaVector3f::origin;
		

		// performance measurements

	}
	
	tintaGLApp::~tintaGLApp ()
	{

	}
	
	bool tintaGLApp::onInitialize ()
	{
		if (!tintaWinApp::onInitialize())
		{
			return false;
		}

        String cam(_M("Cam1"));

        mSceneManager->addCamera(cam);
       
        mpCamera = mSceneManager->getCamera( cam );//NEW_T (tintaCamera)();
        //m_pRenderer->initCenterCamera( mpCamera );

        if ( !mpCamera )
            return false;
                
		m_pRenderer->setCamera( mpCamera );		      

		return true;
	}
	
	void tintaGLApp::OnClose ()
	{
        if( m_pRenderer )
		    m_pRenderer->setCamera(NULL_M);        
		tintaWinApp::OnClose();
	}
	
	void tintaGLApp::OnDisplay ()
	{
		if (m_pRenderer)
		{
			OnIdle();
		}
	}

	void tintaGLApp::InitializeCamera ()
	{
		mWorldAxis[0] = mpCamera->getDVector();
		mWorldAxis[1] = mpCamera->getUVector();
		mWorldAxis[2] = mpCamera->getRVector();
	}	
	
	
	void tintaGLApp::SetRootObject (tintaDrawable* pkMotionObject)
	{

	}
		

}