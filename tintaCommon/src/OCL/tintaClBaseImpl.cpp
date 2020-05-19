/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaClBaseImpl.h"
#include "tintaCommon.h"
#include <iostream>

#ifdef USING_GPUCL 

namespace Tinta {


tintaClBaseImpl::tintaClBaseImpl()
:mInited(false)
,mContext(0)
,mKernel(0)
,mProgram(0)
,mQueue(0)
,mPlatforms(NULL_M)
,mPlatform(0)
,mDevice(0)
{

}

tintaClBaseImpl::tintaClBaseImpl(const String &name, const String &scriptPath,
                                const StringBasic &kernelName, m_uint32 platform, m_uint32 device)
:mInited(false)
,mContext(0)
,mKernel(0)
,mProgram(0)
,mQueue(0)
,mName( name )
,mScriptPath(scriptPath)
,mKernelName( kernelName )
,mPlatforms(NULL_M)
,mPlatform(platform)
,mDevice(device) {       
    readUTF8Text(mScriptPath, mSrc);    
}


tintaClBaseImpl::tintaClBaseImpl( const String &name, const char *src,
                            const StringBasic &kernelName, m_uint32 platform, m_uint32 device)
:mInited(false)
,mContext(0)
,mKernel(0)
,mProgram(0)
,mQueue(0)
,mName( name )
,mKernelName( kernelName )
,mPlatforms(NULL_M)
,mPlatform(platform)
,mDevice(device) {

    if ( src )
        mSrc = src;

}

tintaClBaseImpl::~tintaClBaseImpl(){
// 	if(mKernelSrc)
// 		delete[] mKernelSrc;

	if(mKernel)
		clReleaseKernel(mKernel);
	if(mProgram)
		clReleaseProgram(mProgram);
	if(mQueue)
			clReleaseCommandQueue(mQueue);
	if(mContext)
		clReleaseContext(mContext);	

    if ( mPlatforms )
            free(mPlatforms);
}

bool tintaClBaseImpl::isInit()const{
	return mInited;
}

String tintaClBaseImpl::getScriptFileName() const {
	return mScriptPath;
}

String tintaClBaseImpl::getProgramName() const {
	return mName;
}
StringBasic tintaClBaseImpl::getKernelName() const {
	return mKernelName;

}
bool tintaClBaseImpl::create() {


    mErrTxt = "CL kernel initialization error. Kernel file: ";
    mErrTxt.append(mScriptPath);
    mErrTxt.append(" Kernel: ");
    mErrTxt.append(mKernelName);


    cl_int status;

    // Discovery platform
    //cl_platform_id platforms[2];
    cl_platform_id platform;

    cl_uint num_platforms;

    /* Find number of platforms */
    status = clGetPlatformIDs(1, NULL, &num_platforms);

    riseEXCEPTION(status, "Error: clGetPlatformIDs");
    if (mPlatform >= num_platforms) {
        riseEXCEPTION(-1, "Error: wrong platform number");
    }

    mPlatforms = (cl_platform_id*)
        malloc(sizeof(cl_platform_id) * num_platforms);

    status = clGetPlatformIDs(num_platforms, mPlatforms, NULL);

    riseEXCEPTION(status, "Error: clGetPlatformIDs");


    platform = mPlatforms[mPlatform];

    // Discover device 
    cl_uint num_devices;
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, NULL, NULL, &num_devices);
    riseEXCEPTION(status, " Error: clGetDeviceIDs");
    if (mDevice >= num_devices) {
        riseEXCEPTION(-1, "Error: wrong device number");
    }
    /* Access all installed platforms */
    mDevices = (cl_device_id*)
        malloc(sizeof(cl_device_id) * num_devices);

    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, num_devices, mDevices, NULL);


    //cl_device_id device = 0;
    //clGetDeviceIDs(platform, TINTA_CL_DEVICE_TYPE, 1, &device, NULL);	
    //if(!device)
    //	status = -33; // not supported device

    //error.append(" Error: clGetDeviceIDs");
    riseEXCEPTION(status, " Error: clGetDeviceIDs");


    // Create context
    cl_context_properties props[3] = { CL_CONTEXT_PLATFORM,
        (cl_context_properties)(platform), 0 };

    //cl_context context;
    mContext = clCreateContext(props, 1, &mDevices[mDevice], NULL, NULL, &status);

    //error.append(" Error: clCreateContext");
    riseEXCEPTION(status, " Error: clCreateContext");

    assert(mContext);

    // Create command queue	
    mQueue = clCreateCommandQueue(mContext, mDevices[mDevice], 0, &status);

    //error.append(" Error: clCreateCommandQueue");
    riseEXCEPTION(status, " Error: clCreateCommandQueue");

    // Create a program object with source and build it
    const char* src = mSrc.c_str();
    mProgram = clCreateProgramWithSource(mContext, 1, &src, NULL, &status);

    //error.append(" Error: clCreateProgramWithSource");
    riseEXCEPTION(status, " Error: clCreateProgramWithSource");

    status = clBuildProgram(mProgram, 1, &mDevices[mDevice], NULL, NULL, NULL);
    StringStreamBasic msg;
    msg << "Error: clBuildProgram";

    if (status == CL_BUILD_PROGRAM_FAILURE) {
        // Determine the size of the log
        size_t log_size;
        clGetProgramBuildInfo(mProgram, mDevices[mDevice], CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

        // Allocate memory for the log
        //assert_ uniqptr
        UNIQPTRALLOC(char, log, ALLOC_T(char, log_size));
        //char *log = new char [log_size];

        // Get the log
        clGetProgramBuildInfo(mProgram, mDevices[mDevice], CL_PROGRAM_BUILD_LOG, log_size, log.get(), NULL);
        msg << log.get();

    }
    riseEXCEPTION(status, msg.str());

    // Create the kernel object	
    mKernel = clCreateKernel(mProgram, mKernelName.c_str(), &status);

    //error.append(" Error: clCreateKernel");
    riseEXCEPTION(status, " Error: clCreateKernel");

    mInited = true;

    return mInited;
}

