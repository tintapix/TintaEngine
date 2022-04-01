/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaGraphicaPrecompile.h"
#include <tintaImgStack.h>
#include <ImageCodecs/tintaJpgImgCodec.h>
#include <ImageCodecs/tintaPngImgCodec.h>
#include <tintaMemoryAllocator.h>
#include "tintaGLRenderer.h"
#include "tintaVertexBuffer.h"
#include "tintaIndexBuffer.h"
#include "tintaVisibleSet.h"
#include "tintaAttributes.h"
#include "tintaGraphicConfig.h"
#include <tintaLogger.h>
#include "tintaGLSLProgram.h"
#include "tintaRoot.h"
#include "tintaSceneManager.h"

namespace Graphica {



GLenum tintaGLRenderer::GLGeomType[GT_MAX_QUANTITY] =
{
	GL_POINTS,          // GT_POLYPOINT
	GL_LINES,           // GT_POLYLINE_SEGMENTS
	GL_LINE_STRIP,      // GT_POLYLINE_OPEN
	GL_LINE_LOOP,       // GT_POLYLINE_CLOSED
	GL_TRIANGLES,       // GT_TRIMESH
	GL_TRIANGLE_STRIP,  // GT_TRISTRIP
	GL_TRIANGLE_FAN     // GT_TRIFAN
};

GLenum tintaGLRenderer::GLTexInterp[MAX_FILTER_TYPES] =
{
	GL_NEAREST,
	GL_LINEAR	
};


GLenum tintaGLRenderer::GLImgInternalFormat[IMG_FORMAT_QUANTITY] =
{
    COMPONENTS_3,
    COMPONENTS_4,
    COMPONENTS_1,
    COMPONENTS_1,
    COMPONENTS_1,
    COMPONENTS_1,
    COMPONENTS_1
};

GLenum tintaGLRenderer::GLImgTypes[IMG_FORMAT_QUANTITY] =
{
    GL_UNSIGNED_BYTE,
    GL_UNSIGNED_INT,
    GL_FLOAT,
    GL_UNSIGNED_INT_8_8_8_8
};

GLenum tintaGLRenderer::GLImgFormat[IMG_FORMAT_QUANTITY] =
{
    GL_RGB,
    GL_RGBA,
    GL_RED,
    GL_GREEN,
    GL_BLUE,
    GL_ALPHA,
    GL_LUMINANCE
};



GLenum tintaGLRenderer::SamplerTypes[MAX_SAMPLER_TYPES] =
{
	//GL_TEXTURE_1D,        // tintaSamplerInformation::SAMPLER_1D Deprecated
	GL_TEXTURE_2D,        // tintaSamplerInformation::SAMPLER_2D
	GL_TEXTURE_3D,        // tintaSamplerInformation::SAMPLER_3D	
};


GLenum tintaGLRenderer::SBufferDataDraw[MAX_DRAW] = {
    GL_STATIC_DRAW,
    GL_DYNAMIC_DRAW    
};

void tintaGLRenderer::restoreAspectRatio() {

    if ( mWinWidth <= 0 || mWinHeight <= 0 )
        return;
    float width = (float)mWinWidth;
    float height = (float)mWinHeight;
   
    if (width > height) 
        mAspectRatio = { width / height, 1.f };   
    else 
        mAspectRatio = { 1.f,height / width };

    onViewportChange();       
    onFrameChange();
}


void tintaGLRenderer::setAspectRatio(const tintaVector2f &val) {
    mAspectRatio = val;   
    onViewportChange();
}

tintaGLRenderer::tintaGLRenderer(tintaFrameBuffer::FormatType eFormat,
    tintaFrameBuffer::DepthType eDepth, tintaFrameBuffer::StencilType eStencil,
    tintaFrameBuffer::BufferingType eBuffering,
    tintaFrameBuffer::MultisamplingType eMultisampling, int iWidth,
    int iHeight, const tintaRGBAColor &clearColor)
    :mFormat(eFormat),
    mDepth(eDepth),
    mStencil(eStencil),
    mBuffering(eBuffering),
    mMultisampling(eMultisampling),
    mWinWidth(iWidth),
    mWinHeight(iHeight),
    mClearColor(clearColor),
    mClearStencil(0),  
    mCamera(NULL_M),              
    mclearDepth(0.0f),
    mFullscreen(false),
    mGlewInited(false),
    mTimeElapsed(0),    
    mShotNameGen( _M("Shot") ) { 

    restoreAspectRatio();

    mProjMatrix = &mProjMatrixPerspect;
    mViewMatrix = &mviewMatrixPos;
}


const tintaGLVideoCap &tintaGLRenderer::getGLVideoCap() const {
    return mCaps;
}

void tintaGLRenderer::initialize ()
{

    // glew initialisation
    GLenum err = glewInit();

    if ( GLEW_OK != err ){
        EXCEPTION("GLEW initialisation error " );			
        return;
    }
    mGlewInited = true;    
    glEnableClientState(GL_VERTEX_ARRAY);    
    glDisableClientState(GL_COLOR_ARRAY);
    glColor4fv( (const float*) tintaRGBAColor::WHITE);
    glClearColor(mClearColor[0], mClearColor[1],mClearColor[2],
        mClearColor[3]);

    
    glDisableClientState(GL_NORMAL_ARRAY);
    
    // hardware info
    StringUtil::StrStreamType msg; 		    
    msg << TINTA_GL_LIBRARY << _M("\n");
    msg << _M("Vendor: ") <<  mCaps.getVendor().c_str() << _M("\n");
    msg << _M("Renderer: ") <<  mCaps.getRenderer().c_str() << _M("\n");
    msg << _M("Version(GL): ") <<  mCaps.getVerRenderer().c_str() << _M("\n"); 
    int major = 0;
    int minor = 0;
    // dublicate removed
    //mCaps.getGlVersion(&major, &minor);
    //msg << _M("OpenGL version: ") << major << _M(".")<< minor  << _M("\n");
    mCaps.getGlSLVersion(&major, &minor);
    msg << _M("GLSL version: ") << major << _M(".") << minor << _M("\n");
    
    int maxSamplers = mCaps.getIntCap( tintaIVideoCap::CapiMaxPShaderImages );
    if ( maxSamplers > 0 ){        

    }
    msg << _M("Max samplers: ") << maxSamplers << _M("\n");    
  
    Tinta::tintaLogger::getPtr()->logMsg(msg.str());

    Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
    const tintaGraphicConfig *config = r ? r->getConfigurator() : NULL_M;
    
    if ( config ) {        

        if( config->getScreenShotType() == _M("jpg") )
            mScreenshot =  M_NEW tintaJpgImgCodec();
        else
            mScreenshot =  M_NEW tintaPngImgCodec();        
    }

    if ( mGuiFront )
        DELETE_T( mGuiFront, tintaGuiManager );

    mGuiFront = NEW_T(tintaGuiManager)(this);

    if (mBackground)
        DELETE_T(mBackground, tintaGuiManager);

    mBackground = NEW_T(tintaGuiManager)(this);

    

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,(const float*)tintaRGBAColor::BLACK);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_FALSE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR);
    glDisable(GL_LIGHTING);    
    glDisable(GL_LINE_STIPPLE);

    // glob def config
    glDisable(GL_BLEND);
    glEnable(GL_ALPHA_TEST);
 
    glDisable(GL_POLYGON_OFFSET_FILL);
    glDisable(GL_POLYGON_OFFSET_LINE);
    glDisable(GL_POLYGON_OFFSET_POINT);
    glPolygonOffset(0.f, 0.f);
    glDisable(GL_STENCIL_TEST);
    glPolygonMode(GL_BACK, GL_FILL);     

    // ----- 

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);        
}

