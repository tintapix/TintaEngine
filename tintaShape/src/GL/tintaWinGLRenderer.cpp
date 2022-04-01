/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaGraphicaPrecompile.h"
#include "tintaWinGLRenderer.h"

namespace Graphica {

	tintaWinGLRenderer::tintaWinGLRenderer (HWND hWnd, tintaFrameBuffer::FormatType eFormat,
							  tintaFrameBuffer::DepthType eDepth, tintaFrameBuffer::StencilType eStencil,
							  tintaFrameBuffer::BufferingType eBuffering,
							  tintaFrameBuffer::MultisamplingType eMultisampling, int iWidth, int iHeight,
                              const tintaRGBAColor &clearColor,
							  int iPixelFormat)
							  :
	tintaGLRenderer(eFormat,eDepth,eStencil,eBuffering,eMultisampling,iWidth,
				   iHeight, clearColor)
	{
		CoreAssert(mWinWidth > 0 && mWinHeight > 0,"mWinWidth > 0 && mWinHeight > 0");
		m_iSaveWidth = mWinWidth;
		m_iSaveHeight = mWinHeight;

		m_hWnd = hWnd;
		m_hWindowDC = ::GetDC(m_hWnd);

		// select format for drawing surface
		PIXELFORMATDESCRIPTOR kPFD;
		memset(&kPFD,0,sizeof(PIXELFORMATDESCRIPTOR));
		kPFD.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		kPFD.nVersion = 1;
		kPFD.dwFlags =
			PFD_DRAW_TO_WINDOW |
			PFD_SUPPORT_OPENGL |
			PFD_GENERIC_ACCELERATED;


		if (mBuffering == tintaFrameBuffer::BT_BUFFERED_DOUBLE)
		{
			kPFD.dwFlags |= PFD_DOUBLEBUFFER;
		}

		// m_kBufferParams.Format is ignored, always create 32-bit RGBA buffer
		kPFD.iPixelType = PFD_TYPE_RGBA;
		kPFD.cColorBits = 32;

		switch (mDepth)
		{
		case tintaFrameBuffer::DT_DEPTH_NONE:  kPFD.cDepthBits =  0;  break;
		case tintaFrameBuffer::DT_DEPTH_16:    kPFD.cDepthBits = 16;  break;
		case tintaFrameBuffer::DT_DEPTH_24:    kPFD.cDepthBits = 24;  break;
		case tintaFrameBuffer::DT_DEPTH_32:    kPFD.cDepthBits = 32;  break;
		default:                          kPFD.cDepthBits = 16;  break;
		}

		if (mStencil == tintaFrameBuffer::ST_STENCIL_8)
		{
			kPFD.cStencilBits = 8;
		}
		else
		{
			kPFD.cStencilBits = 0;
		}

		// set the pixel format for the rendering context
		bool bFirstTime = (iPixelFormat == 0 ? true : false);
		if (bFirstTime)
		{			
			iPixelFormat = ChoosePixelFormat(m_hWindowDC,&kPFD);
			if (iPixelFormat == 0)
			{
				return;
			}
		}

		BOOL bSuccess = SetPixelFormat(m_hWindowDC,iPixelFormat,&kPFD);
		if (!bSuccess)
		{
			CoreAssert(false,"SetPixelFormat(m_hWindowDC,iPixelFormat,&kPFD)");
			return;
		}

		// create an OpenGL context
		m_hWindowRC = wglCreateContext(m_hWindowDC);
		if (!m_hWindowRC)
		{
			CoreAssert(false,"wglCreateContext(m_hWindowDC);");
			return;
		}

		bSuccess = wglMakeCurrent(m_hWindowDC,m_hWindowRC);
		if (!bSuccess)
		{
			CoreAssert(false,"wglMakeCurrent(m_hWindowDC,m_hWindowRC);");
			return;
        }

		initialize();
	}
	
	tintaWinGLRenderer::~tintaWinGLRenderer ()
	{		

		if (m_hWindowRC)
		{
			wglDeleteContext(m_hWindowRC);
		}

		if (m_hWindowDC)
		{
			SelectObject(m_hWindowDC,m_hOldFont);
			ReleaseDC(m_hWnd,m_hWindowDC);
		}
	}
	
	void tintaWinGLRenderer::activate ()
	{
		if (wglGetCurrentContext() != m_hWindowRC)
		{
			wglMakeCurrent(m_hWindowDC,m_hWindowRC);
			tintaGLRenderer::activate();
		}
	}
	
	bool tintaWinGLRenderer::IsActive () const
	{
		return wglGetCurrentContext() == m_hWindowRC;
	}
    void tintaWinGLRenderer::resizeView( int iWidth, int iHeight, bool fullScreen ) {

        tintaGLRenderer::resizeView(iWidth, iHeight, fullScreen);
        if ( fullScreen ) {
            glFinish();

            DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

            UINT uiFlags = SWP_NOACTIVATE | SWP_NOOWNERZORDER |
                SWP_NOSENDCHANGING | SWP_NOZORDER;

            RECT kRect;

            if ( mFullscreen )
            {
                //m_iSaveWidth = mWinWidth;
                //m_iSaveHeight = mWinHeight;
                GetWindowRect(GetDesktopWindow(), &kRect);
                AdjustWindowRect(&kRect, dwStyle, FALSE);
            }
            else
            {
                //mWinWidth = m_iSaveWidth;
                //mWinHeight = m_iSaveHeight;

                kRect.left = 0;
                kRect.top = 0;
                kRect.right = mWinWidth - 1;
                kRect.bottom = mWinHeight - 1;
                AdjustWindowRect(&kRect, dwStyle, FALSE);

                kRect.right -= kRect.left;
                kRect.left = 0;
                kRect.bottom -= kRect.top;
                kRect.top = 0;
            }

            SetWindowPos(m_hWnd, HWND_TOP, kRect.left, kRect.top,
                kRect.right - kRect.left, kRect.bottom - kRect.top, uiFlags);
        }
    }
		
	/*bool tintaWinGLRenderer::beginScene (){
		return wglGetCurrentContext() == m_hWindowRC;
	}*/
	
	void tintaWinGLRenderer::displayBackBuffer (){
		SwapBuffers(m_hWindowDC);
	}
		
	void tintaWinGLRenderer::SaveContext (){
		m_hSaveDC = wglGetCurrentDC();
		m_hSaveRC = wglGetCurrentContext();
	}
	
	void tintaWinGLRenderer::RestoreContext (){
		wglMakeCurrent(m_hSaveDC,m_hSaveRC);
    }
}