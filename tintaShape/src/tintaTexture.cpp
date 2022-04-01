/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaGraphicaPrecompile.h"
#include "tintaTexture.h"
#include "tintaRoot.h"

namespace Graphica {


    tintaTexture::tintaTexture()
	{}   
	

    tintaTexture::tintaTexture( FilterType type, const tintaImage   *img, const String &textureName ) :
        mFilter( type ),
        mImage( img ) {
        if( textureName.length() == 0 )
            mName = img->getName();
        else
            mName = textureName;
    }


    tintaTexture::~tintaTexture()
	{	
        mImage = NULL_M;
	}
	
    void tintaTexture::load ( const String &name ){
        tintaImage * img = Graphica::tintaRoot::getPtr()->getImage(name);
        if ( img ) {
            mImage = img;
            if( mName.length() == 0  )
                mName = img->getName();

        }
    }	

    void  tintaTexture::setImage( const tintaImage* img) {
        if ( img ) {
            mImage = img;
            if ( mName.length() == 0 )
                mName = img->getName();
        }
    }
    

	const tintaImage* const tintaTexture::getImage () const
	{
		return mImage;
	}    

	 void tintaTexture::setFilterType (FilterType type)
	{
         mFilter = type;
	}

	 FilterType tintaTexture::getFilterType() const
	{
		return mFilter;
	} 

}