const tintaGeometry* tintaGLRenderer::getGeometry () const
{
	return mpGeometry;
}

void tintaGLRenderer::setWorldTransformation ()
{
    mpGeometry->updateWorldData();
    mWorldMatrix = mpGeometry->getWorld(); // mHomWorld;
}

void tintaGLRenderer::enableIBuffer ()
{
    tintaIndexBuffer* pIBuffer = mpGeometry->getIBuffer();
	

    BufferID* pId = pIBuffer->GetId(); 

    if( !pId )
        loadIBuffer(pIBuffer);
    else    
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pId->ID);
}

void tintaGLRenderer::disableIBuffer ()
{    
    if ( mpGeometry ) 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);    

}


void tintaGLRenderer::enableSBuffer( tintaSamplerBuffer* buff, int index, SBufferDraw type) {
    
   
    if ( !buff->GetId() ) {        
        loadSBuffer(buff, index, type);
    }
    else {        
        printGLError();        

        int target = GL_TEXTURE_BUFFER_ARB;

        glActiveTextureARB(GL_TEXTURE0 + index);

        glBindTexture(target, buff->getTextureId() );        
        
        glBindBufferARB(target, buff->GetId()->ID);              

        if( buff->updated() )
            glBufferDataARB(target, buff->getDataSize(), buff->getData(), SBufferDataDraw[type] );

        printGLError();
       
    }
    
    
}

void tintaGLRenderer::disableSBuffer( tintaSamplerBuffer* buff ) {

}

void tintaGLRenderer::loadSBuffer( tintaSamplerBuffer* buff, int index, SBufferDraw type ) {

    BufferID*  id = buff->GetId();

    if( !id )
        id = buff->createID(); 
   
    glGenTextures( 1, &buff->getTextureId() );
    
    glActiveTextureARB( GL_TEXTURE0 + index );

    int target = GL_TEXTURE_BUFFER_ARB;

    glBindTexture(target, buff->getTextureId() );

    glGenBuffersARB( 1, (GLuint*)&id->ID ); 
   
    glBindBufferARB( target, id->ID);   
    //float arr[] = { 0.1f,0.1f, 0.1f, 0.1f,  0.2f, 0.1f, 0.1f, 0.1f };
    glBufferDataARB( target, buff->getDataSize(), buff->getData(), SBufferDataDraw[type] );

    //glBufferDataARB( target, sizeof(arr), arr, GL_DYNAMIC_DRAW );
   
    glTexBufferARB( target, GL_RGBA32F, id->ID );
    
    printGLError();
}

bool tintaGLRenderer::releaseSBuffer(tintaSamplerBuffer* buff) {

    if ( !buff )
        return false;

    BufferID* id = buff->GetId();

    if ( id ) {               
        glDeleteTextures((GLsizei)1, (GLuint*)&buff->getTextureId() );
        glDeleteBuffers((GLsizei)1, (GLuint*)&id->ID);
        printGLError();
        buff->releaseID();        
    }
    return true;
}

void tintaGLRenderer::draw ( tintaGeometry* entity ){   

    if ( !entity)
        return;

    const tintaMaterial *m = entity->getMaterial();

    if ( !m || !entity->isVisible() )
        return; // material is not added yet

	mpGeometry = entity;   

	setWorldTransformation();   

    const tintaGLSLProgram *p  = m->getProgram( 0 ); 
    bool rez = false;

    if ( p ) {

        const tintaVertexBuffer* buff = enableVBuffer();
        enableIBuffer();
        rez = mpGeometry->enableMaterial();
        if ( rez )
            drawElements();

        disableVBuffer(buff);
        rez = rez && mpGeometry->disableMaterial();

    }
    if( !rez ) {       

        StringStream msg;
        msg << _M("Shader programm error for material: ");
        msg << m->getName();
        if (Tinta::tintaLogger::getPtr())
            Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error);  

        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();

        tintaSceneManager *sm = r ? r->getSceneManager() : NULL_M;

        if ( sm )
            sm->removeEntity( mpGeometry->getName() );

        mpGeometry = NULL_M;   

        return;
    }
    

    disableIBuffer();  

    tintaBasicStringVector commands = mpGeometry->updateActions( mTimeElapsed );
    for (auto i : commands) {
        Graphica::tintaRoot* r = Graphica::tintaRoot::getPtr();
        tintaScriptContext* execute = r->getContext();
       
        execute->executeBuffer( i.c_str(), i.length() );

        size_t error_count(0);
        const String *errors = execute->getErrors(error_count);
        for (unsigned err = 0; err < error_count; err++, errors++) {

            if (Tinta::tintaLogger::getPtr())
                Tinta::tintaLogger::getPtr()->logMsg(*errors, msg_error);
            else
                stream_out << *errors;
        }

        execute->resetErrors();                   
    }   
}


tintaGuiManager   *tintaGLRenderer::getFrontManager() {
    return mGuiFront;
}

tintaGuiManager    *tintaGLRenderer::getBackManager() {
    return mBackground;
}


const tintaVector2f   &tintaGLRenderer::getAspectRatio() const {
    return mAspectRatio;
}

const tintaVertexBuffer* tintaGLRenderer::enableVBuffer() //, const tintaAttributes& rkOAttr)
{
	tintaVertexBuffer* pVBuff = mpGeometry->getVBuffer();
	
	BufferID* id = pVBuff->GetId();
    tintaAttributes& attrib = pVBuff->getAttributes();
    if (!id) 
        loadVBuffer(pVBuff);
    else {
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, id->ID);
        int arrCount = 0;
        if (attrib.hasPosition()) {
            glVertexAttribPointer(arrCount, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(attrib.getPositionOffset() * pVBuff->getVertexQuantity() * sizeof(float)));
            glEnableVertexAttribArray(arrCount++);
        }

        if (attrib.hasNormal())
        {
            //glDisableClientState(GL_NORMAL_ARRAY);
            //glDisableVertexAttribArray(arrCount++);
        }

        if (attrib.hasTCoord(0)) {
            glVertexAttribPointer(arrCount, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(attrib.getTCoordOffset(0) * pVBuff->getVertexQuantity() * sizeof(float)));// (GLvoid*)(12 * sizeof(float)));
            glEnableVertexAttribArray(arrCount++);
        }
    }
        //glBindVertexArray(id->ID);    

    return pVBuff;
}

