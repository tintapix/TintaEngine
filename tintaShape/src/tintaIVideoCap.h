/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_IVIDEO_CAP_H_
#define _TINTA_IVIDEO_CAP_H_


/*
    Describes video card capability 
*/
#include <tintaPredefine.h>


struct tintaIVideoCap {
        
    enum IntVideoCap{
        CapiMaxLights,        
        CapiMaxTCoords,
        CapiMaxVShaderImages,
        CapiMaxPShaderImages,
        CapiMaxStencilIndices,
        CapiMaxUserClipPlanes,             
        CapiLast = CapiMaxUserClipPlanes
    };

    enum FloatVideoCap{
        CapfMaxAnisotropy,
        CapfLast = CapfMaxAnisotropy
    };


    virtual Tinta::StringBasic getVendor() const = 0;

    virtual Tinta::StringBasic getRenderer() const = 0;

    virtual Tinta::StringBasic getVerRenderer() const = 0;


    virtual void getGlVersion(int *major, int *minor) = 0;

    virtual void getGlSLVersion(int *major, int *minor) = 0;

    virtual bool isExtSupports( const Tinta::StringBasic &ex )const = 0 ;

    virtual int getIntCap(IntVideoCap c)const = 0;

    virtual float getFloatCap(FloatVideoCap c)const = 0;

    virtual Tinta::String getExtensionsInfo()const = 0;


}; 

#endif