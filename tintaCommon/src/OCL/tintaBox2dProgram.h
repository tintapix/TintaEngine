/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_BOX2DPROGRAM_CL_H_
#define _TINTA_BOX2DPROGRAM_CL_H_





#include "tintaClBaseImpl.h"
#include "tintaPredefine.h"

#ifdef USING_GPUCL 

namespace Tinta
{

	/*
		Programs to processing Box2d container( buffers, images )
	*/
	class  Box2dProgram
		: public tintaClBaseImpl
	{
	public:
		
		
		virtual bool execute(const size_t *globalsize, m_uint32 dimensions, const size_t *readidc,
			m_uint32 readquant, const size_t *workitems = NULL);

		Box2dProgram(const String &uniqName, const String &pathToFile, const StringBasic &kernelName
                                                                ,m_uint32 platform = 0, m_uint32 device = 0);
		virtual ~Box2dProgram();	
	};

}

#endif
#endif