void tintaGLRenderer::disableVBuffer(const tintaVertexBuffer* buff){

    CoreAssert(buff, "buff == NULL");

    if ( !buff )
        return;
    
    const tintaAttributes& attr = buff->getAttributes();
    
   
    int arrCount = 0;
    if ( attr.hasPosition() )
    {
        //glDisableClientState(GL_VERTEX_ARRAY);
        glDisableVertexAttribArray(arrCount++);
    }    

    if ( attr.hasNormal() )
    {
        //glDisableClientState(GL_NORMAL_ARRAY);
        glDisableVertexAttribArray(arrCount++);
    }       

    for (int unit = 0; unit < attr.getMaxTCoords(); unit++) {
        if ( attr.hasTCoord(unit) ){

            glClientActiveTexture( GL_TEXTURE0 + unit );
            //glDisableClientState( GL_TEXTURE_COORD_ARRAY );
            //glDisableVertexAttribArray(attribTexCoord);       // deactivate texture coords
            glDisableVertexAttribArray(arrCount++);
        }
    }   
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);
}


bool tintaGLRenderer::glewInited() const {
    return mGlewInited;
}

m_uint32 tintaGLRenderer::getTimeElapsed()const{
	return mTimeElapsed;
}

void tintaGLRenderer::setWareFrame(bool bWareFrame) {
    mWareFrame = bWareFrame;

    if ( mWareFrame ){
        glPolygonMode(GL_FRONT, GL_LINE);
    }
    else
        glPolygonMode(GL_FRONT, GL_FILL);
}



void tintaGLRenderer::onReset() {
    mTimeElapsed = 0;
}
void tintaGLRenderer::onProcess(m_uint32 mlsElapsed){
    //cout << mlsElapsed << "\n";
    mTimeElapsed = mlsElapsed;
}


int tintaGLRenderer::printGLError() {
    int    retCode(0);

#ifdef GL_DEBUG
    GLenum err = glGetError();
    GLenum errBefore = GL_NO_ERROR;

    while ( err != GL_NO_ERROR 
                        && err != errBefore ) {

        const GLubyte* sError = gluErrorString(err);

        StringStreamBasic msg_;
        if (sError)
            msg_ << "GLSL Error " << err << "(" << sError << ") " << " in file " << __FILE__ << " at line: " << __LINE__ << "\n";
        else
            msg_ << "GLSL Error " << err << " (no message)" << " in File " << __FILE__ << " at line: " << __LINE__ << "\n";

        retCode = 1;
        errBefore = err;
        err = glGetError();

        std::cout << msg_.str();

        if (Tinta::tintaLogger::getPtr())
            Tinta::tintaLogger::getPtr()->logMsg(msg_.str());

    }

#endif
    return retCode;
}



void tintaGLRenderer::draw ( tintaVisibleSet& set ){
    
    
   
    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_ALWAYS);
   // Draw( (Geometry*)mBack, true);
    //glDisable(GL_DEPTH_TEST);
   // glEnable(GL_DEPTH_TEST);
    
    const int q = set.GetQuantity();
	tintaVisibleObject* visible = set.GetVisible();

	for ( auto i = 0; i < q; i++ ){
		if (visible[i].Object ){
#if _DEBUG
            draw((tintaGeometry*)visible[i].Object);

#else
            draw((tintaGeometry*)visible[i].Object);
#endif      


		}
	}
    
}

void tintaGLRenderer::enableTexture( tintaTexture* texture, int index){

	CoreAssert( texture && texture->getImage(), _M( " texture " ) );


    if ( !texture->GetId() ) 
        loadTexture( texture );               
           
    CoreAssert(texture->GetId(), " texture->GetId() == NULL ");

    int target = GL_TEXTURE_2D; 
    
    glActiveTexture( GL_TEXTURE0 + index );      

    glBindTexture( target, texture->GetId()->mId );

    printGLError();
}

void tintaGLRenderer::disableTexture ( tintaTexture* pkTexture ){
}


#define checkImageWidth 1000
#define checkImageHeight 1000
GLubyte checkImage[checkImageHeight][checkImageWidth][4];

void makeCheckImage(void)
{
    int i, j, c;

    for (i = 0; i < checkImageHeight; i++) {
        for (j = 0; j < checkImageWidth; j++) {
            

            checkImage[i][j][0] = (GLubyte)11;
            checkImage[i][j][1] = (GLubyte)12;
            checkImage[i][j][2] = (GLubyte)255;
            checkImage[i][j][3] = (GLubyte)100;
        }
    }
}

void makeCheckImage2(void)
{
    int i, j, c;

    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            

            checkImage[i][j][0] = (GLubyte)255;
            checkImage[i][j][1] = (GLubyte)12;
            checkImage[i][j][2] = (GLubyte)255;
            checkImage[i][j][3] = (GLubyte)100;
        }
    }
}

void tintaGLRenderer::initImg(void)
{
    //glClearColor(0.0, 0.0, 0.0, 0.0);
    //glShadeModel(GL_FLAT);
    
    
}
void tintaGLRenderer::display(void)
{
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glRasterPos2f(0.f, 0.f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    makeCheckImage();

    glDrawPixels(checkImageWidth, checkImageHeight, GL_RGBA,
        GL_UNSIGNED_BYTE, checkImage);

    makeCheckImage2();

    glDrawPixels(checkImageWidth, checkImageHeight, GL_RGBA,
        GL_UNSIGNED_BYTE, checkImage);

    glFlush();

    glDisable(GL_BLEND);
}

void tintaGLRenderer::drawImage(const Tinta::tintaUInt8Image &img,  const tintaVector2f &pos, const tintaVector2f &zoom) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glRasterPos2f(pos.mx, pos.my);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPixelZoom(zoom.mx, -zoom.my);
    glDrawPixels(img.getWidth(), img.getHeight(), img.channels() == Tinta::ImgChannels_3 ? GL_RGB : GL_RGBA,
        GL_UNSIGNED_BYTE, img.getMemPtr() );

    glFlush();

    glDisable(GL_BLEND);
}

void tintaGLRenderer::drawImages(const Tinta::tintaImgStack *images, const tintaVector2f &zoom ) {

    CoreAssert(images, "images");

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    //glClear(GL_COLOR_BUFFER_BIT);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPixelZoom(zoom.mx, -zoom.my);

    m_uint32 index(0);
    const Tinta::tintaUInt8Image *img = images->getFirstImg(index);
    int i = 0;
    while ( img ) { 
              
        tintaVector2f p = images->getPosition(index);
        glRasterPos2f(p.mx, p.my);

        glDrawPixels(img->getWidth(), img->getHeight(), img->channels() == Tinta::ImgChannels_3 ? GL_RGB : GL_RGBA,
            GL_UNSIGNED_BYTE, img->getMemPtr());

        img = images->getNextImg(index);
        i++;
    }    

    glFlush();

    glDisable(GL_BLEND);
}