GpuArg_t tintaClBaseImpl::getDataIn( m_uint32 pos ) const {

	if( pos < mArgsIn.size()  ){
		return mArgsIn.at(pos);
	}
	return GpuArg_t();
}



bool tintaClBaseImpl::setDataIn(void *argumentData, GpuArg::enGpuArgType type, m_uint32 dataSize, m_uint32 memMask ) {



	//assert(arg.mData && arg.mArgType != GpuArg::enLastType);

	cl_int status;
    mArgsIn.push_back({ argumentData, type , dataSize });

	if (type == GpuArg_t::enArray ){
			
		cl_mem data = clCreateBuffer(mContext, memMask, dataSize, NULL, &status);
		riseEXCEPTION(status, " Error: setDataIn");
		mMemObjs.push_back(data);

		
		status = clEnqueueWriteBuffer(mQueue, data, CL_TRUE, 0, dataSize, argumentData, 0, NULL, NULL);
		riseEXCEPTION(status, " Error: clEnqueueWriteBuffer");
		

		status = clSetKernelArg(mKernel, mArgsIn.size() - 1, sizeof(cl_mem), &data);
		riseEXCEPTION(status, " Error: setDataIn");
		
	}
	else if (type == GpuArg_t::enValue ){
			
		status = clSetKernelArg(mKernel, mArgsIn.size() - 1, dataSize, argumentData);
		riseEXCEPTION(status, " Error: setDataIn");
		mMemObjs.push_back(NULL);
	}
	else
		return false;
				
	return true;
	
}
#if 0
GpuArg_t tintaClBaseImpl::getDataOut( m_uint32 pos ) const {

	if( pos < mArgsOut.size()  ){
		return mArgsOut.at(pos);
	}
	return GpuArg_t();
}

bool tintaClBaseImpl::setDataOut( const GpuArg_t &arg ){
		assert(arg.mData && (arg.mTypeBasic || arg.mTypeArr));
		

		cl_int status;
		mArgsOut.push_back(arg);

		if (arg.mTypeArr != GpuArg_t::enLastArrType){

			cl_mem data = clCreateBuffer(mContext, CL_MEM_WRITE_ONLY, arg.mDataSize, NULL, &status);
			riseEXCEPTION(status, " Error: setDataOut");
			mMemObjs.push_back(data);
			status = clEnqueueWriteBuffer(mQueue, data, CL_TRUE, 0, arg.mDataSize, arg.mData, 0, NULL, NULL);
		}
		else if (arg.mTypeBasic != GpuArg_t::enLastBasicType){
			status = clSetKernelArg(mKernel, 0, sizeof(cl_mem), arg.mData);
			riseEXCEPTION(status, " Error: setDataOut");
		}
		else
			return false;

	return true;
	
}
#endif
void tintaClBaseImpl::clearData( ){
	
	mArgsIn.clear();
	CLMemObjArr_t::iterator itcl =	mMemObjs.begin(); 
	for(;itcl!= mMemObjs.end(); itcl++){
		if( *itcl )
			clReleaseMemObject(*itcl);
		*itcl = NULL;
	}
	mMemObjs.clear();
} 


/*
    preferred platform
*/
void tintaClBaseImpl::setPlatform( m_uint32 platform ) {
    mPlatform = platform;
}

/*
    preferred device
*/
void tintaClBaseImpl::setDevice( m_uint32 device ) {
    mDevice = device;
}



const String *tintaClBaseImpl::getErrors(unsigned &errors_count) const{
	errors_count = mErrors.size();

	if(errors_count == 0)
		return 0;

	return &mErrors[0];
}

void tintaClBaseImpl::clearErrors(){
	mErrors.clear();
}

//bool tintaClBaseImpl::initData() = 0;

void tintaClBaseImpl::riseEXCEPTION(cl_int status, const StringBasic &err_message){
	if( status != CL_SUCCESS && err_message.length() > 0 ){      

		StringStreamBasic msg;
		msg << mErrTxt;	
		msg << " Status: "<<status<<" ";	
		msg << err_message;
		
        EXCEPTION(msg.str() );
		return;
	}
	
}

}

#endif
