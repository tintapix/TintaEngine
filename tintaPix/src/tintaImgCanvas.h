/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_IMG_CANVAS_H_
#define _TINTA_IMG_CANVAS_H_

#include "tintaPredefine.h"

#include <tintaScriptContext.h>
#include "tintaConfigurator.h"
#include "tintaUInt8Image.h"
#include "tintaImgStack.h"

 
namespace Tinta
{
		
	typedef tintaImgCanvas Images_t;	

    typedef int(*OperationComplete)();

    
	/// Class implements Images container
	class  _CoreExport tintaImgCanvas: 
		public tintaImgStack
	{
		
	protected:
		
		//images_vec_type m_images;
		int		   m_image_selected; 
		// back color default from config
		color_type m_color_default;				

		//Tinta::tintaScriptContext *m_context;	

		tintaPixImageContainer  *mImages;

		imageFactories_t	    mImageFactVec;		

	public:		
		tintaImgCanvas(void);	

		virtual ~tintaImgCanvas(void);
		
		virtual void initialization();
	
		/// selecting image by index
		/// return false if image does not exist	
		virtual bool					selectImage(m_uint32 index_);


		/// getting image by index	
		virtual const tintaUInt8Image		*getImage(unsigned index) const;	

		/// getting image by index	
		virtual tintaUInt8Image				*getImageEx(unsigned index);	
		
		/// getting selected image
		virtual const tintaUInt8Image		*getImgSel() const;
		
		/// getting selected image for editing
		virtual tintaUInt8Image				*getImgSelEx();

		/// returns images quantity in container
		virtual size_t			   getImgQuantity()const;

		/// deleting image by index	
		virtual void					delImg(unsigned index);	

		virtual void					delAllImg();
		/// return index of selected image
		virtual m_uint32					getIndexImgSel() const;		

		/// adds new image into the set
		virtual m_uint32					addImg( const StringBasic &type);	

		//virtual Tinta::tintaScriptContext *getContext();	

		

		virtual const tintaPixImageContainer* getContainer()const;
	
		//virtual tintaRandomV2*			getRandGen();
	};
}



#endif