void tintaGLRenderer::loadTexture ( tintaTexture* texture )
{
	if ( !texture )	
		return;	
    
    TextureID* id = texture->GetId();

	if ( !id ){

        id = texture->createID();

        const tintaImage* image = texture->getImage();

        CoreAssert( image, "image" );

        m_uint32 iDimension = image->dimention(); // pkImage->GetDimension();
        iDimension = 0;

        //m_uint8* data = image->data();
       
        int internalFormat = GLImgInternalFormat[image->getFormat() ];
        int format = GLImgFormat[image->getFormat() ];
        int type = GLImgTypes[image->getType() ];
            
        int target = GL_TEXTURE_2D; // SamplerTypes[iDimension];
       
        glGenTextures( (GLsizei)1, &id->mId );
        printGLError();

        glBindTexture( target, id->mId );
        printGLError();

        glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
        glPixelStorei(GL_UNPACK_LSB_FIRST, GL_TRUE);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
        glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glTexImage2D( target, 0, internalFormat,
             image->width(), image->height(), 0,
            format,
            type,
            image->data() );
        printGLError();
        glTexParameteri( target, GL_TEXTURE_MIN_FILTER, GLTexInterp[texture->getFilterType()] );
        glTexParameteri( target, GL_TEXTURE_MAG_FILTER, GLTexInterp[texture->getFilterType()] );  
	}
}


bool tintaGLRenderer::releaseTexture(const String &name, bool videoOnly) {
    tintaTexture* texture = findTexture(name);
    return releaseTexture( texture, videoOnly);
}

bool tintaGLRenderer::releaseTexture(tintaTexture* pTexture, bool videoOnly){

	if ( !pTexture )
		return false;	


    TextureID* id = pTexture->GetId();
	if ( id ){
        bool bIsActive = IsActive();

        if ( !bIsActive ) 
        {
            SaveContext();
            activate();
        }        
        glDeleteTextures((GLsizei)1, (GLuint*)&id->mId);
        printGLError();
        pTexture->releaseID();       

        if ( !bIsActive ) {
            RestoreContext();
        }		
	}
       

    if ( !videoOnly ) {

        // from list
        deleteTexture( pTexture );         
    }
    return true;
}

void tintaGLRenderer::makeScreenShot( const String &prefix, const String &directory ){

    if ( !mScreenshot )
        return;
    const int channels = (int)Tinta::ImgChannels_3;
    UNIQPTRALLOC(m_uint8, data, ALLOC_T(m_uint8, channels * mWinWidth * mWinHeight));
    
    glReadBuffer(GL_BACK);

    glReadPixels( 0, 0, mWinWidth, mWinHeight, GL_RGB, GL_UNSIGNED_BYTE, data.get() );

    if ( mScreenshot->createImage( mWinWidth, mWinHeight,
                                    Tinta::ImgChannels_4, tintaIImgCodec::en32bits ) ) {

        const m_uint8 *ptr = data.get();

        m_uint8 vals[channels] = { 0, 0, 0 };
        size_t pos = 0;
        for (int h = 0; h < mWinHeight; h++) {
            for (int w = 0; w < mWinWidth; w++) {

                vals[0] = ptr[pos];
                vals[1] = ptr[pos + 1];
                vals[2] = ptr[pos + 2];
                vals[3] = 255;

                mScreenshot->setPixel(w, (mWinHeight - 1) - h, vals);
                pos+= channels;
            }
        }       

       
        StringStream msg;
        if( directory.length() == 0  )
            msg << getRootPath() << _M("//");      
        else {

            if ( !Tinta::isDirectory(directory) ) {
                Tinta::createFolder(directory);

                if (!Tinta::isPathValid(directory)) {

                    if (Tinta::tintaLogger::getPtr())
                        Tinta::tintaLogger::getPtr()->logMsg(_M("Make screenshot error. Can`t create or find directory"));

                    return;
                }
            }
            msg << directory << _M("//");
        }
         
        if ( mScreenshot->getType() == tintaIImgCodec::enPNGType )
            msg << mShotNameGen.generate(prefix) << _M(".png");
        else if ( mScreenshot->getType() == tintaIImgCodec::enJPGType )
            msg << mShotNameGen.generate(prefix) << _M(".jpg");
        else
            return;

        mScreenshot->saveImage(msg.str());
        
    }     
}

void tintaGLRenderer::loadVBuffer ( tintaVertexBuffer* vBuffer)
{
	if ( !vBuffer){
		return;
	}

    BufferID* id = vBuffer->GetId();
       
    if ( id )
        return;

    id = vBuffer->createID();

    glGenVertexArrays(1, &id->ID);
    glBindVertexArray(id->ID);

    glGenBuffersARB(1, &id->ID);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, id->ID);

    int sizeData;

    float* buff = 0;  

    vBuffer->packBuffer(sizeData, buff );

    tintaAttributes attr = vBuffer->getAttributes();
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeData * sizeof(float), buff, GL_STATIC_DRAW);
    int arrCount = 0;
    if ( attr.hasPosition() ) {        
        glVertexAttribPointer(arrCount, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(attr.getPositionOffset() * vBuffer->getVertexQuantity() * sizeof(float) ) );
        glEnableVertexAttribArray(arrCount++);
    }

    if (attr.hasNormal()) {
        glVertexAttribPointer(arrCount, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(attr.getNormalOffset() * vBuffer->getVertexQuantity() * sizeof(float)));
        glEnableVertexAttribArray(arrCount++);
    }

    if ( attr.hasTCoord( 0 ) ) {        
        glVertexAttribPointer(arrCount, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)( attr.getTCoordOffset( 0 ) * vBuffer->getVertexQuantity() * sizeof(float) ) );// (GLvoid*)(12 * sizeof(float)));
        glEnableVertexAttribArray(arrCount++);
    }

    FREE_T(buff);
        
}

void tintaGLRenderer::releaseVBuffer( tintaVertexBuffer* buff )
{
    if ( !buff )
		return;
	
    BufferID* id = buff->GetId();
    if ( id )	{                
        glDeleteBuffers(1, &id->ID);
        buff->releaseID();          
		return;
	}
}

void tintaGLRenderer::setStateAlpha(bool bActive) {

    GLboolean bIsAlphaEnables = glIsEnabled(GL_ALPHA_TEST);
    GLboolean bBlend = glIsEnabled(GL_BLEND);

    if ( bActive ) {

        if( !bIsAlphaEnables)
            glEnable(GL_ALPHA_TEST);
        if (!bBlend) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
    }
    else {
        if ( bIsAlphaEnables )
            glDisable(GL_ALPHA_TEST);
        if ( bBlend ) {
            glDisable(GL_BLEND);            
        }
    }
}

void tintaGLRenderer::loadIBuffer (tintaIndexBuffer* pIBuffer)
{
	if (!pIBuffer)
	{
		return;
	}
    BufferID* id = pIBuffer->GetId();
	if ( !id ){

        id = pIBuffer->createID();
        
        glGenBuffers(1, &id->ID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id->ID);        
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, pIBuffer->GetIndexQuantity()*sizeof(int),pIBuffer->GetData(), GL_STATIC_DRAW);        
	}
    //glBindVertexArray(0);
}

void tintaGLRenderer::releaseIBuffer(tintaIndexBuffer* buff)
{
    if (!buff)
	{
		return;
	}

    BufferID* id = buff->GetId();
	if ( id ){
        bool bIsActive = IsActive();
        if ( !bIsActive ) {
            SaveContext();
            activate();
        }

        glDeleteBuffers(1, &id->ID);

        buff->releaseID();

        if (!bIsActive){
            RestoreContext();
        }		
	}
}


