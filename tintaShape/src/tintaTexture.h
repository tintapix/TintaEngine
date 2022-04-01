/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef TINTA_TEXTURE_H
#define TINTA_TEXTURE_H


#include "tintaResource.h"
#include "tintaRGBAColor.h"
#include "tintaImage.h"

namespace Graphica {

class _CoreExport tintaTexture
    :public AllocatedMemObject,
     public tintaResource< TextureID > {

public:
    tintaTexture();

    tintaTexture( FilterType type, const tintaImage *image, const String &textureName = _M("") );
   
    virtual ~tintaTexture ();
    
    void  setImage( const  tintaImage* img );

    void load( const String &imgName );

    const tintaImage* const getImage  () const;   
    
    void       setFilterType ( FilterType type);

    FilterType getFilterType () const;        
    	

protected:   

    const tintaImage   *mImage = NULL_M;
    FilterType    mFilter = NEAREST;   
    
};

}

#endif
