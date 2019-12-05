/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _MEM_TRACKER_H_
#define _MEM_TRACKER_H_

#include "tintaPredefine.h"


namespace Tinta
{

    /*
    This source file is part of OGRE
        (Object - oriented Graphics Rendering Engine)
        For the latest info, see http ://www.ogre3d.org/

    Copyright(c) 2000 - 2009 Torus Knot Software Ltd

        Permission is hereby granted, free of charge, to any person obtaining a copy
        of this software and associated documentation files(the "Software"), to deal
        in the Software without restriction, including without limitation the rights
        to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
        copies of the Software, and to permit persons to whom the Software is
        furnished to do so, subject to the following conditions :

    The above copyright notice and this permission notice shall be included in
        all copies or substantial portions of the Software.

        THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
        IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
        FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
        AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
        LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
        OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
        THE SOFTWARE.
*/
	class _CoreExport MemTracker {
	public:
		~MemTracker();
	protected:		
        AUTO_MUTEX;
		typedef struct Alloc {

			unsigned mBytes;
			m_uint32 mPool;
			StringBasic mFile;
			unsigned mLine;
			StringBasic mFunc;
			
			Alloc(size_t sz, m_uint32 p, const char* file, size_t ln, const char* func)
                :mBytes(sz),
                 mPool(p),
                 mLine(ln){

				if (file)
                    mFile = file;
				if (func)
                    mFunc = func;
			}
            Alloc() :mBytes(0), mLine(0) {}
		} t_alloc_info; 

		StringBasic m_leaks_log_file;		
        typedef std::unordered_map<void*, t_alloc_info> AllocationMap;
		AllocationMap m_allocations;

		size_t m_total_alloc;
		typedef std::vector<size_t> AllocationsByPool;
		AllocationsByPool m_allocations_by_pool;
		
		MemTracker()
			: m_leaks_log_file("leaks.txt"),
			m_total_alloc(0)
		{}		
	public:			
		size_t getTotalMemAllocated() const;		
		size_t getMemAllocated(m_uint32 pool) const;
		void recordAlloc(void* ptr, size_t sz, m_uint32 pool = 0, const char* file = 0, size_t ln = 0, const char* func = 0);		
		void recordDealloc(void* ptr);
		void leaksReport();
		static MemTracker& get();
	};
}

#endif