void  tintaGLRenderer::initCenterCamera( const tintaCamera* cam ) {

    if ( !cam )
        return;
    

    const tintaVector3f &eye = cam->getPosition();
    tintaVector3f rVector = cam->getRVector();
    tintaVector3f uVector = cam->getUVector();
    tintaVector3f dVector = cam->getDVector();

    mviewMatrixCenter[0][0] = rVector[0];
    mviewMatrixCenter[0][1] = uVector[0];
    mviewMatrixCenter[0][2] = dVector[0];
    mviewMatrixCenter[0][3] = 0.0f;
    mviewMatrixCenter[1][0] = rVector[1];
    mviewMatrixCenter[1][1] = uVector[1];
    mviewMatrixCenter[1][2] = dVector[1];
    mviewMatrixCenter[1][3] = 0.0f;
    mviewMatrixCenter[2][0] = rVector[2];
    mviewMatrixCenter[2][1] = uVector[2];
    mviewMatrixCenter[2][2] = dVector[2];
    mviewMatrixCenter[2][3] = 0.0f;
    mviewMatrixCenter[3][0] = -rVector.dot(eye);
    mviewMatrixCenter[3][1] = -uVector.dot(eye);
    mviewMatrixCenter[3][2] = -dVector.dot(eye);
    mviewMatrixCenter[3][3] = 1.0f;
       

    updateParallelMatrix();

}
void tintaGLRenderer::setCamera (tintaCamera* cam) {

	if ( mCamera ) {
        mCamera->m_pRenderer = 0;
	}

	if (cam)
        cam->m_pRenderer = this;

    mCamera = cam;

	if ( mCamera )	{
 		onFrustumChange();
 		onViewportChange();
 		onFrameChange();
	}
}




// MaterialState* tintaGLRenderer::GetMaterialState ()
// {
// 	return (MaterialState*)(m_aspkState[GlobalState::MATERIAL]);
// }
// void tintaGLRenderer::SetLight (int i, Light* pkLight)
// {
// 	CoreAssert(0 <= i && i < ms_iMaxLights,"0 <= i && i < ms_iMaxLights");
// 	// impl mev
// 	m_aspkLight[i] = pkLight[0];
// }

const tintaCamera* tintaGLRenderer::getCamera () const {
	return mCamera;
}

// void tintaGLRenderer::SetCullState (CullState* pkState)
// {
// 	m_aspkState[GlobalState::CULL] = pkState;
// 
// 
// 	// GL 
// 	if (pkState->Enabled)
// 	{
// 		glEnable(GL_CULL_FACE);
// 	}
// 	else
// 	{
// 		glDisable(GL_CULL_FACE);
// 	}
// 
// 	glFrontFace(ms_aeFrontFace[pkState->FrontFace]);
// 
// 	if (!m_bReverseCullFace)
// 	{
// 		glCullFace(ms_aeCullFace[pkState->CullFace]);
// 	}
// 	else
// 	{
// 		if (ms_aeCullFace[pkState->CullFace] == GL_BACK)
// 		{
// 			glCullFace(GL_FRONT);
// 		}
// 		else
// 		{
// 			glCullFace(GL_BACK);
// 		}
// 	}
// }

