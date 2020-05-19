/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */



#include "tintaBox2dProgram.h"

#ifdef USING_GPUCL 

namespace Tinta
{

	bool Box2dProgram::execute(const m_uint32 *globalsize, m_uint32 dimensions, const size_t *readidc,
		m_uint32 readquant, const m_uint32 *workitems) {

        
       // size_t offset[1] = { 4 };

		cl_int status = clEnqueueNDRangeKernel(mQueue, mKernel, dimensions, NULL, globalsize, workitems, 0,
			NULL, NULL);
		riseEXCEPTION(status, " Error: clEnqueueNDRange");

		// Read the image back to the host 			
		for ( m_uint32 i = 0; i< readquant; i++ ){

            if (mArgsIn.size() <= readidc[i]) {
                riseEXCEPTION(CL_INVALID_PROPERTY, " Error: invalid output parameter index");
            }

			status = clEnqueueReadBuffer( mQueue, mMemObjs[readidc[i]], CL_TRUE, 0, mArgsIn.at(readidc[i]).mDataSize, mArgsIn.at(readidc[i]).mData, 0, NULL, NULL );
			riseEXCEPTION(status, " Error: clEnqueueReadBuffer");

		}
		return true;
	}
	Box2dProgram::Box2dProgram(const String &uniqName, const String &pathToFile, const StringBasic &kernelName
                                                                    ,m_uint32 platform, m_uint32 device)
		:tintaClBaseImpl(uniqName
		, pathToFile
		, kernelName, platform, device)
	{

		//setOutArgsQuantity();
	}
	Box2dProgram::~Box2dProgram()	{

	}


}

#endif
