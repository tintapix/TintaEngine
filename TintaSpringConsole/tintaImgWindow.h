/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov 
tintapix.com
tintapix@gmail.com  */

#ifndef _TINTA_IMG_WINDOW_H_
#define _TINTA_IMG_WINDOW_H_

#include <tintaIImgWindow.h>
#include <tintaMemoryAllocator.h>

#ifdef USE_GL_WINDOW

#include <GL/tintaGLApp.h>
#include <GL/tintaGLRenderer.h>
#include <tintaControls.h>
#include <tintaGraphicaPredefine.h>

namespace Tinta
{
    
    class tintaImgWindow :  public tintaIImgWindow
                          //, public AllocatedObjectCommon
                          , public Graphica::tintaGLApp
                          , public KeyStateCallback
                          , public MouseStateCallback
    {
    public:

        static String mConfigName;
        tintaImgWindow();
        

        virtual ~tintaImgWindow();

        virtual void create();

        virtual void show(bool bShow);        

        virtual bool shown() const;


        virtual void showImage( m_uint32 key );

        virtual void addData( m_uint32 key, const tintaUInt8Image * data );

        virtual void updateData( m_uint32 key );

        virtual void deleteData( m_uint32 key = ZERO_ID );
        
        virtual bool onInitialize();

        virtual void OnClose();

        virtual void OnIdle();

        void posImage( const Graphica::tintaImage * img );

        virtual void setData( m_uint32 key, m_uint32 index , const tintaPixel24b *color );

        virtual void setData( m_uint32 key, m_uint32 x, m_uint32 y, const tintaPixel24b *color );

        virtual void setData(m_uint32 key, const tintaUInt8Image * data);

        // from KeyStateCallback
        virtual void OnKeyStateChanged( tintaControls::EnInnerKeys key, bool pressed );
        // from MouseStateCallback
        virtual void OnMouseStateChanged(int x, int y);       

        virtual bool keyState(int key, bool &bValid) const;

        virtual tintaVector2i mouseWinPos() const;

        virtual tintaVector2i mouseImgPos() const;

        void OnMouseWeelStateChanged(int param);

        virtual void OnResize(int iWidth, int iHeight, bool bFullscreen);
        
        virtual void addHandler(const String &type, const String &execute);

        virtual void delHandlers();

    private:

        void fillData(m_uint32 key, const tintaUInt8Image * data);

        enum WinDataActions {
            ActionUpdate,
            ActionShow,            
            ActionAdd,
            ActionRecreate,
            ActionDelete,                        
            NoAction
        };

        struct ImgAction {

            WinDataActions mAction;
            m_uint32 mKey;

            void reset() {
                mAction = NoAction;
                mKey = ZERO_ID;
            }
        };

        void updateGLData();

        static String toStringKey( m_uint32 key );

        Graphica::imageLayer   *mLayer    = NULL_M;        

        tintaVector3f mParams = { 0.f, 0.f, 1.0f };
        
        struct WindowImgData {            
            m_uint32                     mKey;
            Graphica::tintaImage        *mImg;               
        };

        typedef std::list< WindowImgData> t_layers;
        typedef std::list< WindowImgData>::iterator t_layers_i;

        t_layers_i findImage( m_uint32 key );       

        bool removeImage(m_uint32 key);

        t_layers mImages;

        tintaCoord2Rec mPos;

        
        
        //objDatafVec_t   mLayers;

        //ImgAction mAction = { NoAction , ZERO_ID };

        typedef std::list<  ImgAction > actionqueue_t;

        actionqueue_t mActions;

        String mOnKey;

        String mOnMouseMove;

        MUTEX_T(mDataMutex);

    };
}

#endif

#endif