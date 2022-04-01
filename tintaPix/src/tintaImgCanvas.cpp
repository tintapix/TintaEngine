/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaPrecompile.h"
#include "tintaImgCanvas.h"

#include "tintaObjCreator.h"
#include "tintaUInt8Image.h"
#include "tintaUInt8ImageSeamless.h"
#include "tintaCommonImageFactory.h"
#include "tintaTexSpringMain.h"


namespace Tinta
{

tintaImgCanvas::tintaImgCanvas(void)		
   :m_image_selected(-1),
   m_color_default( { 255, 255, 255, 0 } )
{
	mImages			   = NEW_T(tintaPixImageContainer)();
	
}

void tintaImgCanvas::initialization(){
	mImageFactVec.push_back( M_NEW tintaCommonImageFactory() );
	mImageFactVec.push_back( M_NEW tintaSeamlessImageFactory() );
	
	for( auto it = mImageFactVec.begin(); it!= mImageFactVec.end(); it++ )	{
		bool rez = 	mImages->registerFactory( *it );	
		CoreAssert( rez,"bool rez = 	mImages->registerFactory( *it );	" );
	}
}

tintaImgCanvas::~tintaImgCanvas( void ){

	mImages->clear();
	if( mImages ){
		DELETE_T(mImages, tintaPixImageContainer);		
		mImages = 0;
	}

	for(imageFactories_t::iterator it = mImageFactVec.begin(); it!= mImageFactVec.end(); it++) 		M_DELETE  *it;	
	mImageFactVec.clear();
}


bool tintaImgCanvas::selectImage(m_uint32 index)
{
    TINTA_LOCK_RECUR_MUTEX( mMutexImg );
    
	return mImages->selectObj( index );
}


const tintaUInt8Image* tintaImgCanvas::getImage( unsigned index ) const {	
	return mImages->getObj( index );
}

tintaUInt8Image* tintaImgCanvas::getImageEx(unsigned  index ){	
	
	return mImages->getObjEx( index );
}

const tintaUInt8Image* tintaImgCanvas::getImgSel() const {		
	return mImages->getObj( mImages->getIndexSel() );
}

tintaUInt8Image* tintaImgCanvas::getImgSelEx(){	
	return mImages->getObjEx( mImages->getIndexSel() );
}
size_t	tintaImgCanvas::size()const {
	return mImages->getSize();
}

const tintaUInt8Image *tintaImgCanvas::getNextImg(m_uint32 &index) const {
    return mImages->getNextObj(index);
}

const tintaUInt8Image *tintaImgCanvas::getFirstImg(m_uint32 &index) const {
    return mImages->getFirstObj(index);
}


void tintaImgCanvas::delImg( m_uint32 index ){
    TINTA_LOCK_RECUR_MUTEX(mMutexImg);
 	mImages->delObj( index );

    auto posIt = std::find_if(mPositions.begin(), mPositions.end(), [&](const Position_t &v) { return v.index == index;  });
    if (posIt != mPositions.end())
        mPositions.erase(posIt);
      

    //if (winp)
       // winp->showImage( getIndexImgSel() );
}

tintaVector2f  tintaImgCanvas::getPosition( m_uint32 &index ) const {
    auto posIt = std::find_if(mPositions.begin(), mPositions.end(), [&](const Position_t &v) { return v.index == index;  });
    if (posIt != mPositions.end()) {
        return posIt->position;
    }
    return{ 0.f, 0.f };
}

void  tintaImgCanvas::setPosition( m_uint32 &index, const tintaVector2f &pos ) {

    auto posIt = std::find_if(mPositions.begin(), mPositions.end(), [&](const Position_t &v) { return v.index == index;  });
    if (posIt != mPositions.end()) {
        posIt->position = pos;
    }
}

void tintaImgCanvas::delAllImg() {	
    TINTA_LOCK_RECUR_MUTEX(mMutexImg);
	mImages->clear();
    mPositions.clear();   
}

m_uint32 tintaImgCanvas::getIndexImgSel() const {	
	return mImages->getIndexSel();
}

m_uint32 tintaImgCanvas::addImg( const StringBasic &type ) {
    TINTA_LOCK_RECUR_MUTEX(mMutexImg);
	m_uint32 index = mImages->addObj( type );	
    mPositions.push_back( { index , {0.f, 0.f} });

    return index;
}


const tintaImgCanvas::tintaPixImageContainer* tintaImgCanvas::getContainer()const {
	return mImages;
}

}
