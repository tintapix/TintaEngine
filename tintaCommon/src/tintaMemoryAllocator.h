/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_MEMORY_ALLOCATOR_H_
#define _TINTA_MEMORY_ALLOCATOR_H_


#include "MemTracker.h"
#include <memory>

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


	enum object_categories {
		ALLOC_COMMON,
		ALLOC_IMAGES,
		ALLOC_LOG,
		ALLOC_COMP_OBJ_CREATOR,
		ALLOC_CANVAS,
		ALLOC_COMP_OBJ,
		ALLOC_BOX_CONTAINER,
		ALLOC_GPU_PROCESS,
		ALLOC_TASK,
		ALLOC_IMAGE_CODEC,
		ALLOC_COMP_OBJ_FACTORY,
		ALLOC_IMAGE_FACTORY,
		ALLOC_BUFF_FACTORY,
		ALLOC_BUFF_ACTIONS,
		ALLOC_VAL_MAP_FACTORY,
		ALLOC_VAL_STACK_FACTORY,
		ALLOC_INTERACT,
		//ALLOC_POINTS_BUFFER,
		//ALLOC_POINTS_BUFFER_ALGORITHM,
		
		// graphic engine
		ALLOC_RENDER_OBJ,
		ALLOC_EFFECT,
		ALLOC_SPATIAL_OBJ,
		ALLOC_BINDABLE_OBJ,
		ALLOC_WIDGET_OBJ,
		ALLOC_APPLICATION,
		ALLOC_BOUNDVOLUME,
        ALLOC_GLSL_SHADER,
        ALLOC_MEMOBJ,

		// QT 
		ALLOC_QTWINDOW

		
	};

    class MemoryPoolingPolicy
    {
    public:
        static void* allocate_mbytes(size_t count, const char* file = 0, int line = 0, const char* func = 0);

        static void deallocate_mbytes(void* ptr);

        static size_t get_allocation_size();

    private:
        MemoryPoolingPolicy()
        { }
    };

	template <class Allocator>
	class Allocated
	{
	public:
		explicit Allocated(){ }
		~Allocated(){ }


// to prevent using any other macros instead of M_NEW when deriving, stub was added
#if DEBUG_MODE
		void* operator new(size_t size, int stub, const char* file, int line, const char* func){			
			return Allocator::allocate_mbytes(size, file, line, func);
		}
// #else
// 		void* operator new(size_t sz, const char* file, int line, const char* func){
// 			return Allocator::allocate_mbytes(sz, file, line, func);
// 		}

#endif

#if DEBUG_MODE
		void* operator new(size_t sz, int stub){
			return Allocator::allocate_mbytes(sz);
		}
#else
		void* operator new(size_t sz){
			return Allocator::allocate_mbytes(sz);
		}
#endif

		void* operator new[] ( size_t sz ){
			return Allocator::allocate_mbytes(sz);
		}
#if DEBUG_MODE
		void* operator new(size_t sz, void* ptr, int stub ){
			(void) sz;
			return ptr;
		}	
#else		
		void* operator new(size_t sz, void* ptr){
			(void) sz;
			return ptr;
		}		
#endif		
		void* operator new[] ( size_t sz, const char* file, int line, const char* func ){
			return Allocator::allocate_mbytes(sz, file, line, func);
		}	

		void operator delete( void* ptr ){ 
			Allocator::deallocate_mbytes(ptr); 
		}
		
		void operator delete( void* ptr, void* ){
			Allocator::deallocate_mbytes(ptr);
		}

		// if there is an EXCEPTION in new
#if DEBUG_MODE
		void operator delete( void* ptr, int stub, const char* , int , const char*  ){			
			Allocator::deallocate_mbytes(ptr);
		}
#endif

		void operator delete[] ( void* ptr ){
			Allocator::deallocate_mbytes(ptr);
		}

		void operator delete[] ( void* ptr, int stub, const char* , int , const char*  ){
			
			Allocator::deallocate_mbytes(ptr);
		}
	};


	class MemoryPooling
	{
	public:
		static void* allocBytes(size_t count,const char* file, int line, const char* func);
		static void deallocBytes(void* ptr);			
		//static void* allocBytesAligned(size_t align, size_t count, const char* file, int line, const char* func);
		//static void deallocBytesAligned(size_t align, void* ptr);

	};	

    
	template <object_categories Cat> class AllocatedTypes 
        : public MemoryPoolingPolicy{
	};	
	
	typedef AllocatedTypes<ALLOC_COMMON>				MemoryObjectCommon;
	typedef AllocatedTypes<ALLOC_IMAGES>				MemoryObjectImages;
	typedef AllocatedTypes<ALLOC_LOG>					MemoryObjectLog;
	typedef AllocatedTypes<ALLOC_COMP_OBJ_CREATOR>		MemoryObjectCompoundObjectsCreator;
	typedef AllocatedTypes<ALLOC_CANVAS>				MemoryObjectCanvas;
	typedef AllocatedTypes<ALLOC_COMP_OBJ>				MemoryObjectenCompoundObjects;
	typedef AllocatedTypes<ALLOC_BOX_CONTAINER>			MemoryObjectBoxContainer;
	
	typedef AllocatedTypes<ALLOC_GPU_PROCESS>			MemoryObjectenGPUProcess;
	typedef AllocatedTypes<ALLOC_TASK>					MemoryObjectenTask;
	typedef AllocatedTypes<ALLOC_IMAGE_CODEC>			MemoryObjectenImgCodesc;
	typedef AllocatedTypes<ALLOC_COMP_OBJ_FACTORY>		MemoryObjectCompObjFactory;
	typedef AllocatedTypes<ALLOC_IMAGE_FACTORY>			MemoryObjectImageFactory;
	typedef AllocatedTypes<ALLOC_BUFF_FACTORY>			MemoryObjectBuffFactory;
	typedef AllocatedTypes<ALLOC_BUFF_ACTIONS>			MemoryObjectBuffActionsFactory;
	typedef AllocatedTypes<ALLOC_VAL_MAP_FACTORY>		MemoryObjectValMapFactory;
	typedef AllocatedTypes<ALLOC_VAL_STACK_FACTORY>		MemoryObjectValStackFactory;	
	typedef AllocatedTypes<ALLOC_INTERACT>				MemoryObjectInteract;		
	//graphic engine
	typedef AllocatedTypes<ALLOC_RENDER_OBJ>				MemoryObjectRenderState;
	typedef AllocatedTypes<ALLOC_EFFECT>					MemoryObjectEffect;
	typedef AllocatedTypes<ALLOC_SPATIAL_OBJ>			MemoryObjectSpatialObject;
	typedef AllocatedTypes<ALLOC_BINDABLE_OBJ>			MemoryObjectBindableObject;	
	typedef AllocatedTypes<ALLOC_APPLICATION>		    	MemoryObjectApplication;	
	typedef AllocatedTypes<ALLOC_BOUNDVOLUME>		    	MemoryObjectBoundVolume;		
	typedef AllocatedTypes<ALLOC_WIDGET_OBJ>		    MemoryObjectWidget;
    typedef AllocatedTypes<ALLOC_GLSL_SHADER>		    MemoryObjectGLSLShader;
    typedef AllocatedTypes<ALLOC_MEMOBJ>		        MemObject;

    
    
	
	// QT	
	typedef AllocatedTypes<ALLOC_QTWINDOW>		    MemoryObjectQTWindow;	

	typedef Allocated<MemoryObjectCommon> AllocatedObjectCommon;
	typedef Allocated<MemoryObjectImages> AllocatedObjectImage;
	typedef Allocated<MemoryObjectCanvas> AllocatedObjectCanvas;
	typedef Allocated<MemoryObjectLog> AllocatedObjectLog;
	typedef Allocated<MemoryObjectCompoundObjectsCreator> AllocatedObjectCompoundObjectsCreator;
	typedef Allocated<MemoryObjectenCompoundObjects> AllocatedObjectCompoundObjects;
	typedef Allocated<MemoryObjectBoxContainer> AllocatedObjectBoxContainer;

	typedef Allocated<MemoryObjectenGPUProcess> AllocatedObjectGPUProcess;
	typedef Allocated<MemoryObjectenTask>	   AllocatedObjectTask;	
	typedef Allocated<MemoryObjectenImgCodesc> AllocatedObjectImgCodec;	
	typedef Allocated<MemoryObjectCompObjFactory> AllocatedObjectCompObjFactory;	
	typedef Allocated<MemoryObjectImageFactory> AllocatedImageFactory;	
	typedef Allocated<MemoryObjectBuffFactory> AllocatedBuffFactory;
	typedef Allocated<MemoryObjectBuffActionsFactory> AllocatedBuffActionsFactory;
	typedef Allocated<MemoryObjectValMapFactory> AllocatedValMapFactory;
	typedef Allocated<MemoryObjectValStackFactory> AllocatedValStackFactory;
	typedef Allocated<MemoryObjectInteract> AllocatedValInteract;

	
	//graphic engine
	typedef Allocated<MemoryObjectRenderState> AllocatedObjectRenderState;
	typedef Allocated<MemoryObjectEffect> AllocatedObjectEffect;		
	typedef Allocated<MemoryObjectSpatialObject> AllocatedObjectSpatialObject;	
	typedef Allocated<MemoryObjectBindableObject> AllocatedObjectBindableObject;	
	typedef Allocated<MemoryObjectApplication> AllocatedObjectApplication;
	typedef Allocated<MemoryObjectWidget> AllocatedObjectWidget;
	typedef Allocated<MemoryObjectBoundVolume> AllocatedObjectBoundVolume;
    typedef Allocated<MemoryObjectGLSLShader> AllocatedObjectGLSLShader;   
    

    
	
	
	// for 
	typedef AllocatedObjectImage	AllocatedObjectImage;
	typedef AllocatedObjectCanvas	AllocatedObjectCanvas;
	typedef AllocatedObjectLog	AllocatedObjectLog;
	typedef AllocatedObjectCompoundObjectsCreator	AllocatedObjectCompoundObjectsCreator;
	typedef AllocatedObjectCompoundObjects	AllocatedObjectCompGeometry;
	typedef AllocatedObjectGPUProcess	AllocatedObjectCLPrograms;
	typedef AllocatedObjectCommon	AllocatedObjectCommon;
	typedef AllocatedObjectTask	AllocatedTaskExecutor;	
    typedef Allocated<MemObject> AllocatedMemObject;
	//typedef AllocatedObjectImgCodec	AllocatedObjectImgCodec;

	//QT
	
	typedef MemoryObjectQTWindow	AllocatedQTWindow;	


	template< typename T >
	T* alloc_array( T* ptr, size_t count ){
		for (size_t i = 0; i < count; ++i){
			new ( (void*)(ptr+i) ) T();
		}
		return ptr;
	}

 #if DEBUG_MODE
 		
	 #define MALLOC_RAW(mbytes) MemoryPoolingPolicy::allocate_mbytes(mbytes, __FILE__, __LINE__, __FUNCTION__)
 		// for primitive types
	 #define ALLOC_T(T, count) static_cast<T*>(MemoryPoolingPolicy::allocate_mbytes(sizeof(T)*(count), __FILE__, __LINE__, __FUNCTION__)) 		
	 #define FREE_T(ptr) MemoryPoolingPolicy::deallocate_mbytes((void*)ptr)

	 #define NEW_T(T) new (MemoryPoolingPolicy::allocate_mbytes(sizeof(T), __FILE__, __LINE__, __FUNCTION__)) T	 	 	
	 #define DELETE_T(ptr, T) if(ptr){(ptr)->~T(); MemoryPoolingPolicy::deallocate_mbytes((void*)ptr);}

	 #define NEW_ARRAY_T(T, count) alloc_array(static_cast<T*>(MemoryPoolingPolicy::allocate_mbytes(sizeof(T)*(count), __FILE__, __LINE__, __FUNCTION__)), count) 	
	 #define DELETE_ARRAY_T(ptr, T, count) if(ptr){for (size_t b = 0; b < count; ++b) { (ptr)[b].~T();}  MemoryPoolingPolicy::deallocate_mbytes((void*)ptr);}

		// if deriving 
	 #define M_NEW new (0, __FILE__, __LINE__, __FUNCTION__)
	 #define M_DELETE delete 
	
 #else  // release
	
	 #define MALLOC_RAW(mbytes) MemoryPoolingPolicy::allocate_mbytes(mbytes)
	 #define ALLOC_T(T, count) static_cast<T*>(MemoryPoolingPolicy::allocate_mbytes(sizeof(T)*(count) ))
	 #define FREE_T(ptr) MemoryPoolingPolicy::deallocate_mbytes((void*)ptr)

	 

	 #define NEW_T(T) new (MemoryPoolingPolicy::allocate_mbytes(sizeof(T))) T
	 #define DELETE_T(ptr, T) if(ptr){(ptr)->~T(); MemoryPoolingPolicy::deallocate_mbytes((void*)ptr);}

	 #define NEW_ARRAY_T(T, count) alloc_array(static_cast<T*>(MemoryPoolingPolicy::allocate_mbytes(sizeof(T)*(count))), count) 
	 #define DELETE_ARRAY_T(ptr, T, count) if(ptr){for (size_t b = 0; b < count; ++b) { (ptr)[b].~T();}  MemoryPoolingPolicy::deallocate_mbytes((void*)ptr);}

	 // for successors from   Allocated types
	 #define M_NEW new 
	 // for successors from   Allocated types
	 #define M_DELETE delete