//  void tintaGLRenderer::SetPolygonOffsetState (PolygonOffsetState* pkState) {
// // 	GlobalStatePtr loc_ptr(pkState, MFM_DELETE);
// // 	m_aspkState[GlobalState::POLYGONOFFSET].setNull();
// 	m_aspkState[GlobalState::POLYGONOFFSET] = pkState;
// 	
// 
// 	// GL
// 
// 	if (pkState->FillEnabled)
// 	{
// 		glEnable(GL_POLYGON_OFFSET_FILL);
// 	}
// 	else
// 	{
// 		glDisable(GL_POLYGON_OFFSET_FILL);
// 	}
// 
// 	if (pkState->LineEnabled)
// 	{
// 		glEnable(GL_POLYGON_OFFSET_LINE);
// 	}
// 	else
// 	{
// 		glDisable(GL_POLYGON_OFFSET_LINE);
// 	}
// 
// 	if (pkState->PointEnabled)
// 	{
// 		glEnable(GL_POLYGON_OFFSET_POINT);
// 	}
// 	else
// 	{
// 		glDisable(GL_POLYGON_OFFSET_POINT);
// 	}
// 
// 	glPolygonOffset(pkState->Scale,pkState->Bias);
//  }

 void tintaGLRenderer::onFrameChange ()
 { 
     if (mCamera) {

         tintaVector3f eye = mCamera->getPosition();
         tintaVector3f rVector = mCamera->getRVector();
         tintaVector3f uVector = mCamera->getUVector();
         tintaVector3f dVector = mCamera->getDVector();

         mviewMatrixPos[0][0] = rVector[0];
         mviewMatrixPos[0][1] = uVector[0];
         mviewMatrixPos[0][2] = dVector[0];
         mviewMatrixPos[0][3] = 0.0f;
         mviewMatrixPos[1][0] = rVector[1];
         mviewMatrixPos[1][1] = uVector[1];
         mviewMatrixPos[1][2] = dVector[1];
         mviewMatrixPos[1][3] = 0.0f;
         mviewMatrixPos[2][0] = rVector[2];
         mviewMatrixPos[2][1] = uVector[2];
         mviewMatrixPos[2][2] = dVector[2];
         mviewMatrixPos[2][3] = 0.0f;
         mviewMatrixPos[3][0] = -rVector.dot(eye);
         mviewMatrixPos[3][1] = -uVector.dot(eye);
         mviewMatrixPos[3][2] = -dVector.dot(eye);
         mviewMatrixPos[3][3] = 1.0f; 

         //initCenterCamera(mCamera);
     }

     
#if 0
     system("cls");
     std::cout<<mViewMatrix[0][0]<<"\n";
     std::cout << mViewMatrix[0][1] << "\n";
     std::cout << mViewMatrix[0][2] << "\n";
     std::cout << mViewMatrix[0][3] << "\n";
     std::cout << mViewMatrix[1][0] << "\n";
     std::cout << mViewMatrix[1][1] << "\n";
     std::cout << mViewMatrix[1][2] << "\n";
     std::cout << mViewMatrix[1][3] << "\n";
     std::cout << mViewMatrix[2][0] << "\n";
     std::cout << mViewMatrix[2][1] << "\n";
     std::cout << mViewMatrix[2][2] << "\n";
     std::cout << mViewMatrix[2][3] << "\n";
     std::cout << mViewMatrix[3][0] << "\n";
     std::cout << mViewMatrix[3][1] << "\n";
     std::cout << mViewMatrix[3][2] << "\n";
     std::cout << mViewMatrix[3][3] << "\n";

#endif
 }
 const tintaMatrix44 &tintaGLRenderer::getProjMatrix() const {
     return *mProjMatrix;
 }

 const tintaMatrix44 &tintaGLRenderer::getViewMatrix() const {
     return *mViewMatrix;
 }

 const tintaMatrix44 &tintaGLRenderer::getWorldMatrix() const {
     return mWorldMatrix;
 }

 void tintaGLRenderer::setGUIMode(bool perspective, bool movable) {
     if ( perspective ) 
         mProjMatrix = &mProjMatrixPerspect;
     else
         mProjMatrix = &mProjMatrixParallel;
     
     if ( movable )
         mViewMatrix = &mviewMatrixPos;         
     else
         mViewMatrix = &mviewMatrixCenter;
 }


 void tintaGLRenderer::updateParallelMatrix() {

     if (!mCamera)
         return;

     float rMin = 0.f, rMax = 0.f, upMin = 0.f, upMax = 0.f, downMin = 0.f, downMax = 0.f;
     mCamera->getFrustum(rMin, rMax, upMin, upMax, downMin, downMax);

     float rDiff = 1.0f / (rMax - rMin);
     float uDiff = 1.0f / (upMax - upMin);
     float dDiff = 1.0f / (downMax - downMin);

     mProjMatrixParallel[0][0] = 2.0f*rDiff;
     mProjMatrixParallel[0][1] = 0.0f;
     mProjMatrixParallel[0][2] = 0.0f;
     mProjMatrixParallel[0][3] = 0.0f;
     mProjMatrixParallel[1][0] = 0.0f;
     mProjMatrixParallel[1][1] = 2.0f*uDiff;
     mProjMatrixParallel[1][2] = 0.0f;
     mProjMatrixParallel[1][3] = 0.0f;
     mProjMatrixParallel[2][0] = 0.0f;
     mProjMatrixParallel[2][1] = 0.0f;
     mProjMatrixParallel[2][2] = dDiff;
     mProjMatrixParallel[2][3] = 0.0f;
     mProjMatrixParallel[3][0] = -(rMin + rMax) * rDiff;
     mProjMatrixParallel[3][1] = -(upMin + upMax) * uDiff;
     mProjMatrixParallel[3][2] = -dDiff * rDiff;
     mProjMatrixParallel[3][3] = 1.0f;
 }

 void tintaGLRenderer::onFrustumChange () {

	 if ( !mCamera)
		 return;	 
     
         float rMin = 0.f, rMax = 0.f, upMin = 0.f, upMax = 0.f, downMin = 0.f, downMax = 0.f;
         mCamera->getFrustum( rMin, rMax, upMin, upMax, downMin, downMax );

         float fInvRDiff = 1.0f / (rMax - rMin);
         float fInvUDiff = 1.0f / (upMax - upMin);
         float fInvDDiff = 1.0f / (downMax - downMin);

         mProjMatrixPerspect[0][0] = 2.0f*downMin*fInvRDiff;
         mProjMatrixPerspect[0][1] = 0.0f;
         mProjMatrixPerspect[0][2] = 0.0f;
         mProjMatrixPerspect[0][3] = 0.0f;
         mProjMatrixPerspect[1][0] = 0.0f;
         mProjMatrixPerspect[1][1] = 2.0f*downMin*fInvUDiff;
         mProjMatrixPerspect[1][2] = 0.0f;
         mProjMatrixPerspect[1][3] = 0.0f;
         mProjMatrixPerspect[2][0] = -(rMin + rMax)*fInvRDiff;
         mProjMatrixPerspect[2][1] = -(upMin + upMax)*fInvUDiff;
         mProjMatrixPerspect[2][2] = downMax*fInvDDiff;
         mProjMatrixPerspect[2][3] = 1.0f;
         mProjMatrixPerspect[3][0] = 0.0f;
         mProjMatrixPerspect[3][1] = 0.0f;
         mProjMatrixPerspect[3][2] = -downMax*downMin*fInvDDiff;
         mProjMatrixPerspect[3][3] = 0.0f;
     
     updateParallelMatrix();

     initCenterCamera(mCamera);
 }
 void tintaGLRenderer::activate() {

	 onViewportChange();
	 onFrustumChange();
	 if (mCamera)
	 {
		 onFrameChange ();
	 }

 }
 
 void tintaGLRenderer::resolution (){
	mFullscreen =! mFullscreen;
 }

 void tintaGLRenderer::setClearColor (const tintaRGBAColor& rkClearColor) {
     mClearColor = rkClearColor;
 }
 const tintaRGBAColor& tintaGLRenderer::getClearColor () const {
	return mClearColor;
 }


 tintaVector2i tintaGLRenderer::getWindowSize() const {
     return{ mWinWidth, mWinHeight };
 }

 void tintaGLRenderer::resizeView(int iWidth, int iHeight, bool fullScreen)
 {
     mFullscreen = fullScreen;
     
     mWinWidth = iWidth;
     mWinHeight = iHeight;             
     restoreAspectRatio();
 }



tintaGLRenderer::~tintaGLRenderer(){
    if ( mGuiFront ) 
        DELETE_T(mGuiFront, tintaGuiManager);    

    if (mBackground)
        DELETE_T(mBackground, tintaGuiManager);

    if (mScreenshot)
        M_DELETE mScreenshot;
     /*   

    if (config) {

        if (config->getScreenShotType() == _M("jpg"))
            mScreenshot = M_NEW tintaJpgImgCodec();
        else
            mScreenshot = M_NEW tintaPngImgCodec();
    }

    if (mGuiFront)
        DELETE_T(mGuiFront, tintaGuiManager);

    mGuiFront = NEW_T(tintaGuiManager)(this);

    if (mBackground)
        DELETE_T(mBackground, tintaGuiManager);

    mBackground = NEW_T(tintaGuiManager)(this);


    */

    deleteTextures();
}


void tintaGLRenderer::drawElements ()
{

 	tintaIndexBuffer* buffer = mpGeometry->getIBuffer();

	CoreAssert(buffer,"buffer");

    GLenum eType = GLGeomType[mpGeometry->getGeomType()];

 	int iOffset = buffer->GetOffset();

    //glDrawArrays(GL_TRIANGLES, 0, buffer->GetIndexQuantity());

    glDrawElements( eType, buffer->GetIndexQuantity(), GL_UNSIGNED_INT, (const GLvoid*)((int*)0 + iOffset) );

}


void tintaGLRenderer::onViewportChange () {

	float fPortL, fPortR, fPortT, fPortB;
	if ( mCamera )	
         mCamera->getViewport( fPortL, fPortR, fPortT, fPortB );
	else {	
		fPortL = 0.0f;
		fPortR = 1.0f;
		fPortT = 1.0f;
		fPortB = 0.0f;
	}   
	
  //  tintaApp*  app = tintaApp::mWinApp;

	GLint iX = (GLint)( fPortL * mWinWidth );
	GLint iY = (GLint)( fPortB * mWinHeight );  // See note above.
	GLsizei iW = (GLsizei)( ( fPortR - fPortL) * mWinWidth );
	GLsizei iH = (GLsizei)( ( fPortT - fPortB) * mWinHeight );

	glViewport(iX, iY, iW, iH);
}

void tintaGLRenderer::enableBuffers( bool depthTest ) {
    if ( depthTest ) {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glClearDepth(1.0f);
        glDepthMask(GL_TRUE);
        glClear(GL_DEPTH_BUFFER_BIT);       
        
    }
    else {
        glDisable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glClearDepth(0.0f);
        glDepthMask(GL_FALSE);
        glClear(GL_DEPTH_BUFFER_BIT);         
        //glDisable(GL_STENCIL_TEST);
    }
}
void tintaGLRenderer::clearBuffers (bool depthTest)
{
	glClearColor(mClearColor[0],mClearColor[1],mClearColor[2],
		mClearColor[3]);    

	glClearDepth((double)mclearDepth);
	glClearStencil((GLint)mClearStencil);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);


    enableBuffers(depthTest);
}


