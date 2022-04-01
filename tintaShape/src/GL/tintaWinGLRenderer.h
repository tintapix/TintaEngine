/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef TINTA_WIN_GL_RENDERER_H
#define TINTA_WIN_GL_RENDERER_H


#include "tintaGLRenderer.h"

namespace Graphica {


	class _CoreExport tintaWinGLRenderer
		: public tintaGLRenderer
	{
	public:

		tintaWinGLRenderer (HWND hWnd, tintaFrameBuffer::FormatType eFormat,
			tintaFrameBuffer::DepthType eDepth, tintaFrameBuffer::StencilType eStencil,
			tintaFrameBuffer::BufferingType eBuffering,
			tintaFrameBuffer::MultisamplingType eMultisampling, int iWidth,
			int iHeight,  const tintaRGBAColor &clearColor, int iPixelFormat = 0);

		virtual ~tintaWinGLRenderer ();

		virtual void activate ();
		virtual bool IsActive () const;
		//virtual void resolution ();
        virtual void resizeView(int iWidth, int iHeight, bool fullScreen = false);
		//virtual bool beginScene ();
		virtual void displayBackBuffer ();
		
		HWND GetWindow () { 
			return m_hWnd; 
		}
		void SetWindow (HWND hWnd) { 
			m_hWnd = hWnd; 
		}
		HDC GetDC () { return m_hWindowDC; }
		HGLRC GetRC () { return m_hWindowRC; }

	protected:
		// context handling
		virtual void SaveContext ();
		virtual void RestoreContext ();

		// window parameters
		HWND m_hWnd;
		HDC m_hWindowDC;
		HGLRC m_hWindowRC;
		HGDIOBJ m_hOldFont;

		// for switching contexts
		HDC m_hSaveDC;
		HGLRC m_hSaveRC;

		// fullscreen/window support
		int m_iSaveWidth, m_iSaveHeight;
	};

}
#endif