#endif

    template<typename T>
    struct TintaDefDeleter {
        void operator()(T* ptr) { DELETE_T(ptr, T); }
    };

    template<typename T>
    struct TintaVirtDeleter {
        void operator()(T* ptr) { M_DELETE(ptr); }
    };

    template<typename T, const size_t size_ >
    struct TintaArrayDeleter {
        void operator()(T* ptr) {
            DELETE_ARRAY_T(ptr, T, size_);
        }       
    };

    template<typename T>
    struct TintaAllocDeleter {
        void operator()(T* ptr) {
            FREE_T(ptr);
        }
    };

    

    #define UNIQPTRALLOC(T, v, ptr)  std::unique_ptr<T,TintaAllocDeleter<T>> v( ptr )
    #define UNIQPTRALLOC_T(T)  std::unique_ptr<T,TintaAllocDeleter<T>>
    // value
    #define UNIQPTRDEF(T,v, ptr)  std::unique_ptr<T,TintaDefDeleter<T>> v( ptr )
    // type
    #define UNIQPTRDEF_T(T)  std::unique_ptr<T,TintaDefDeleter<T>>
    // value
    // UNIQPTRDEF(int, v,NEW_T(int)(10));    
    #define UNIQPTRVIRT(T,v,ptr) std::unique_ptr<T,TintaVirtDeleter<T>> v( ptr )    
    // type
    #define UNIQPTRVIRT_T(T) std::unique_ptr<T,TintaVirtDeleter<T>>
    // value
    // UNIQPTRVIRT(int, v,M_NEW());
    #define UNIQPTRARRAY(T,v,ptr,count) std::unique_ptr<T,TintaArrayDeleter<T,count>> v( ptr );   
    // UNIQPTRARRAY(int, v, NEW_ARRAY_T(int,10), 10 );

    #define SHAREDPTRDEF(T,v, ptr)  std::shared_ptr<T> v( ptr,TintaDefDeleter<T>() )
    #define SHAREDPTR(T)  std::shared_ptr<T>
    #define SHAREDPTRVIRT(T,v, ptr)  std::shared_ptr<T> v( ptr,TintaVirtDeleter<T>() )
    #define SHAREDPTRDEF_RESET(T,v,ptr) v.reset( ptr, TintaDefDeleter<T>());
    #define SHAREDPTRVIRT_RESET(T,v,ptr) v.reset( ptr, TintaVirtDeleter<T>()); 


}

#endif