void tintaGLRenderer::setColorMask (bool bAllowRed, bool bAllowGreen,
								   bool bAllowBlue, bool bAllowAlpha)
{
	// impl  mev Renderer::setColorMask(bAllowRed,bAllowGreen,bAllowBlue,bAllowAlpha);

	glColorMask((GLboolean)bAllowRed,(GLboolean)bAllowGreen,
		(GLboolean)bAllowBlue,(GLboolean)bAllowAlpha);
}

void tintaGLRenderer::enableUserClipPlane (int i, const tintaPlane & plane)
{
	GLdouble adPlane[4] =
	{
        (double)plane.mNormal.mx,
        (double)plane.mNormal.my,
        (double)plane.mNormal.mz,
        (double)-plane.mMag
	};
	glClipPlane(GL_CLIP_PLANE0 + i,adPlane);
	glEnable(GL_CLIP_PLANE0 + i);
}

void tintaGLRenderer::disableUserClipPlane (int i)
{
	glDisable(GL_CLIP_PLANE0 + i);
}

void tintaGLRenderer::onDepthRangeChange () {
	if ( mCamera ) {
		float fPortN, fPortF;
        mCamera->getDepthRange( fPortN, fPortF );
		glDepthRange( (GLclampd)fPortN,(GLclampd)fPortF );
	}
	else {
		glDepthRange(0.0,1.0);
	}
}


tintaTexture * tintaGLRenderer::addTexture(const String &name) {

    tintaTexture * tex = findTexture( name );
    
    if ( !tex ) {
        const tintaImage * img = Graphica::tintaRoot::getPtr()->getImage( name );
        tex = addTexture( img );
    }
    return tex;
}


tintaTexture * tintaGLRenderer::addTexture( const tintaImage * img , const String &nameTexture ) {
      
    tintaTexture *texture = NULL_M;
    if ( img ) {
        String name = nameTexture.length() > 0 ? nameTexture : img->getName();

        auto l = std::find_if(mActiveTextures.begin(), mActiveTextures.end(), [&](auto *val) {return val->getName() == name; });
        if (l == mActiveTextures.end()) {
            texture = M_NEW tintaTexture(NEAREST, img, nameTexture);

            mActiveTextures.push_back(texture);
        }
        else
            texture = (*l);
    }
    return texture;
}


tintaTexture* tintaGLRenderer::findTexture( const String &name ) {

    tintaTexture* rez = NULL_M;

    auto v = mActiveTextures.begin();

    for ( ; v != mActiveTextures.end(); v++ ) {
        if ( name == (*v)->getName() ) {
            
            // move forward
           // std::swap( (*v), (*mActiveTextures.begin()) );
            rez =  *v;
            break;
        }
    }
    return rez;
    
}

void tintaGLRenderer::drawImageLayer( imageLayer *layer, const tintaCoord2Rec *pos, const tintaVector3f &params ) {

   // if ( mImageLayers.size() == 0 )
   //     return;
    
    GLboolean bIsAlphaEnables = glIsEnabled(GL_ALPHA_TEST);
    GLboolean bBlend = glIsEnabled(GL_BLEND);    
    GLboolean bTexture2D = glIsEnabled(GL_TEXTURE_2D);

   if( !bIsAlphaEnables )
    glEnable( GL_ALPHA_TEST);

   if ( !bBlend ) {
       glEnable(GL_BLEND);
       glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   }

   if( !bTexture2D )
     glEnable(GL_TEXTURE_2D);
   
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
   glLoadIdentity();
   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();

   if ( layer ) {

    tintaTexture* tex = layer->getFrontTexture(); // .front();

    if ( !tex ) // no textures ok
        return; 

    enableTexture(tex);

    glBegin(GL_TRIANGLES);

    /*
    glTexCoord2f(0, 1); glVertex3f(-1.0f, -1.0f, 0.0);
    glTexCoord2f(1, 0); glVertex3f(1.0f, 1.0f, 0.0);
    glTexCoord2f(0, 0); glVertex3f(-1.0f, 1.0f, 0.0);

    glTexCoord2f(0, 1); glVertex3f(-1.0f, -1.0f, 0.0);
    glTexCoord2f(1, 1); glVertex3f(1.0f, -1.0f, 0.0);
    glTexCoord2f(1, 0); glVertex3f(1.0f, 1.0f, 0.0);

    */
    
        if ( !pos ) {

            glTexCoord2f(0, 1); glVertex3f((layer->getPos().mLB.mx - params.mx)* params.mz,
                (layer->getPos().mLB.my + params.my)* params.mz, 0.0f);

            glTexCoord2f(1, 0); glVertex3f((layer->getPos().mRU.mx - params.mx)* params.mz,
                (layer->getPos().mRU.my + params.my)* params.mz, 0.0f);

            glTexCoord2f(0, 0); glVertex3f((layer->getPos().mLU.mx - params.mx)* params.mz,
                (layer->getPos().mLU.my + params.my)* params.mz, 0.0f);


            glTexCoord2f(0, 1); glVertex3f((layer->getPos().mLB.mx - params.mx)* params.mz,
                (layer->getPos().mLB.my + params.my)* params.mz, 0.0f);

            glTexCoord2f(1, 1); glVertex3f((layer->getPos().mRB.mx - params.mx)* params.mz,
                (layer->getPos().mRB.my + params.my)* params.mz, 0.0f);

            glTexCoord2f(1, 0); glVertex3f((layer->getPos().mRU.mx - params.mx)* params.mz,
                (layer->getPos().mRU.my + params.my)* params.mz, 0.0f);

        }
        else {

            glTexCoord2f(0, 1); glVertex3f((pos->mLB.mx - params.mx)* params.mz,
                (pos->mLB.my + params.my)* params.mz, 0.0f);

            glTexCoord2f(1, 0); glVertex3f((pos->mRU.mx - params.mx)* params.mz,
                (pos->mRU.my + params.my)* params.mz, 0.0f);

            glTexCoord2f(0, 0); glVertex3f((pos->mLU.mx - params.mx)* params.mz,
                (pos->mLU.my + params.my)* params.mz, 0.0f);


            glTexCoord2f(0, 1); glVertex3f((pos->mLB.mx - params.mx)* params.mz,
                (pos->mLB.my + params.my)* params.mz, 0.0f);

            glTexCoord2f(1, 1); glVertex3f((pos->mRB.mx - params.mx)* params.mz,
                (pos->mRB.my + params.my)* params.mz, 0.0f);

            glTexCoord2f(1, 0); glVertex3f((pos->mRU.mx - params.mx)* params.mz,
                (pos->mRU.my + params.my)* params.mz, 0.0f);
        }

        glEnd();
    }
    else {      
        imageLayer* l  =  mGuiFront->getFirstImageLayer();
        while ( l ) {

            tintaTexture* t =  l->getFrontTexture();
            if ( !t )
                continue;

            enableTexture( t );

            glBegin(GL_TRIANGLES);

            glTexCoord2f(0, 1); glVertex3f((l->getPos().mLB.mx - params.mx)* params.mz,
                (l->getPos().mLB.my + params.my)* params.mz, 0.0f);

            glTexCoord2f(1, 0); glVertex3f((l->getPos().mRU.mx - params.mx)* params.mz,
                (l->getPos().mRU.my + params.my)* params.mz, 0.0f);

            glTexCoord2f(0, 0); glVertex3f((l->getPos().mLU.mx - params.mx)* params.mz,
                (l->getPos().mLU.my + params.my)* params.mz, 0.0f);


            glTexCoord2f(0, 1); glVertex3f((l->getPos().mLB.mx - params.mx)* params.mz,
                (l->getPos().mLB.my + params.my)* params.mz, 0.0f);

            glTexCoord2f(1, 1); glVertex3f((l->getPos().mRB.mx - params.mx)* params.mz,
                (l->getPos().mRB.my + params.my)* params.mz, 0.0f);

            glTexCoord2f(1, 0); glVertex3f((l->getPos().mRU.mx - params.mx)* params.mz,
                (l->getPos().mRU.my + params.my)* params.mz, 0.0f);
            glEnd();

            l = mGuiFront->getNextImageLayer();
        }        
    }  

    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();


    glBindTexture(GL_TEXTURE_2D, 0);

    if (!bIsAlphaEnables)
        glDisable(GL_ALPHA_TEST);
    if (!bBlend) {
        glDisable(GL_BLEND);        
    }
    if ( !bTexture2D )
        glDisable(GL_TEXTURE_2D);      

    //glColor3f(1.0f, 1.0f, 1.0f);
}


