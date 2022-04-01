/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaGraphicaPrecompile.h"
#include "tintaGLVideoCap.h"
#include <tintaLogger.h>


namespace Graphica {

tintaGLVideoCap::tintaGLVideoCap(void):
miCap(CapiLast,-1),
mfCap( CapfLast,MAX_FLOAT ){    
}

tintaGLVideoCap::~tintaGLVideoCap(void){
}

Tinta::StringBasic tintaGLVideoCap::getVendor() const {
    char* chStr = (char*)glGetString(GL_VENDOR);
    // not init yet 
    assert( chStr );
    if( !chStr )
        return StringBasic();

    return StringBasic( chStr );
}   

Tinta::StringBasic tintaGLVideoCap::getRenderer() const {   

    char* chStr = (char*)glGetString(GL_RENDERER);
    // not init yet 
    assert( chStr );
        if( !chStr )
            return StringBasic();

    return StringBasic( chStr );
}

Tinta::StringBasic tintaGLVideoCap::getVerRenderer() const {   

//     OpenGL Version	GLSL Version
//         2.0	        1.10
//         2.1	        1.20
//         3.0	        1.30
//         3.1	        1.40
//         3.2	        1.50

    char* chStr = (char*)glGetString(GL_VERSION);
    // not init yet 
    assert( chStr );
        if( !chStr )
            return StringBasic();

    return StringBasic( chStr );
}

bool   tintaGLVideoCap::isExtSupports( const Tinta::StringBasic &ex )const {
    if ( GL_TRUE != glewGetExtension( ex.c_str() ) )
        return false;
    return true;
}

int tintaGLVideoCap::getIntCap(IntVideoCap c) const {

    int v (-1);
    GLint b(0);
    CoreAssert(miCap.size() > (size_t)c , "miCap.size() > (int)c" );
    if( miCap.at( (int)c ) != -1  ){
        return miCap.at( (int)c );
    }
    switch( c ) {
    case CapiMaxLights:
        glGetIntegerv(GL_MAX_LIGHTS,&v);
        break;
    case CapiMaxTCoords:
        glGetIntegerv(GL_MAX_TEXTURE_COORDS,&v);
        break;

    case CapiMaxVShaderImages:
        glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,&v);
        break;
    case  CapiMaxPShaderImages:
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,&v);
        break;    
    case CapiMaxStencilIndices:
        
        glGetIntegerv(GL_STENCIL_BITS,&b);
        v = (b > 0 ? (1 << b) : 0);
    case CapiMaxUserClipPlanes:
        glGetIntegerv(GL_MAX_CLIP_PLANES,&v);
        break;
     default:
         break;
    }
    return v;
}

float tintaGLVideoCap::getFloatCap(FloatVideoCap c)const {
    CoreAssert(mfCap.size() > (size_t)c, "mfCap.size() > (size_t)c" );

    if( mfCap.at( (int)c ) < MAX_FLOAT  ){
        return mfCap.at( (int)c );
    }
    
    float v( MAX_FLOAT );
    switch( c ) {
    case CapfMaxAnisotropy:
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT,&v);
        break;
    default:
        break;
    }
    return v;
}


void tintaGLVideoCap::getGlVersion(int *major, int *minor){
    const char *verstr = (const char *)glGetString(GL_VERSION);
    if ((verstr == NULL) || (sscanf(verstr, "%d.%d", major, minor) != 2))
    {
        *major = *minor = 0;
        fprintf(stderr, "Invalid GL_VERSION format!!!\n");
    }
}
void tintaGLVideoCap::getGlSLVersion(int *major, int *minor){

    int gl_major, gl_minor;
    getGlVersion(&gl_major, &gl_minor);
    *major = *minor = 0;
    if (gl_major == 1)
    {
        /* GL v1.x can only provide GLSL v1.00 as an extension */
        const char *extstr = (const char *)glGetString(GL_EXTENSIONS);
        if ((extstr != NULL) &&
            (strstr(extstr, "GL_ARB_shading_language_100") != NULL))
        {
            *major = 1;
            *minor = 0;
        }
    }
    else if (gl_major >= 2)
    {
        /* GL v2.0 and greater must parse the version string */
        const char *verstr =
            (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION);
        if ((verstr == NULL) ||
            (sscanf(verstr, "%d.%d", major, minor) != 2))
        {
            *major = *minor = 0;
            fprintf(stderr,
                "Invalid GL_SHADING_LANGUAGE_VERSION format!!!\n");
        }
    }
}

Tinta::String   tintaGLVideoCap::getExtensionsInfo() const {

     
         GLint exNum = 0; 
         glGetIntegerv(GL_NUM_EXTENSIONS, &exNum); 
 
         StringUtil::StrStreamType msg;

         for ( GLint i=0; i < exNum; i++ )  {
             const char* extension =  (const char*)glGetStringi(GL_EXTENSIONS, i);    
             if (extension)
                 msg << extension << _M("\n");
         }
         return msg.str();
     
}


}