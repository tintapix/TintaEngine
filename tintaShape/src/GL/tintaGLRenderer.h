/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef TINTA_GL_RENDERER_H
#define TINTA_GL_RENDERER_H

#include "tintaGraphicaPredefine.h"
#include "tintaPlane.h"
#include <Math/tintaMatrix44.h>
#include "tintaRGBAColor.h"
#include "tintaFrameBuffer.h"
#include "tintaCamera.h"
#include "tintaAttributes.h"
#include "tintaSamplerBuffer.h"
#include "tintaImage.h"
#include "tintaGLVideoCap.h"
#include "tintaIWinContext.h"
#include "tintaITimerCallback.h"
#include "tintaGuiManager.h"
#include <tintaUInt8Image.h>

#define  TINTA_GL_LIBRARY "NVIDIA SDK"

namespace Graphica {
    

    class _CoreExport tintaGLRenderer :
        public tintaIWinContext,
        public tintaITimerCallback

{
public:
    // construction
    tintaGLRenderer(tintaFrameBuffer::FormatType eFormat,
        tintaFrameBuffer::DepthType eDepth, tintaFrameBuffer::StencilType eStencil,
        tintaFrameBuffer::BufferingType eBuffering,
        tintaFrameBuffer::MultisamplingType eMultisampling, int iWidth,
        int iHeight, const tintaRGBAColor &clearColor);

    virtual ~tintaGLRenderer();

	enum 
	{
		CT_RENDERER,
		CT_NUMERICAL,
		CT_USER
	};
    
protected:

	tintaFrameBuffer::FormatType        mFormat;
	tintaFrameBuffer::DepthType         mDepth;
	tintaFrameBuffer::StencilType       mStencil;
	tintaFrameBuffer::BufferingType     mBuffering;
	tintaFrameBuffer::MultisamplingType mMultisampling;

	tintaRGBAColor mClearColor;
    int            mWinWidth;
    int            mWinHeight;
	
    // actial aspect ration w/h or h/w
    tintaVector2f  mAspectRatio = { 1.f, 1.f };

	tintaCamera   *mCamera;	
	bool           mFullscreen;	
	tintaGeometry *mpGeometry;		
    float          mclearDepth;
    m_uint32       mClearStencil;

    bool m_bAllowRed, m_bAllowGreen, m_bAllowBlue, m_bAllowAlpha;

    void updateParallelMatrix();    
  

    

private:
    tintaMatrix44  mWorldMatrix;
    tintaMatrix44 *mViewMatrix = NULL_M;
    tintaMatrix44  mviewMatrixPos;
    tintaMatrix44  mviewMatrixCenter;

    tintaMatrix44  mProjMatrixPerspect;
    tintaMatrix44  mProjMatrixParallel;
    tintaMatrix44 *mProjMatrix = NULL_M;
    
public:

    void initImg(void);
    void display(void);

    void restoreAspectRatio();

    void setAspectRatio(const tintaVector2f &val);

	void setCamera ( tintaCamera* pkCamera );

	const tintaCamera *getCamera () const;

	void onFrustumChange ();
    
	void onFrameChange ();
    
	virtual void activate ();	

	virtual void resolution ();

	void setClearColor (const tintaRGBAColor& rkClearColor);

	const tintaRGBAColor& getClearColor () const;

    virtual void resizeView(int iWidth, int iHeight, bool fullScreen = false);
    

    tintaVector2i getWindowSize() const;
    // buffers
    const tintaVertexBuffer* enableVBuffer();
    void disableVBuffer(const tintaVertexBuffer* buff);
    void loadVBuffer(tintaVertexBuffer* buff);
    void releaseVBuffer(tintaVertexBuffer* buff);
    void setStateAlpha(bool bActive);
    void enableIBuffer (); 
    void disableIBuffer ();
    void loadIBuffer(tintaIndexBuffer* buff); 
    void releaseIBuffer(tintaIndexBuffer* buff);	

    void enableSBuffer( tintaSamplerBuffer* buff, int index, SBufferDraw type );
    void disableSBuffer( tintaSamplerBuffer* buff );
    void loadSBuffer( tintaSamplerBuffer* buff, int index, SBufferDraw type);
    bool releaseSBuffer( tintaSamplerBuffer* buff );

	// textures
    tintaTexture* findTexture(const String &name);
    void loadTexture( tintaTexture* texture );
    bool releaseTexture( tintaTexture* texture, bool videoOnly = false ); 
    bool releaseTexture( const String &name, bool videoOnly = false);
    void enableTexture( tintaTexture* texture, int index  = 0 );
    //bool enableTexture(const tintaTexture* texture) const ;
    void disableTexture( tintaTexture* texture );   

    void makeScreenShot( const String &prefix = _M("Shot"), const String &directory = _M("") );		
	
	const tintaGeometry* getGeometry () const;
	void setWorldTransformation ();	
	void updateCameras();
	
    void initialize ();

    const tintaMatrix44 &getProjMatrix() const ;

    const tintaMatrix44 &getViewMatrix() const;      

    const tintaMatrix44 &getWorldMatrix() const;

    void setGUIMode( bool perspective, bool movable );

    virtual void draw( tintaVisibleSet& set );

    virtual void draw( tintaGeometry* geom );	

    tintaGuiManager       *getFrontManager();  

    tintaGuiManager       *getBackManager();

    const tintaVector2f   &getAspectRatio() const;


    /*
        params {x_offset_GL_coordinates, y_offset_GL_coordinates, zoom }
    */
    virtual void drawImageLayer(  imageLayer *layer = nullptr, const tintaCoord2Rec *pos = nullptr, const tintaVector3f &params = { 0.f, 0.f, 1.f });
     

    virtual void drawPoints(const VecPoints_t &points, float size, PrimitiveType type, bool smooth = false);


    virtual void drawFrontLayer(tintaDrawable * layer, bool movableCam = false);

    virtual void drawFrontLayer( const String &name, bool movableCam = false);

    virtual void drawFrontLayers( bool movableCam = false);


    virtual void drawBackLayer(const String &name, bool movableCam = false);

    virtual void drawBackLayers( bool movableCam = false);

    virtual void drawBackLayer(tintaDrawable * layer, bool movableCam = false);

    virtual void drawImage( const Tinta::tintaUInt8Image &img, const tintaVector2f &pos, const tintaVector2f &zoom);

    virtual void drawImages(const Tinta::tintaImgStack *images, const tintaVector2f &zoom);

    virtual void initCenterCamera(const tintaCamera* cam );
    
    
public:

    void enableBuffers( bool depthTest );

    void clearBuffers( bool depthTest );
    
    virtual void setColorMask ( bool bAllowRed, bool bAllowGreen,
        bool bAllowBlue, bool bAllowAlpha );

    virtual void enableUserClipPlane ( int i, const tintaPlane& rkPlane );
    virtual void disableUserClipPlane ( int i );
    
    // tintaCamera updates
    friend class OpenGLtintaFrameBuffer;
    virtual void onViewportChange ();
    virtual void onDepthRangeChange ();   

    
    virtual tintaTexture * addTexture( const String &name);
    virtual tintaTexture * addTexture( const tintaImage * img, const String &nameTexture = _M(""));

    const tintaGLVideoCap &getGLVideoCap() const;
    
    m_uint32               getTimeElapsed()const;   	
	
	typedef std::vector<tintaSamplerInformation*> samplersvec_t;    	

    void setWareFrame( bool bWareFrame );    

    // from tintaITimerCallback
    virtual void onReset();
    virtual void onProcess( m_uint32 mlsElapsed );
    static int   printGLError();

    bool         glewInited() const;


    
    
private:
    // capabilities
    tintaGLVideoCap mCaps;    
    
    static GLenum GLGeomType[GT_MAX_QUANTITY];
    static GLenum GLTexInterp[MAX_FILTER_TYPES];    
    static GLenum GLImgInternalFormat[IMG_FORMAT_QUANTITY];
    static GLenum GLImgFormat[IMG_FORMAT_QUANTITY];    
    static GLenum GLImgTypes[IMG_FORMAT_QUANTITY];
    static GLenum SamplerTypes[MAX_SAMPLER_TYPES];
    static GLenum SBufferDataDraw[MAX_DRAW];

    
    void drawElements();

    bool mGlewInited;

    bool mWareFrame;

    m_uint32       mTimeElapsed;

    //tintaDrawable *mBack;

    NameGenerator  mShotNameGen;      

    Tinta::tintaIImgCodec *mScreenshot = NULL_M;

    tintaGuiManager       *mGuiFront = NULL_M;

    tintaGuiManager       *mBackground = NULL_M;

    typedef std::list< tintaTexture *> textureList;

    textureList mActiveTextures;

    void deleteTexture( const tintaTexture *val );

    void deleteTexture(const String &name);

    void deleteTextures();

    
        
};

}

#endif
