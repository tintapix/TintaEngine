/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_IMG_STACK_H_
#define _TINTA_IMG_STACK_H_

#include <tintaPredefine.h>
#include <tintaSingleton.h>
#include <tintaScriptContext.h>
#include <tintaCommon.h>
#include "tintaScriptTaskExecutor.h"
#include "tintaScriptFunc.h"
#include "tintaObjContainer.h"
#include <tintaImageFactory.h>
#include <tintaISerialisableSet.h>


namespace Tinta
{	
	
	


	//typedef TBaseScriptFunc<tintaUInt8Image> t_base_script_image;
	struct _CoreExport tintaImgStack:
		public Singleton<tintaImgStack>,
        public tintaINamed,
        public tintaISerialisableSet,
		public AllocatedObjectCanvas
	{	
		
		typedef tintaObjContainer< tintaUInt8Image, tintaImageFactory > tintaPixImageContainer;
		typedef std::vector<tintaImageFactory*> imageFactories_t;


		typedef tintaUInt8Image image_type;	
		//typedef  std::map< THREAD_CURRENT_ID_TYPE, tintaImageContainer > t_map_image_containers;
		
        MUTEX_RECURSIVE(mMutexImg)

        static tintaImgStack* getPtr(void)	{			
			return mPtr;
		}
        
        virtual ~tintaImgStack();

		virtual bool selectImage(m_uint32 index) = 0;		

		virtual const tintaUInt8Image *getImage(m_uint32 index)const = 0;

		// sets the temporary image instead of selected image, must be removed manualy by calling setTempImage(0)
		

		virtual tintaUInt8Image		 *getImageEx(m_uint32 index) = 0;

		virtual const tintaUInt8Image *getImgSel()const = 0;

		/// deleting image		
		virtual void			      delImg(unsigned index) = 0;

		/// deleting image		
		virtual void			      delAllImg() = 0;

		virtual m_uint32			  getIndexImgSel() const = 0;

		virtual tintaUInt8Image		 *getImgSelEx() = 0;

		virtual size_t			      getImgQuantity()const = 0;

		/// adds new image into the set 
		virtual m_uint32			   addImg( const StringBasic &type ) = 0;	

		virtual void			       initialization() = 0;		
		
		virtual const tintaPixImageContainer* getContainer()const = 0;



        virtual const StringBasic& getName() const;

        virtual size_t GetBufferSize(m_uint32 key) const;

        virtual size_t packData(m_uint32 key, m_int8 *data, size_t offset) const;

        virtual StringBasic GetObjectName(m_uint32 key) const;

        virtual size_t unpackData(const StringBasic &type, const m_int8 *data, size_t offset, m_uint32 &rezId);

        static const StringBasic strCanvasContainerName;
		
	};

	typedef  Tinta::tintaImgStack::image_type image_t;
		
}


#endif
