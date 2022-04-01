/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_GPU_EXT_H_
#define _TINTA_GPU_EXT_H_

#include <tintaSingleton.h>
#include "tintaException.h"
#include "tintaPredefine.h"
#include "tintaCLConfig.h"
#include "tintaMemoryAllocator.h"

namespace Tinta {

    /*
        GPU OpenCL  capabilities and extensions
    */
	class _CoreExport tintaGPUExt
		: public Singleton< tintaGPUExt > 
	{
	public:

		typedef struct GPUExtData {
			enum enType {
				enExText,
				enExDouble,
				enExBool,
				enExInt,
				enExUInt,
				enExULong,
				enExSizeT,                
                enExSizeT3, // array
				enExOther				
			};

			GPUExtData( const String &dataText, enType type )
				:mDataText( dataText )
				,mType( type ){}

			GPUExtData( ):				
				mType( enExInt ){}
			
			GPUExtData& operator=( const GPUExtData &rV ){
				if ( this == &rV )
					return *this;
				mDataText = rV.mDataText;
				mType	  = rV.mType;
				return *this;
			}
				// 			return *this;
			// data type
			enType mType;
			// parameter description
			String mDataText; 		

		} GPUExtData_t;

		enum GPUPlatInform {
			GPUPlatProfile = 0, //CL_PLATFORM_PROFILE 0x0900
			GPUPlatVersion, //CL_PLATFORM_VERSION 0x0901
			GPUPlatVersionName, //CL_PLATFORM_NAME 0x0902
			GPUPlatVersionVendor, //CL_PLATFORM_VENDOR 0x0903
			GPUPlatExten, //CL_PLATFORM_EXTENSIONS 0x0904
			GPUPlatAll = 5

		};
		enum GPUDevInform {

		GPUDevType = 0, // 				#define CL_DEVICE_TYPE                              0x1000
		GPUDevVendorId = 1, // 				#define CL_DEVICE_VENDOR_ID                         0x1001
		GPUDevMaxCaptureUnits= 2,// 				#define CL_DEVICE_MAX_COMPUTE_UNITS                 0x1002
		GPUDevMaxWorkItemsDimentions= 3,// 				#define CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS          0x1003
		GPUDevMaxWorkGroupSize= 4,// 				#define CL_DEVICE_MAX_WORK_GROUP_SIZE               0x1004
		GPUDevMaxWorkItemSizes= 5,// 				#define CL_DEVICE_MAX_WORK_ITEM_SIZES               0x1005
		GPUDevMaxRefVecWidthChar=6,// 				#define CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR       0x1006
		GPUDevMaxRefVecWidthShort= 7,// 				#define CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT      0x1007
		GPUDevMaxRefVecWidthInt= 8,// 				#define CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT        0x1008
		GPUDevMaxRefVecWidthLong= 9,	// 				#define CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG       0x1009
		GPUDevMaxRefVecWidthFloat= 10,// 				#define CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT      0x100A
		GPUDevMaxRefVecWidthDouble= 11,// 				#define CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE     0x100B
		GPUDevMaxClockFrequency= 12,// 				#define CL_DEVICE_MAX_CLOCK_FREQUENCY               0x100C
		GPUDevMaxArdessBits= 13,// 				#define CL_DEVICE_ADDRESS_BITS                      0x100D
		GPUDevMaxReadImgArgs= 14,// 				#define CL_DEVICE_MAX_READ_IMAGE_ARGS               0x100E
		GPUDevMaxWriteImgArgs= 15,// 				#define CL_DEVICE_MAX_WRITE_IMAGE_ARGS              0x100F
		GPUDevMaxMemAllocSize= 16,// 				#define CL_DEVICE_MAX_MEM_ALLOC_SIZE                0x1010
		GPUDevMaxImage2dWidth= 17,// 				#define CL_DEVICE_IMAGE2D_MAX_WIDTH                 0x1011
		GPUDevMaxImage2dHeight= 18,// 				#define CL_DEVICE_IMAGE2D_MAX_HEIGHT                0x1012
		GPUDevMaxImage3dWidth= 19,// 				#define CL_DEVICE_IMAGE3D_MAX_WIDTH                 0x1013
		GPUDevMaxImage3dHeight= 20,// 				#define CL_DEVICE_IMAGE3D_MAX_HEIGHT                0x1014
		GPUDevMaxImage3dMaxDepth= 21,// 				#define CL_DEVICE_IMAGE3D_MAX_DEPTH                 0x1015
		GPUDevImageSupport= 22,// 					#define CL_DEVICE_IMAGE_SUPPORT                     0x1016
		GPUDevMaxParameterSize= 23,// 				#define CL_DEVICE_MAX_PARAMETER_SIZE                0x1017
		GPUDevMaxSamplers= 24,// 				#define CL_DEVICE_MAX_SAMPLERS                      0x1018
		GPUDevMemBaseAdrAlign= 25,// 				#define CL_DEVICE_MEM_BASE_ADDR_ALIGN               0x1019
		GPUDevMinDataAlignSize= 26,// 				#define CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE          0x101A
		GPUDevSingleFPConfig= 27,// 				#define CL_DEVICE_SINGLE_FP_CONFIG                  0x101B
		GPUDevGlobMemCacheType= 28,// 				#define CL_DEVICE_GLOBAL_MEM_CACHE_TYPE             0x101C
		GPUDevGlobMemCacheLineSize= 29,// 				#define CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE         0x101D
		GPUDevGlobMemCacheSize= 30,// 				#define CL_DEVICE_GLOBAL_MEM_CACHE_SIZE             0x101E
		GPUDevGlobMemSize= 31,// 				#define CL_DEVICE_GLOBAL_MEM_SIZE                   0x101F
		GPUDevMaxConstBuffSize= 32,// 				#define CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE          0x1020
		GPUDevMaxConstBuffArgs= 33,// 				#define CL_DEVICE_MAX_CONSTANT_ARGS                 0x1021
		GPUDevLocalMemType= 34,// 				#define CL_DEVICE_LOCAL_MEM_TYPE                    0x1022
		GPUDevLocalMemSize= 35,// 				#define CL_DEVICE_LOCAL_MEM_SIZE                    0x1023
		GPUDevErrorCorrectionSupport= 36,// 				#define CL_DEVICE_ERROR_CORRECTION_SUPPORT          0x1024
		GPUDevProfilingTimerResolution= 37,// 				#define CL_DEVICE_PROFILING_TIMER_RESOLUTION        0x1025
		GPUDevEndianLittle= 38,// 				#define CL_DEVICE_ENDIAN_LITTLE                     0x1026
		GPUDevDeviceAvailable= 39,// 				#define CL_DEVICE_AVAILABLE                         0x1027
		GPUDevCompilerAvailable= 40,// 				#define CL_DEVICE_COMPILER_AVAILABLE                0x1028
		GPUDevExecutionCapabilites= 41,// 				#define CL_DEVICE_EXECUTION_CAPABILITIES            0x1029
		GPUDevQueueProperties= 42,// 				#define CL_DEVICE_QUEUE_PROPERTIES                  0x102A
		GPUDevName= 43,// 				#define CL_DEVICE_NAME                              0x102B
		GPUDevVendor= 44,// 				#define CL_DEVICE_VENDOR                            0x102C
		GPUDevDriverVersion= 45,// 				#define CL_DRIVER_VERSION                           0x102D
		GPUDevProfile= 46,// 				#define CL_DEVICE_PROFILE                           0x102E
		GPUDevVersion= 47,// 				#define CL_DEVICE_VERSION                           0x102F
		GPUDevExten= 48,// 				#define CL_DEVICE_EXTENSIONS                        0x1030
		GPUDevPlatform= 49,// 				#define CL_DEVICE_PLATFORM                          0x1031		
		GPUDevPreferredVecWidthHalf= 50,// 				#define CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF       0x1034
		GPUDevHostUnfiedMemory= 51,// 				#define CL_DEVICE_HOST_UNIFIED_MEMORY               0x1035
		GPUDevNativeVecWidthChar= 52,// 				#define CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR          0x1036
		GPUDevNativeVecWidthShort= 53,// 				#define CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT         0x1037
		GPUDevNativeVecWidthInt= 54,// 				#define CL_DEVICE_NATIVE_VECTOR_WIDTH_INT           0x1038
		GPUDevNativeVecWidthLong= 55,// 				#define CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG          0x1039
		GPUDevNativeVecWidthFloat= 56,// 				#define CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT         0x103A
		GPUDevNativeVecWidthDouble= 57,// 				#define CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE        0x103B
		GPUDevNativeVecWidthHalf= 58,// 				#define CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF          0x103C
		GPUDevCLVersion= 59,	// 				#define CL_DEVICE_OPENCL_C_VERSION                  0x103D
        // work group info
        GPUDevWorkGroupSize = 60,//                 #define CL_KERNEL_WORK_GROUP_SIZE                   0x11B0
        GPUDevWorkCompileGroupSize = 61,//          #define CL_KERNEL_COMPILE_WORK_GROUP_SIZE           0x11B1
        GPUDevWorkLocalMemSize = 62,//              #define CL_KERNEL_LOCAL_MEM_SIZE                    0x11B2
        GPUDevWorkPreferedWorkGroupSize = 63,//     #define CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE 0x11B3
        GPUDevWorkPrivateMemSize = 64,//            #define CL_KERNEL_PRIVATE_MEM_SIZE                  0x11B4
        
		GPUDevAll = 65		// all data
		};
        		
		static tintaGPUExt* getPtr( void )	{
            CoreAssert(mPtr, "tintaGPUExt mPtr");
			return mPtr;
		}

		tintaGPUExt(void);
		virtual ~tintaGPUExt(void);

		String getPlatformInfo( );

		String getDeviceInfo( m_uint32 platformId, m_uint32 *deviceId = NULL_M, void* kernel = NULL_M );

		m_ulong32 getPlatformsIDs( )const;       

		/*
            returns data type for the device property
        */
		GPUExtData::enType	  getDevDataType( GPUDevInform data )const;

        String getError();

	private:
		/*
            containing text information for platform 
        */
		std::vector<String> mPlatformInfoText;		

		/*  
            containing text information for devices 		
        */
		std::vector<GPUExtData> mDevInfoText;

        mutable String mLastError;

		String	  preparePlatTag( GPUPlatInform data )const;

		String	  prepareDeviceTag( GPUDevInform data )const;		

		m_ulong32 getPlatInfoCLField( GPUPlatInform field )const;		

		m_ulong32 getDeviceInfoCLField( GPUDevInform field )const;

        static const char_m *str_gpu_plat_profile;// = _M("Platform profile: ");
        static const char_m *str_gpu_plat_version;// = _M("Platform version: ");
        static const char_m *str_gpu_plat_name;// = _M("Platform name: ");
        static const char_m *str_gpu_plat_vendor;// = _M("Platform vendor: ");
        static const char_m *str_gpu_plat_extensions;// = _M("Platform extensions: ");

        // text tags for device information
        static const char_m *str_gpu_device_type;// = _M("Device type: ");			//			#define CL_DEVICE_TYPE                              0x1000
        static const char_m *str_gpu_device_vendor_id;// = _M("Device vendor_id: ");	 // 				#define CL_DEVICE_VENDOR_ID                         0x1001
        static const char_m *str_gpu_device_max_compute_units;// = _M("Device max compute_units: ");	// 				#define CL_DEVICE_MAX_COMPUTE_UNITS                 0x1002
        static const char_m *str_gpu_device_max_work_item_dimentions;// = _M("Device max_work item_dimentions: ");	// 				#define CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS          0x1003
        static const char_m *str_gpu_device_max_work_group_size;// = _M("Device max work_group size: ");	// 				#define CL_DEVICE_MAX_WORK_GROUP_SIZE               0x1004
        static const char_m *str_gpu_device_max_work_items_sizes;// = _M("Device max work items sizes: ");	// 				#define CL_DEVICE_MAX_WORK_ITEM_SIZES               0x1005
        static const char_m *str_gpu_device_prereferred_vector_width_char;// = _M("Device prereferred vector width char: ");	// 				#define CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR       0x1006
        static const char_m *str_gpu_device_prereferred_vector_width_short;// = _M("Device prereferred vector width short: ");	// 				#define CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT      0x1007
        static const char_m *str_gpu_device_prereferred_vector_width_int;// = _M("Device prereferred vector width int: ");	// 				#define CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT        0x1008
        static const char_m *str_gpu_device_prereferred_vector_width_long;// = _M("Device prereferred vector width long: ");		// 				#define CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG       0x1009
        static const char_m *str_gpu_device_prereferred_vector_width_float;// = _M("Device prereferred vector width float: ");	// 				#define CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT      0x100A
        static const char_m *str_gpu_device_prereferred_vector_width_double;// = _M("Device_prereferred vector width double: ");	// 				#define CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE     0x100B
        static const char_m *str_gpu_device_max_clock_friquiency;// = _M("Device max clock friquiency: ");	// 				#define CL_DEVICE_MAX_CLOCK_FREQUENCY               0x100C
        static const char_m *str_gpu_device_adress_bits;// = _M("Device adress bits: ");	// 				#define CL_DEVICE_ADDRESS_BITS                      0x100D
        static const char_m *str_gpu_device_read_image_args;// = _M("Device rea image args: ");	// 				#define CL_DEVICE_MAX_READ_IMAGE_ARGS               0x100E
        static const char_m *str_gpu_device_max_write_image_args;// = _M("Device max write image_args: ");	// 				#define CL_DEVICE_MAX_WRITE_IMAGE_ARGS              0x100F
        static const char_m *str_gpu_device_max_mem_alloc_size;// = _M("Device max mem alloc_size: ");	// 				#define CL_DEVICE_MAX_MEM_ALLOC_SIZE                0x1010
        static const char_m *str_gpu_device_image2d_max_width;// = _M("Device_image2d max width: ");	// 				#define CL_DEVICE_IMAGE2D_MAX_WIDTH                 0x1011
        static const char_m *str_gpu_device_image2d_max_height;// = _M("Device image2d max height: ");	// 				#define CL_DEVICE_IMAGE2D_MAX_HEIGHT                0x1012
        static const char_m *str_gpu_device_image3d_max_width;// = _M("Device image3d max width: ");	// 				#define CL_DEVICE_IMAGE3D_MAX_WIDTH                 0x1013
        static const char_m *str_gpu_device_image3d_max_height;// = _M("Device image3d max height: ");	// 				#define CL_DEVICE_IMAGE3D_MAX_HEIGHT                0x1014
        static const char_m *str_gpu_device_image3d_max_depth;// = _M("Device image3d max depth: ");	// 				#define CL_DEVICE_IMAGE3D_MAX_DEPTH                 0x1015
        static const char_m *str_gpu_device_image_support;// = _M("Device image support: ");	// 					#define CL_DEVICE_IMAGE_SUPPORT                     0x1016
        static const char_m *str_gpu_device_max_parameter_size;// = _M("Device max parameter size: ");	// 				#define CL_DEVICE_MAX_PARAMETER_SIZE                0x1017
        static const char_m *str_gpu_device_max_samplers;// = _M("Device max samplers: ");	// 				#define CL_DEVICE_MAX_SAMPLERS                      0x1018
        static const char_m *str_gpu_device_mem_base_addr_align;// = _M("Device mem base addr align: ");	// 				#define CL_DEVICE_MEM_BASE_ADDR_ALIGN               0x1019
        static const char_m *str_gpu_device_min_data_type_align_size;// = _M("Device min data type align_size: ");	// 				#define CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE          0x101A
        static const char_m *str_gpu_device_single_fp_config;// = _M("Device single fp config: ");	// 				#define CL_DEVICE_SINGLE_FP_CONFIG                  0x101B
        static const char_m *str_gpu_device_global_mem_cache_type;// = _M("Device global mem cache type: ");	// 				#define CL_DEVICE_GLOBAL_MEM_CACHE_TYPE             0x101C
        static const char_m *str_gpu_device_global_mem_cacheline_size;// = _M("Device global mem cacheline size: ");	// 				#define CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE         0x101D
        static const char_m *str_gpu_device_global_cache_size;// = _M("Device global cache size: ");	// 				#define CL_DEVICE_GLOBAL_MEM_CACHE_SIZE             0x101E
        static const char_m *str_gpu_device_global_mem_size;// = _M("Device global mem_size: ");	// 				#define CL_DEVICE_GLOBAL_MEM_SIZE                   0x101F
        static const char_m *str_gpu_device_max_constant_buffer_size;// = _M("Device max constant buffer size: ");	// 				#define CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE          0x1020
        static const char_m *str_gpu_device_max_constant_args;// = _M("Device max constant args: ");	// 				#define CL_DEVICE_MAX_CONSTANT_ARGS                 0x1021
        static const char_m *str_gpu_device_local_memtype;// = _M("Device local memtype: ");	// 				#define CL_DEVICE_LOCAL_MEM_TYPE                    0x1022
        static const char_m *str_gpu_device_mem_size;// = _M("Device mem size: ");	// 				#define CL_DEVICE_LOCAL_MEM_SIZE                    0x1023
        static const char_m *str_gpu_device_error_correction_support;// = _M("Device_error correction support: ");	// 				#define CL_DEVICE_ERROR_CORRECTION_SUPPORT          0x1024
        static const char_m *str_gpu_device_profiling_times_resolution;// = _M("Device profiling times resolution: ");	// 				#define CL_DEVICE_PROFILING_TIMER_RESOLUTION        0x1025
        static const char_m *str_gpu_device_endian_little;// = _M("Device endian little: ");	// 				#define CL_DEVICE_ENDIAN_LITTLE                     0x1026
        static const char_m *str_gpu_device_device_available;// = _M("Device device available: ");	// 				#define CL_DEVICE_AVAILABLE                         0x1027
        static const char_m *str_gpu_device_compiler_available;// = _M("Device compiler available: ");	// 				#define CL_DEVICE_COMPILER_AVAILABLE                0x1028
        static const char_m *str_gpu_device_execution_capabilities;// = _M("Device execution capabilities: ");	// 				#define CL_DEVICE_EXECUTION_CAPABILITIES            0x1029
        static const char_m *str_gpu_device_queue_properties;// = _M("Device queue properties: ");	// 				#define CL_DEVICE_QUEUE_PROPERTIES                  0x102A
        static const char_m *str_gpu_device_name;// = _M("Device name: ");	// 				#define CL_DEVICE_NAME                              0x102B
        static const char_m *str_gpu_device_vendor;// = _M("Device vendor: ");	// 				#define CL_DEVICE_VENDOR                            0x102C
        static const char_m *str_gpu_device_driver_version;// = _M("Device driver version: ");	// 				#define CL_DRIVER_VERSION                           0x102D
        static const char_m *str_gpu_device_profile;// = _M("Device profile: ");	// 				#define CL_DEVICE_PROFILE                           0x102E
        static const char_m *str_gpu_device_version;// = _M("Device version: ");	// 				#define CL_DEVICE_VERSION                           0x102F
        static const char_m *str_gpu_device_extensions;// = _M("Device extensions: ");	// 				#define CL_DEVICE_EXTENSIONS                        0x1030
        static const char_m *str_gpu_device_platform;// = _M("Device platform: ");	// 				#define CL_DEVICE_PLATFORM                          0x1031
        //GPUDevMaxWorkItemSizes,// 							/* 0x1032 reserved for CL_DEVICE_DOUBLE_FP_CONFIG */
        //GPUDevMaxWorkItemSizes,// 							/* 0x1033 reserved for CL_DEVICE_HALF_FP_CONFIG */
        static const char_m *str_gpu_device_prereferred_vector_width_half;// = _M("Device prereferred vector width half: ");	// 				#define CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF       0x1034
        static const char_m *str_gpu_device_host_unified_memory;// = _M("Device host unified memory: ");	// 				#define CL_DEVICE_HOST_UNIFIED_MEMORY               0x1035
        static const char_m *str_gpu_device_native_vector_width_char;// = _M("Device native vector width char: ");	// 				#define CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR          0x1036
        static const char_m *str_gpu_device_native_vector_width_short;// = _M("Device native vector width short: ");	// 				#define CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT         0x1037
        static const char_m *str_gpu_device_native_vector_width_int;// = _M("Device native vector width int: ");	// 				#define CL_DEVICE_NATIVE_VECTOR_WIDTH_INT           0x1038
        static const char_m *str_gpu_device_native_vector_width_long;// = _M("Device native vector width long: ");	// 				#define CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG          0x1039
        static const char_m *str_gpu_device_native_vector_width_float;// = _M("Device native vector width float: ");	// 				#define CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT         0x103A
        static const char_m *str_gpu_device_native_vector_width_double;// = _M("Device native vector width double: ");	// 				#define CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE        0x103B
        static const char_m *str_gpu_device_native_vector_width_half;// = _M("Device native vector width half: ");	// 				#define CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF          0x103C
        static const char_m *str_gpu_device_opencl_version;// = _M("Device opencl version: ");		// 				#define CL_DEVICE_OPENCL_C_VERSION                  0x103D

        static const char_m *str_gpu_device_work_group_size;    // #define CL_KERNEL_WORK_GROUP_SIZE                   0x11B0
        static const char_m *str_gpu_device_compile_work_group_size;         //#define CL_KERNEL_COMPILE_WORK_GROUP_SIZE           0x11B1
        static const char_m *str_gpu_device_local_mem_size;     //#define CL_KERNEL_LOCAL_MEM_SIZE                    0x11B2
        static const char_m *str_gpu_device_prefered_work_group_size;    // #define CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE 0x11B3
        static const char_m *str_gpu_device_private_mem_size;    //#define CL_KERNEL_PRIVATE_MEM_SIZE                  0x11B4

        static const char_m *str_gpu_plat_text;// = _M("Platform");
        static const char_m *str_gpu_device;// = _M("GPU device");

		

	};

}


#endif