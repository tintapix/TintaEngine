/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaPlatform.h"
#include "tintaPredefine.h"
#include "MemTracker.h"


namespace Tinta {


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
    MemTracker& MemTracker::get(){
        static MemTracker tracker;
        return tracker;
    }
    void MemTracker::recordAlloc(void* ptr, size_t sz, m_uint32 pool,
                                    const char* file, size_t ln, const char* func){

        assert(m_allocations.find(ptr) == m_allocations.end());

        TINTA_LOCK_RECUR_MUTEX_AUTO;

            m_allocations[ptr] = Alloc(sz, pool, file, ln, func);
        if (pool >= m_allocations_by_pool.size())
            m_allocations_by_pool.resize(pool + 1, 0);
        m_allocations_by_pool[pool] += sz;
        m_total_alloc += sz;
    }

    void MemTracker::recordDealloc(void* ptr){
        if (!ptr)
            return;

        TINTA_LOCK_RECUR_MUTEX_AUTO;

            AllocationMap::iterator i = m_allocations.find(ptr);
        assert(i != m_allocations.end());
        m_allocations_by_pool[i->second.mPool] -= i->second.mBytes;
        m_total_alloc -= i->second.mBytes;
        m_allocations.erase(i);
    }
    size_t MemTracker::getTotalMemAllocated() const{
        return m_total_alloc;
    }

    size_t MemTracker::getMemAllocated(m_uint32 pool) const{
        return m_allocations_by_pool[pool];
    }
    void MemTracker::leaksReport(){
        StringStreamBasic os;
        if (m_allocations.empty()){
            os << "No memory leaks" << std::endl;
        }
        else {
            os << "Detected memory leaks !!! " << std::endl;
            os << "(" << m_allocations.size() << ") Allocation(s) with total " << m_total_alloc << " mbytes." << std::endl;
            os << "Dumping allocations -> " << std::endl;
            for (AllocationMap::const_iterator i = m_allocations.begin(); i != m_allocations.end(); ++i){
                const t_alloc_info& alloc = i->second;
                if (!alloc.mFile.empty())
                    os << alloc.mFile;
                else
                    os << "(unknown source):";
                os << "(" << alloc.mLine << ") : {" << alloc.mBytes << " mbytes}" << " function: " << alloc.mFunc << std::endl;
            }
            os << std::endl;
        }
        std::cout << os.str();
        std::ofstream of;
        of.open(m_leaks_log_file.c_str());
        of << os.str();
        of.close();
    }

    MemTracker::~MemTracker(){
        leaksReport();
    }

}