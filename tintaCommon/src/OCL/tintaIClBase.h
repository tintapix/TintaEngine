/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_CL_BASE_H_
#define _TINTA_CL_BASE_H_

#include <tintaPredefine.h>
namespace Tinta
{
	
	typedef struct GpuArg {	

		// buffers arguments
		
		enum enGpuArgType{ 			
			enValue,					
			enArray,
			enLastType
		};			

        void *mData;
		enGpuArgType mType;
		size_t mDataSize;

	} GpuArg_t;

	// mem types binds
	enum GPUMem{ 
		enNoAccess = 0x00,
		enReadWrite = 0x01,   //CL_MEM_READ_WRITE
		enWrite = 0x02,		  //CL_MEM_WRITE_ONLY
		enRead = 0x04,        //CL_MEM_READ_ONLY
		enHostPtr = 0x08,     //CL_MEM_USE_HOST_PTR
		enAllocHostPtr = 0x10,//CL_MEM_ALLOC_HOST_PTR
		enCopyHostPtr = 0x20  //CL_MEM_COPY_HOST_PTR

	};
	class tintaIClBase {

	public:

		virtual ~tintaIClBase(){}
		/*
			globalsize size_t [dims] = {dim1_size, dim2_size};
			dimensions = dims            
			workitems = items in group size_t [dims] = {dim1_items, dim2_items};
		*/
		virtual bool execute(const size_t *globalsize, m_uint32 dimensions, const size_t *readidc, m_uint32 readquant, const size_t *workitems = NULL) = 0;

		/*
			Assigning input values for program			
			return false if argument type or position is wrong
		*/
		virtual bool setDataIn( void *data, GpuArg::enGpuArgType type, m_uint32 dataSize, m_uint32 memMask = 0x00) = 0;

		/*
			Returns type for registered  type of the input data, enLastType if position is wrong
		*/
		//virtual GpuArg_t getDataOut( m_uint32 pos ) const = 0;

		/*
			Assigning output values for program
			pos - position from 0 
			return false if argument type or position is wrong
		*/
		//virtual bool setDataOut(   const GpuArg_t &arg ) = 0;

		/*
			Returns type for registered  type of the input data, enLastType if position is wrong
		*/
		virtual GpuArg_t getDataIn( m_uint32 pos ) const = 0;

		/*
			clears all data			
		*/
		virtual void clearData( ) = 0;


		virtual bool isInit() const = 0;
		
		/*
			Returns file name for gpu program
		*/		
		virtual String getScriptFileName() const = 0;

		/*
			Returns uniq name for gpu program
		*/
		virtual String getProgramName() const = 0;

		/*
			Returns kernel name for gpu program
		*/
		virtual StringBasic getKernelName() const = 0;

        /*
            preferred platform
        */
        virtual void setPlatform( m_uint32 platform ) = 0;

        /*
            preferred device
        */
        virtual void setDevice( m_uint32 device ) = 0;       
        /*
            creates program
        */
        virtual bool create() = 0;
	};
}


#endif