/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
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

	// if template image was not removed
    tintaIImgWindow* winp = Tinta::tintaTexSpringMain::getPtr()->getWindow();
    const tintaUInt8Image* img = getImage( index );

    if ( !img )
        return false;

    //if ( img && winp )
       // winp->showImage( index );

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
size_t	tintaImgCanvas::getImgQuantity()const {
		return mImages->getSize();
}


void tintaImgCanvas::delImg( unsigned index ){
    TINTA_LOCK_RECUR_MUTEX(mMutexImg);
 	mImages->delObj( index );

    
    tintaIImgWindow* winp = Tinta::tintaTexSpringMain::getPtr()->getWindow();

    if ( winp )
        winp->deleteData( index );
      

    //if (winp)
       // winp->showImage( getIndexImgSel() );
}

void tintaImgCanvas::delAllImg() {	
    TINTA_LOCK_RECUR_MUTEX(mMutexImg);
	mImages->clear();

    tintaIImgWindow* winp = Tinta::tintaTexSpringMain::getPtr()->getWindow();
    if ( winp )
        winp->deleteData( ZERO_ID );
}

m_uint32 tintaImgCanvas::getIndexImgSel() const {	
	return mImages->getIndexSel();
}




m_uint32 tintaImgCanvas::addImg( const StringBasic &type ) {
    TINTA_LOCK_RECUR_MUTEX(mMutexImg);
	return mImages->addObj( type );	
}


const tintaImgCanvas::tintaPixImageContainer* tintaImgCanvas::getContainer()const {
	return mImages;
}

// Tinta::tintaScriptContext* tintaImgCanvas::getContext() {
// 	
// 	return m_context;
// }	

// tintaScriptTaskExecutor* tintaImgCanvas::getTaskExecutor(){
// 	return m_queue_task_executor;
// }
// void tintaImgCanvas::delImg_container(THREAD_CURRENT_ID_TYPE thread_id) {
// 
// 
// // 	t_map_image_containers::iterator it_find = Images_ts.find( THREAD_CURRENT_ID );
// // 	//	no images no problem
// // 	if(it_find != Images_ts.end()){
// // 		it_find->second.delAllImg();
// // 		Images_ts.erase(it_find);		
// // 	}
// }

// tintaIClBase* tintaImgCanvas::getClObj( const char *kernel_name ){
// #ifdef USING_GPUCL
// 	return m_cl_obj_container.find_object(kernel_name);
// #endif// 	return 0;
// 	
// }



}
