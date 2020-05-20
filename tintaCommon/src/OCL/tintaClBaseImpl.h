/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_OPENCL_BASE_h_
#define _TINTA_OPENCL_BASE_h_



#include <string>
#include <assert.h>
#include <vector>

#include "tintaCLConfig.h"
#include <tintaException.h>
#include <tintaPredefine.h>
#include "tintaIClBase.h"
#include "tintaMemoryAllocator.h"

#ifdef USING_GPUCL 

namespace Tinta
{

/* 
    Base part of the Implementation for GPU algorithms
*/
	class  _CoreExport tintaClBaseImpl:
		public AllocatedObjectCLPrograms,
		public tintaIClBase
{
public:
	
	tintaClBaseImpl( const String &name, const String &fileName, const StringBasic &kernelName, m_uint32 platform = 0, m_uint32 device = 0);

	tintaClBaseImpl( const String &name, const char *src, const StringBasic &kernelName, m_uint32 platform = 0, m_uint32 device = 0);
    	
	
	virtual ~tintaClBaseImpl();		

	const String *getErrors(unsigned &errors_count) const;	

    /*
        Have to be called manually
    */
	void		  clearErrors();
	
    /*
        Returns the input data value by index
    */
	virtual GpuArg_t getDataIn( m_uint32 pos ) const ;

	/*
		If data have to be reassigned, call clearData first and reassign all data again
	*/
	virtual bool setDataIn(void *argumentData, GpuArg::enGpuArgType type, m_uint32 dataSize, m_uint32 memMask = 0x00);

    /*
        Returns the output data value by index
    */
	//virtual GpuArg_t getDataOut( m_uint32 pos ) const;
	
	/*
		If data have to be reassigned, call clearData first and reassign all data again
	*/
	//virtual bool setDataOut(const GpuArg_t &arg );

    /*
        Initialisation flag
    */
	virtual bool isInit()const;


    /*
    preferred platform
    */
    virtual void setPlatform( m_uint32 platform );

    /*
    preferred device
    */
    virtual void setDevice( m_uint32 device );

    /*
        Clears input and output data
    */
	virtual void clearData( );

	//virtual bool initData() = 0;


	virtual String getScriptFileName() const;

	virtual String getProgramName() const;

	virtual StringBasic getKernelName() const;

    bool create();

    virtual void* getKernel();


protected:

	
	void riseEXCEPTION(cl_int status, const StringBasic &err_message);
	
    bool		     mInited;

    cl_context       mContext;

    cl_kernel        mKernel;

    cl_program       mProgram;
    
    cl_command_queue mQueue;

    StringBasic  mKernelName;

    StringBasic  mSrc;    

    /*
        name from config
    */
    String              mName;

    String              mScriptPath;

	typedef std::vector<String> t_cl_error_container;

	t_cl_error_container mErrors;


	StringBasic      mErrTxt;

	
	typedef std::vector<GpuArg_t> GpuArgsArr_t;

	GpuArgsArr_t         mArgsIn;

	GpuArgsArr_t         mArgsOut;

	typedef std::vector<cl_mem> CLMemObjArr_t;

	CLMemObjArr_t mMemObjs;

	m_uint32 xGlobSize = 0;

	m_uint32 yGlobSize = 0;

    cl_platform_id *mPlatforms;

    m_uint32        mPlatform;

    cl_device_id   *mDevices;

    m_uint32        mDevice;
private:
	tintaClBaseImpl();

};

}

#endif
#endif


