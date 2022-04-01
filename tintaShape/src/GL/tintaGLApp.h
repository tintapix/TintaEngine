/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_GL_APP_H_
#define _TINTA_GL_APP_H_

#include "tintaWinApp.h"

#include "tintaGraphicaPredefine.h"
#include <Math/tintaVector3.h>
#include <Math/tintaMatrix33.h>
#include "tintaRGBAColor.h"
#include "tintaCamera.h"


namespace Graphica
{
	

	class _CoreExport tintaGLApp:
        public tintaWinApp {
	public:
		tintaGLApp (const char* winTiltle, int iXPosition,
			int iYPosition, int iWidth, int iHeight,
			const tintaRGBAColor& backColor, const String &configName );

		virtual ~tintaGLApp ();

		// event callbacks
		virtual bool onInitialize ();

		virtual void OnClose ();

		virtual void OnDisplay ();

        

	protected:
		
		void InitializeCamera ();
		
		tintaCamera* mpCamera;
		tintaVector3f mWorldAxis[3];		
		

		// object motion
        void SetRootObject(tintaDrawable* pkMotionObject);
	};

}

#endif