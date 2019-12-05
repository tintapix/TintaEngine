/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaMemoryAllocator.h"

#ifndef _nedmallocc_defined_

	#include "nedmalloc/nedmalloc.c"
	#define _nedmallocc_defined_
#endif


namespace Tinta
{

	namespace ned_space
	{

        // good wrapper from OGRE
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
		const size_t s_poolCount = 14;		 
		void* s_poolFootprint = reinterpret_cast<void*>(0xBB1AA45A);
		nedalloc::nedpool* s_pools[s_poolCount + 1] = { 0 };
		nedalloc::nedpool* s_poolsAligned[s_poolCount + 1] = { 0 };

		size_t getIdpoolFromSize(size_t a_reqSize)
		{
			// Requests size 16 or smaller are allocated at a 4 m_byte granularity.
			// Requests size 17 or larger are allocated at a 16 m_byte granularity.
			// With a s_poolCount of 14, requests size 177 or larger go in the default pool.

			// spreadsheet style =IF(B35<=16; FLOOR((B35-1)/4;1); MIN(FLOOR((B35-1)/16; 1) + 3; 14))

			size_t poolID = 0;
			if (a_reqSize > 0){
				if (a_reqSize <= 16){
					poolID = (a_reqSize - 1) >> 2;
				}
				else{
					poolID = std::min<size_t>(((a_reqSize - 1) >> 4) + 3, s_poolCount);
				}
			}

			return poolID;
		}

		void* internalNedAlloc(size_t a_reqSize){
			size_t poolID = getIdpoolFromSize(a_reqSize);
			nedalloc::nedpool* pool(0); // A pool pointer of 0 means the default pool.
			if (poolID < s_poolCount){
				if ( s_pools[poolID] == 0 ){					
					s_pools[poolID] = nedalloc::nedcreatepool(0, 8);
					nedalloc::nedpsetvalue(s_pools[poolID], s_poolFootprint); // All pools are stamped with a footprint
				}
				pool = s_pools[poolID];
			}

			return nedalloc::nedpmalloc(pool, a_reqSize);
		}

		void* internalAllocNedAligned(size_t a_align, size_t a_reqSize){
			size_t poolID = getIdpoolFromSize(a_reqSize);
			nedalloc::nedpool* pool(0); // A pool pointer of 0 means the default pool.
			if (poolID < s_poolCount){
				if (s_poolsAligned[poolID] == 0){
					// Init pool if first use
					s_poolsAligned[poolID] = nedalloc::nedcreatepool(0, 8);
					nedalloc::nedpsetvalue(s_poolsAligned[poolID], s_poolFootprint); // All pools are stamped with a footprint
				}

				pool = s_poolsAligned[poolID];
			}

			return nedalloc::nedpmemalign(pool, a_align, a_reqSize);
		}

		void internalNedFree(void* a_mem)
		{
			if (a_mem)
			{
				nedalloc::nedpool* pool(0);

				// nedalloc lets us get the pool pointer from the memory pointer
				void* footprint = nedalloc::nedgetvalue(&pool, a_mem);

				// Check footprint
				if (footprint == s_poolFootprint)
				{
					// If we allocated the pool, deallocate from this pool...
					nedalloc::nedpfree(pool, a_mem);
				}
				else
				{
					// ...otherwise let nedalloc handle it.
					nedalloc::nedfree(a_mem);
				}
			}
		}
	}


    void* MemoryPoolingPolicy::allocate_mbytes(size_t count, const char* file, int line, const char* func){
        return MemoryPooling::allocBytes(count, file, line, func);
    }
    void MemoryPoolingPolicy::deallocate_mbytes(void* ptr){
        MemoryPooling::deallocBytes(ptr);
    }

    size_t MemoryPoolingPolicy::get_allocation_size(){
        return 0;// std::numeric_limits<size_t>::max();
    }

	void* MemoryPooling::allocBytes(size_t count,const char* file, int line, const char* func){
		//void* ptr = malloc(count);//ned_space::internalNedAlloc(count);
		void* ptr = ned_space::internalNedAlloc(count);
 #if CORE_MEMORY_TRACKER
 		MemTracker::get().recordAlloc(ptr, count, 0, file, line, func);
 #else
 		// avoid unused params warning
 		file = func = "";
 		line = 0;
 #endif
		return ptr;
	}
	void MemoryPooling::deallocBytes(void* ptr){
		// deal with null
		if ( !ptr )
			return;
 #if CORE_MEMORY_TRACKER
 		MemTracker::get().recordDealloc(ptr);
 #endif
		//free(ptr);
 		ned_space::internalNedFree(ptr);
	}
#	if 0
	void* MemoryPooling::allocBytesAligned(size_t align, size_t count, const char* file, int line, const char* func){
		// default to platform SIMD alignment if none specified impl mev SIMD Align
		void* ptr =  ned_space::internalAllocNedAligned(align, count);			
 #if CORE_MEMORY_TRACKER
 		MemTracker::get().recordAlloc(ptr, count, 0, file, line, func);
 #else
 		// avoid unused params warning
        file = func = "";
 		line = 0;
 #endif
		return ptr;
	}

	void MemoryPooling::deallocBytesAligned(size_t align, void* ptr){
		if ( !ptr )
			return;
 #if CORE_MEMORY_TRACKER
 		MemTracker::get().recordDealloc(ptr);
 #endif
 		ned_space::internalNedFree(ptr);
	}
#endif
}