void tintaGLRenderer::drawPoints(const VecPoints_t &points, float size, PrimitiveType type, bool smooth){

       
    glEnable(GL_POINT_SMOOTH);   

    glEnable(GL_POINT_SPRITE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    

    if ( type == Graphica::PRIMITIVE_POINT ) {  

        glPointSize(size);
        glBegin(GL_POINTS);
        
        for (auto p : points) {
            glColor4f(p.mColor.mx, p.mColor.my, p.mColor.mz, p.mColor.mw);
            glVertex2f(p.mPoint.mx / mAspectRatio.mx, p.mPoint.my / mAspectRatio.my);
        }

        glEnd();            

    }
    else if ( type == Graphica::PRIMITIVE_LINE ) {

        glLineWidth(size);
        //glColor3f( 1.0, 0.0, 0.0 );
        glBegin(GL_LINES);

        for ( auto p : points ) {
            glColor4f(p.mColor.mx, p.mColor.my, p.mColor.mz, p.mColor.mw);
            glVertex2f(p.mPoint.mx, p.mPoint.my );
        }

        glEnd();
    }
    else if (type == Graphica::PRIMITIVE_LINE_STRIP) {
        glLineWidth(size);
        //glColor3f( 1.0, 0.0, 0.0 );      

        glBegin(GL_LINE_STRIP);

        for (auto p : points) {
            glColor4f(p.mColor.mx, p.mColor.my, p.mColor.mz, p.mColor.mw);
            glVertex2f(p.mPoint.mx, p.mPoint.my);
        }

        glEnd();
    }
    else if ( type == Graphica::PRIMITIVE_POLY ) {        
        
        glShadeModel(GL_SMOOTH);                      
        glBegin(GL_POLYGON);

        for ( auto p : points ) {
            glColor4f(p.mColor.mx, p.mColor.my, p.mColor.mz, p.mColor.mw);
            glVertex3f(p.mPoint.mx, p.mPoint.my, 0.f);
        }

        /*glColor3f(1.0, 0.0, 0.0);   // red
        glVertex3f(0.0, 0.0, 0.0);
        glColor3f(0.0, 1.0, 0.0);   // green
        glVertex3f(1.0, 0.0, 0.0);
        glColor3f(0.0, 0.0, 1.0);   // blue
        glVertex3f(1.0, 1.0, 0.0);
        */    

        glEnd();
        
    }
    glDisable(GL_BLEND);
    glDisable(GL_POINT_SPRITE);

   
   
   
        
}



void tintaGLRenderer::drawFrontLayer(const String &name, bool movableCam) {

    setGUIMode(false, movableCam);
    tintaDrawable * layer = mGuiFront->getEffectLayer(name);

    if ( layer ) {
        draw( (tintaGeometry*)layer );    
    }
    setGUIMode(true, true);
}

void tintaGLRenderer::drawFrontLayer(tintaDrawable * layer, bool movableCam) {
    
   
    if ( !layer )
        return;

    setGUIMode(false, movableCam);

    draw((tintaGeometry*)layer);
    
    setGUIMode(true, true);
}


void tintaGLRenderer::drawFrontLayers(bool movableCam) {

    tintaDrawable * layer = mGuiFront->getFirstEffectLayer();

    if (!layer)
        return;

    setGUIMode(false, movableCam);

    while (layer) {
        draw((tintaGeometry*)layer);
        layer = mGuiFront->getNextEffectLayer();
    }
    setGUIMode(true, true);
}

void tintaGLRenderer::drawBackLayer(const String &name, bool movableCam) {

    setGUIMode(false, movableCam);
    tintaDrawable * layer = mBackground->getEffectLayer(name);

    if (layer) {
        draw((tintaGeometry*)layer);
    }
    setGUIMode(true, true);
}

void tintaGLRenderer::drawBackLayers(bool movableCam) {

    tintaDrawable * layer = mBackground->getFirstEffectLayer();
    if ( !layer )
        return;

    setGUIMode(false, movableCam);

    while (layer) {
        draw((tintaGeometry*)layer);
        layer = mBackground->getNextEffectLayer();
    }
    setGUIMode(true, true);
}

void tintaGLRenderer::drawBackLayer(tintaDrawable * layer, bool movableCam ) {

    setGUIMode(false, movableCam);

    if (layer) {
        draw((tintaGeometry*)layer);        
    }
    setGUIMode(true, true);
}

void tintaGLRenderer::updateCameras() {
    mCamera->updateActions( mTimeElapsed );
}

void tintaGLRenderer::deleteTexture( const tintaTexture *val ) {

    if ( !val )
        return;

    String name = val->getName();

    // from materials fill with dummy
    tintaRoot* root = Graphica::tintaRoot::getPtr();
    if ( root ) {
        root->delTexFromMaterials(name);
    }

    auto  it = mActiveTextures.begin();
   
    for (; it != mActiveTextures.end();  ) {
        if ( (*it) == val  || !val ) {
            M_DELETE(*it);
            it = mActiveTextures.erase(it);
        }
        else
            it++;
    }

   
}

void tintaGLRenderer::deleteTexture( const String &name) {
    auto  it = mActiveTextures.begin();

    for (; it != mActiveTextures.end();) {
        if( (*it)->getName() == name ){            
            M_DELETE(*it);
           it =  mActiveTextures.erase(it);
        }
        else
            it++;
    }
}

void tintaGLRenderer::deleteTextures() {
    deleteTexture(NULL_M);
}

}