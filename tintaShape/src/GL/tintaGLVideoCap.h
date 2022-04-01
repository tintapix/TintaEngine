/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_GL_VIDEO_CAP_H_
#define _TINTA_GL_VIDEO_CAP_H_

#include <tintaPredefine.h>
#include "tintaIVideoCap.h"
#include "tintaGLPredefine.h"


namespace Graphica {

class _CoreExport tintaGLVideoCap:
        public tintaIVideoCap
{

public:
             tintaGLVideoCap(void);

    virtual ~tintaGLVideoCap(void);

    virtual Tinta::StringBasic getVendor() const;

    virtual Tinta::StringBasic getRenderer() const;

    virtual Tinta::StringBasic getVerRenderer() const;

    virtual bool   isExtSupports( const Tinta::StringBasic &ex )const ;

    virtual Tinta::String getExtensionsInfo()const;

    virtual void getGlVersion(int *major, int *minor);

    virtual void getGlSLVersion(int *major, int *minor);

    virtual int   getIntCap(IntVideoCap c)const;

    virtual float getFloatCap(FloatVideoCap c)const;

private: 

    typedef std::vector<int> IntCapVec;
    typedef std::vector<float> FloatCapVec;
    IntCapVec   miCap;
    FloatCapVec mfCap;
    
//     int iMaxLights;
//     int iMaxColors;
//     int iMaxTCoords;
//     int ms_iMaxVShaderImages;
//     int ms_iMaxPShaderImages;
//     int ms_iMaxStencilIndices;
//     int ms_iMaxUserClipPlanes;
// 
//     float ms_fMaxAnisotropy;
};

}

#endif