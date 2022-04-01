/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaGPUExt.h"


namespace Tinta {

    const char_m *tintaGPUExt::str_gpu_plat_profile = _M("Platform profile: ");
    const char_m *tintaGPUExt::str_gpu_plat_version = _M("Platform version: ");
    const char_m *tintaGPUExt::str_gpu_plat_name = _M("Platform name: ");
    const char_m *tintaGPUExt::str_gpu_plat_vendor = _M("Platform vendor: ");
    const char_m *tintaGPUExt::str_gpu_plat_extensions = _M("Platform extensions: ");

    // text tags for device information
    const char_m *tintaGPUExt::str_gpu_device_type = _M("Device type: ");			//			#define CL_DEVICE_TYPE                              0x1000
    const char_m *tintaGPUExt::str_gpu_device_vendor_id = _M("Device vendor_id: ");	 // 				#define CL_DEVICE_VENDOR_ID                         0x1001
    const char_m *tintaGPUExt::str_gpu_device_max_compute_units = _M("Device max compute_units: ");	// 				#define CL_DEVICE_MAX_COMPUTE_UNITS                 0x1002
    const char_m *tintaGPUExt::str_gpu_device_max_work_item_dimentions = _M("Device max_work item_dimentions: ");	// 				#define CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS          0x1003
    const char_m *tintaGPUExt::str_gpu_device_max_work_group_size = _M("Device max work_group size: ");	// 				#define CL_DEVICE_MAX_WORK_GROUP_SIZE               0x1004
    const char_m *tintaGPUExt::str_gpu_device_max_work_items_sizes = _M("Device max work items sizes: ");	// 				#define CL_DEVICE_MAX_WORK_ITEM_SIZES               0x1005
    const char_m *tintaGPUExt::str_gpu_device_prereferred_vector_width_char = _M("Device prereferred vector width char: ");	// 				#define CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR       0x1006
    const char_m *tintaGPUExt::str_gpu_device_prereferred_vector_width_short = _M("Device prereferred vector width short: ");	// 				#define CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT      0x1007
    const char_m *tintaGPUExt::str_gpu_device_prereferred_vector_width_int = _M("Device prereferred vector width int: ");	// 				#define CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT        0x1008
    const char_m *tintaGPUExt::str_gpu_device_prereferred_vector_width_long = _M("Device prereferred vector width long: ");		// 				#define CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG       0x1009
    const char_m *tintaGPUExt::str_gpu_device_prereferred_vector_width_float = _M("Device prereferred vector width float: ");	// 				#define CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT      0x100A
    const char_m *tintaGPUExt::str_gpu_device_prereferred_vector_width_double = _M("Device prereferred vector width double: ");	// 				#define CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE     0x100B
    const char_m *tintaGPUExt::str_gpu_device_max_clock_friquiency = _M("Device max clock friquiency: ");	// 				#define CL_DEVICE_MAX_CLOCK_FREQUENCY               0x100C
    const char_m *tintaGPUExt::str_gpu_device_adress_bits = _M("Device adress bits: ");	// 				#define CL_DEVICE_ADDRESS_BITS                      0x100D
    const char_m *tintaGPUExt::str_gpu_device_read_image_args = _M("Device read image args: ");	// 				#define CL_DEVICE_MAX_READ_IMAGE_ARGS               0x100E
    const char_m *tintaGPUExt::str_gpu_device_max_write_image_args = _M("Device max write image_args: ");	// 				#define CL_DEVICE_MAX_WRITE_IMAGE_ARGS              0x100F
    const char_m *tintaGPUExt::str_gpu_device_max_mem_alloc_size = _M("Device max mem alloc_size: ");	// 				#define CL_DEVICE_MAX_MEM_ALLOC_SIZE                0x1010
    const char_m *tintaGPUExt::str_gpu_device_image2d_max_width = _M("Device image2d max width: ");	// 				#define CL_DEVICE_IMAGE2D_MAX_WIDTH                 0x1011
    const char_m *tintaGPUExt::str_gpu_device_image2d_max_height = _M("Device image2d max height: ");	// 				#define CL_DEVICE_IMAGE2D_MAX_HEIGHT                0x1012
    const char_m *tintaGPUExt::str_gpu_device_image3d_max_width = _M("Device image3d max width: ");	// 				#define CL_DEVICE_IMAGE3D_MAX_WIDTH                 0x1013
    const char_m *tintaGPUExt::str_gpu_device_image3d_max_height = _M("Device image3d max height: ");	// 				#define CL_DEVICE_IMAGE3D_MAX_HEIGHT                0x1014
    const char_m *tintaGPUExt::str_gpu_device_image3d_max_depth = _M("Device image3d max depth: ");	// 				#define CL_DEVICE_IMAGE3D_MAX_DEPTH                 0x1015
    const char_m *tintaGPUExt::str_gpu_device_image_support = _M("Device image support: ");	// 					#define CL_DEVICE_IMAGE_SUPPORT                     0x1016
    const char_m *tintaGPUExt::str_gpu_device_max_parameter_size = _M("Device max parameter size: ");	// 				#define CL_DEVICE_MAX_PARAMETER_SIZE                0x1017
    const char_m *tintaGPUExt::str_gpu_device_max_samplers = _M("Device max samplers: ");	// 				#define CL_DEVICE_MAX_SAMPLERS                      0x1018
    const char_m *tintaGPUExt::str_gpu_device_mem_base_addr_align = _M("Device mem base addr align: ");	// 				#define CL_DEVICE_MEM_BASE_ADDR_ALIGN               0x1019
    const char_m *tintaGPUExt::str_gpu_device_min_data_type_align_size = _M("Device min data type align_size: ");	// 				#define CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE          0x101A
    const char_m *tintaGPUExt::str_gpu_device_single_fp_config = _M("Device single fp config: ");	// 				#define CL_DEVICE_SINGLE_FP_CONFIG                  0x101B
    const char_m *tintaGPUExt::str_gpu_device_global_mem_cache_type = _M("Device global mem cache type: ");	// 				#define CL_DEVICE_GLOBAL_MEM_CACHE_TYPE             0x101C
    const char_m *tintaGPUExt::str_gpu_device_global_mem_cacheline_size = _M("Device global mem cacheline size: ");	// 				#define CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE         0x101D
    const char_m *tintaGPUExt::str_gpu_device_global_cache_size = _M("Device global cache size: ");	// 				#define CL_DEVICE_GLOBAL_MEM_CACHE_SIZE             0x101E
    const char_m *tintaGPUExt::str_gpu_device_global_mem_size = _M("Device global mem_size: ");	// 				#define CL_DEVICE_GLOBAL_MEM_SIZE                   0x101F
    const char_m *tintaGPUExt::str_gpu_device_max_constant_buffer_size = _M("Device max constant buffer size: ");	// 				#define CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE          0x1020
    const char_m *tintaGPUExt::str_gpu_device_max_constant_args = _M("Device max constant args: ");	// 				#define CL_DEVICE_MAX_CONSTANT_ARGS                 0x1021
    const char_m *tintaGPUExt::str_gpu_device_local_memtype = _M("Device local memtype: ");	// 				#define CL_DEVICE_LOCAL_MEM_TYPE                    0x1022
    const char_m *tintaGPUExt::str_gpu_device_mem_size = _M("Device mem size: ");	// 				#define CL_DEVICE_LOCAL_MEM_SIZE                    0x1023
    const char_m *tintaGPUExt::str_gpu_device_error_correction_support = _M("Device error correction support: ");	// 				#define CL_DEVICE_ERROR_CORRECTION_SUPPORT          0x1024
    const char_m *tintaGPUExt::str_gpu_device_profiling_times_resolution = _M("Device profiling times resolution: ");	// 				#define CL_DEVICE_PROFILING_TIMER_RESOLUTION        0x1025
    const char_m *tintaGPUExt::str_gpu_device_endian_little = _M("Device endian little: ");	// 				#define CL_DEVICE_ENDIAN_LITTLE                     0x1026
    const char_m *tintaGPUExt::str_gpu_device_device_available = _M("Device device available: ");	// 				#define CL_DEVICE_AVAILABLE                         0x1027
    const char_m *tintaGPUExt::str_gpu_device_compiler_available = _M("Device compiler available: ");	// 				#define CL_DEVICE_COMPILER_AVAILABLE                0x1028
    const char_m *tintaGPUExt::str_gpu_device_execution_capabilities = _M("Device execution capabilities: ");	// 				#define CL_DEVICE_EXECUTION_CAPABILITIES            0x1029
    const char_m *tintaGPUExt::str_gpu_device_queue_properties = _M("Device queue properties: ");	// 				#define CL_DEVICE_QUEUE_PROPERTIES                  0x102A
    const char_m *tintaGPUExt::str_gpu_device_name = _M("Device name: ");	// 				#define CL_DEVICE_NAME                              0x102B
    const char_m *tintaGPUExt::str_gpu_device_vendor = _M("Device vendor: ");	// 				#define CL_DEVICE_VENDOR                            0x102C
    const char_m *tintaGPUExt::str_gpu_device_driver_version = _M("Device driver version: ");	// 				#define CL_DRIVER_VERSION                           0x102D
    const char_m *tintaGPUExt::str_gpu_device_profile = _M("Device profile: ");	// 				#define CL_DEVICE_PROFILE                           0x102E
    const char_m *tintaGPUExt::str_gpu_device_version = _M("Device version: ");	// 				#define CL_DEVICE_VERSION                           0x102F
    const char_m *tintaGPUExt::str_gpu_device_extensions = _M("Device extensions: ");	// 				#define CL_DEVICE_EXTENSIONS                        0x1030
    const char_m *tintaGPUExt::str_gpu_device_platform = _M("Device platform: ");	// 				#define CL_DEVICE_PLATFORM                          0x1031
    //GPUDevMaxWorkItemSizes,// 							/* 0x1032 reserved for CL_DEVICE_DOUBLE_FP_CONFIG */
    //GPUDevMaxWorkItemSizes,// 							/* 0x1033 reserved for CL_DEVICE_HALF_FP_CONFIG */
    const char_m *tintaGPUExt::str_gpu_device_prereferred_vector_width_half = _M("Device prereferred vector width half: ");	// 				#define CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF       0x1034
    const char_m *tintaGPUExt::str_gpu_device_host_unified_memory = _M("Device host unified memory: ");	// 				#define CL_DEVICE_HOST_UNIFIED_MEMORY               0x1035
    const char_m *tintaGPUExt::str_gpu_device_native_vector_width_char = _M("Device native vector width char: ");	// 				#define CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR          0x1036
    const char_m *tintaGPUExt::str_gpu_device_native_vector_width_short = _M("Device native vector width short: ");	// 				#define CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT         0x1037
    const char_m *tintaGPUExt::str_gpu_device_native_vector_width_int = _M("Device native vector width int: ");	// 				#define CL_DEVICE_NATIVE_VECTOR_WIDTH_INT           0x1038
    const char_m *tintaGPUExt::str_gpu_device_native_vector_width_long = _M("Device native vector width long: ");	// 				#define CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG          0x1039
    const char_m *tintaGPUExt::str_gpu_device_native_vector_width_float = _M("Device native vector width float: ");	// 				#define CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT         0x103A
    const char_m *tintaGPUExt::str_gpu_device_native_vector_width_double = _M("Device native vector width double: ");	// 				#define CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE        0x103B
    const char_m *tintaGPUExt::str_gpu_device_native_vector_width_half = _M("Device native vector width half: ");	// 				#define CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF          0x103C
    const char_m *tintaGPUExt::str_gpu_device_opencl_version = _M("Device opencl version: ");		// 				#define CL_DEVICE_OPENCL_C_VERSION                  0x103D
    const char_m *tintaGPUExt::str_gpu_device_work_group_size = _M("Device work group size: ");    // #define CL_KERNEL_WORK_GROUP_SIZE                   0x11B0
    const char_m *tintaGPUExt::str_gpu_device_compile_work_group_size = _M("Device compile work group size: ");         //#define CL_KERNEL_COMPILE_WORK_GROUP_SIZE           0x11B1
    const char_m *tintaGPUExt::str_gpu_device_local_mem_size = _M("Device local mem size: ");     //#define CL_KERNEL_LOCAL_MEM_SIZE                    0x11B2
    const char_m *tintaGPUExt::str_gpu_device_prefered_work_group_size = _M("Device prefered work group size: ");    // #define CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE 0x11B3
    const char_m *tintaGPUExt::str_gpu_device_private_mem_size = _M("Device private mem size: ");    //#define CL_KERNEL_PRIVATE_MEM_SIZE                  0x11B4

    const char_m *tintaGPUExt::str_gpu_plat_text = _M("Platform");
    const char_m *tintaGPUExt::str_gpu_device = _M("GPU device");

tintaGPUExt::tintaGPUExt(void)
{
	mPlatformInfoText.resize( GPUPlatAll );
	mPlatformInfoText[GPUPlatProfile] = tintaGPUExt::str_gpu_plat_profile;
	mPlatformInfoText[GPUPlatVersion] = tintaGPUExt::str_gpu_plat_version;
	mPlatformInfoText[GPUPlatVersionName] = tintaGPUExt::str_gpu_plat_name;
	mPlatformInfoText[GPUPlatVersionVendor] = tintaGPUExt::str_gpu_plat_vendor;
	mPlatformInfoText[GPUPlatExten] = tintaGPUExt::str_gpu_plat_extensions;

	mDevInfoText.resize( GPUDevAll );
	mDevInfoText[GPUDevType]  = GPUExtData(tintaGPUExt::str_gpu_device_type,GPUExtData::enExOther );//("Device type: ");			//			#define CL_DEVICE_TYPE                              0x1000
	mDevInfoText[GPUDevVendorId]  = GPUExtData(tintaGPUExt::str_gpu_device_vendor_id,GPUExtData::enExSizeT);//("Device vendor_id: ");	 // 				#define CL_DEVICE_VENDOR_ID                         0x1001
	mDevInfoText[GPUDevMaxCaptureUnits]  = GPUExtData(tintaGPUExt::str_gpu_device_max_compute_units,GPUExtData::enExUInt);//("Device max compute_units: ");	// 				#define CL_DEVICE_MAX_COMPUTE_UNITS                 0x1002
	mDevInfoText[GPUDevMaxWorkItemsDimentions]  = GPUExtData(tintaGPUExt::str_gpu_device_max_work_item_dimentions,GPUExtData::enExSizeT);//("Device max_work item_dimentions: ");	// 				#define CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS          0x1003
	mDevInfoText[GPUDevMaxWorkGroupSize]  = GPUExtData(tintaGPUExt::str_gpu_device_max_work_group_size,GPUExtData::enExSizeT);//("Device max work_group size: ");	// 				#define CL_DEVICE_MAX_WORK_GROUP_SIZE               0x1004
	mDevInfoText[GPUDevMaxWorkItemSizes]  = GPUExtData(tintaGPUExt::str_gpu_device_max_work_items_sizes,GPUExtData::enExOther);//("Device max work items sizes: ");	// 				#define CL_DEVICE_MAX_WORK_ITEM_SIZES               0x1005
	mDevInfoText[GPUDevMaxRefVecWidthChar]  = GPUExtData(tintaGPUExt::str_gpu_device_prereferred_vector_width_char,GPUExtData::enExSizeT);//("Device prereferred vector width char: ");	// 				#define CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR       0x1006
	mDevInfoText[GPUDevMaxRefVecWidthShort]  = GPUExtData(tintaGPUExt::str_gpu_device_prereferred_vector_width_short,GPUExtData::enExSizeT);//("Device prereferred vector width short: ");	// 				#define CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT      0x1007
	mDevInfoText[GPUDevMaxRefVecWidthInt]  = GPUExtData(tintaGPUExt::str_gpu_device_prereferred_vector_width_int,GPUExtData::enExSizeT);//("Device prereferred vector width int: ");	// 				#define CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT        0x1008
	mDevInfoText[GPUDevMaxRefVecWidthLong]  = GPUExtData(tintaGPUExt::str_gpu_device_prereferred_vector_width_long,GPUExtData::enExSizeT);//("Device prereferred vector width long: ");		// 				#define CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG       0x1009
	mDevInfoText[GPUDevMaxRefVecWidthFloat]  = GPUExtData(tintaGPUExt::str_gpu_device_prereferred_vector_width_float,GPUExtData::enExSizeT);//("Device prereferred vector width float: ");	// 				#define CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT      0x100A
	mDevInfoText[GPUDevMaxRefVecWidthDouble]  = GPUExtData(tintaGPUExt::str_gpu_device_prereferred_vector_width_double,GPUExtData::enExSizeT);//("Device_prereferred vector width double: ");	// 				#define CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE     0x100B
	mDevInfoText[GPUDevMaxClockFrequency]  = GPUExtData(tintaGPUExt::str_gpu_device_max_clock_friquiency,GPUExtData::enExUInt);//("Device max clock friquiency: ");	// 				#define CL_DEVICE_MAX_CLOCK_FREQUENCY               0x100C
	mDevInfoText[GPUDevMaxArdessBits]  = GPUExtData(tintaGPUExt::str_gpu_device_adress_bits,GPUExtData::enExSizeT);//("Device adress bits: ");	// 				#define CL_DEVICE_ADDRESS_BITS                      0x100D
	mDevInfoText[GPUDevMaxReadImgArgs]  = GPUExtData(tintaGPUExt::str_gpu_device_read_image_args,GPUExtData::enExSizeT);//("Device rea image args: ");	// 				#define CL_DEVICE_MAX_READ_IMAGE_ARGS               0x100E
	mDevInfoText[GPUDevMaxWriteImgArgs]  = GPUExtData(tintaGPUExt::str_gpu_device_max_write_image_args,GPUExtData::enExSizeT);//("Device max write image_args: ");	// 				#define CL_DEVICE_MAX_WRITE_IMAGE_ARGS              0x100F
	mDevInfoText[GPUDevMaxMemAllocSize]  = GPUExtData(tintaGPUExt::str_gpu_device_max_mem_alloc_size,GPUExtData::enExSizeT);//("Device max mem alloc_size: ");	// 				#define CL_DEVICE_MAX_MEM_ALLOC_SIZE                0x1010
	mDevInfoText[GPUDevMaxImage2dWidth]  = GPUExtData(tintaGPUExt::str_gpu_device_image2d_max_width,GPUExtData::enExSizeT);//("Device_image2d max width: ");	// 				#define CL_DEVICE_IMAGE2D_MAX_WIDTH                 0x1011
	mDevInfoText[GPUDevMaxImage2dHeight]  = GPUExtData(tintaGPUExt::str_gpu_device_image2d_max_height,GPUExtData::enExSizeT);//("Device image2d max height: ");	// 				#define CL_DEVICE_IMAGE2D_MAX_HEIGHT                0x1012
	mDevInfoText[GPUDevMaxImage3dWidth]  = GPUExtData(tintaGPUExt::str_gpu_device_image3d_max_width,GPUExtData::enExSizeT);//("Device image3d max width: ");	// 				#define CL_DEVICE_IMAGE3D_MAX_WIDTH                 0x1013
	mDevInfoText[GPUDevMaxImage3dHeight]  = GPUExtData(tintaGPUExt::str_gpu_device_image3d_max_height,GPUExtData::enExSizeT);//("Device image3d max height: ");	// 				#define CL_DEVICE_IMAGE3D_MAX_HEIGHT                0x1014
	mDevInfoText[GPUDevMaxImage3dMaxDepth]  = GPUExtData(tintaGPUExt::str_gpu_device_image3d_max_depth,GPUExtData::enExSizeT);//("Device image3d max depth: ");	// 				#define CL_DEVICE_IMAGE3D_MAX_DEPTH                 0x1015
	mDevInfoText[GPUDevImageSupport]  = GPUExtData(tintaGPUExt::str_gpu_device_image_support,GPUExtData::enExBool); // _M("Device image support: ");	// 					#define CL_DEVICE_IMAGE_SUPPORT                     0x1016
	mDevInfoText[GPUDevMaxParameterSize]  = GPUExtData(tintaGPUExt::str_gpu_device_max_parameter_size,GPUExtData::enExSizeT);//("Device max parameter size: ");	// 				#define CL_DEVICE_MAX_PARAMETER_SIZE                0x1017
	mDevInfoText[GPUDevMaxSamplers]  = GPUExtData(tintaGPUExt::str_gpu_device_max_samplers,GPUExtData::enExSizeT);//("Device max samplers: ");	// 				#define CL_DEVICE_MAX_SAMPLERS                      0x1018
	mDevInfoText[GPUDevMemBaseAdrAlign]  = GPUExtData(tintaGPUExt::str_gpu_device_mem_base_addr_align,GPUExtData::enExSizeT);//("Device mem base addr align: ");	// 				#define CL_DEVICE_MEM_BASE_ADDR_ALIGN               0x1019
	mDevInfoText[GPUDevMinDataAlignSize]  = GPUExtData(tintaGPUExt::str_gpu_device_min_data_type_align_size,GPUExtData::enExSizeT);//("Device min data type align_size: ");	// 				#define CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE          0x101A
	mDevInfoText[GPUDevSingleFPConfig]  = GPUExtData(tintaGPUExt::str_gpu_device_single_fp_config,GPUExtData::enExSizeT);//("Device_single fp config: ");	// 				#define CL_DEVICE_SINGLE_FP_CONFIG                  0x101B
	mDevInfoText[GPUDevGlobMemCacheType]  = GPUExtData(tintaGPUExt::str_gpu_device_global_mem_cache_type,GPUExtData::enExSizeT);//("Device global mem cache type: ");	// 				#define CL_DEVICE_GLOBAL_MEM_CACHE_TYPE             0x101C
	mDevInfoText[GPUDevGlobMemCacheLineSize]  = GPUExtData(tintaGPUExt::str_gpu_device_global_mem_cacheline_size,GPUExtData::enExSizeT);//("Device global mem cacheline size: ");	// 				#define CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE         0x101D
	mDevInfoText[GPUDevGlobMemCacheSize]  = GPUExtData(tintaGPUExt::str_gpu_device_global_cache_size,GPUExtData::enExSizeT);//("Device global cache size: ");	// 				#define CL_DEVICE_GLOBAL_MEM_CACHE_SIZE             0x101E
	mDevInfoText[GPUDevGlobMemSize]  = GPUExtData(tintaGPUExt::str_gpu_device_global_mem_size,GPUExtData::enExSizeT);//("Device global mem_size: ");	// 				#define CL_DEVICE_GLOBAL_MEM_SIZE                   0x101F
	mDevInfoText[GPUDevMaxConstBuffSize]  = GPUExtData(tintaGPUExt::str_gpu_device_max_constant_buffer_size,GPUExtData::enExSizeT);//("Device max constant buffer size: ");	// 				#define CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE          0x1020
	mDevInfoText[GPUDevMaxConstBuffArgs]  = GPUExtData(tintaGPUExt::str_gpu_device_max_constant_args,GPUExtData::enExSizeT);//("Device max constant args: ");	// 				#define CL_DEVICE_MAX_CONSTANT_ARGS                 0x1021
	mDevInfoText[GPUDevLocalMemType]  = GPUExtData(tintaGPUExt::str_gpu_device_local_memtype,GPUExtData::enExSizeT);//("Device local memtype: ");	// 				#define CL_DEVICE_LOCAL_MEM_TYPE                    0x1022
	mDevInfoText[GPUDevLocalMemSize]  = GPUExtData(tintaGPUExt::str_gpu_device_mem_size,GPUExtData::enExSizeT);//("Device mem size: ");	// 				#define CL_DEVICE_LOCAL_MEM_SIZE                    0x1023
	mDevInfoText[GPUDevErrorCorrectionSupport]  = GPUExtData(tintaGPUExt::str_gpu_device_error_correction_support,GPUExtData::enExSizeT);//("Device_error correction support: ");	// 				#define CL_DEVICE_ERROR_CORRECTION_SUPPORT          0x1024
	mDevInfoText[GPUDevProfilingTimerResolution]  = GPUExtData(tintaGPUExt::str_gpu_device_profiling_times_resolution,GPUExtData::enExSizeT);//("Device profiling times resolution: ");	// 				#define CL_DEVICE_PROFILING_TIMER_RESOLUTION        0x1025
	mDevInfoText[GPUDevEndianLittle]  = GPUExtData(tintaGPUExt::str_gpu_device_endian_little,GPUExtData::enExSizeT);//("Device endian little: ");	// 				#define CL_DEVICE_ENDIAN_LITTLE                     0x1026
	mDevInfoText[GPUDevDeviceAvailable]  = GPUExtData(tintaGPUExt::str_gpu_device_device_available,GPUExtData::enExSizeT);//("Device device available: ");	// 				#define CL_DEVICE_AVAILABLE                         0x1027
	mDevInfoText[GPUDevCompilerAvailable]  = GPUExtData(tintaGPUExt::str_gpu_device_compiler_available,GPUExtData::enExSizeT);//("Device compiler available: ");	// 				#define CL_DEVICE_COMPILER_AVAILABLE                0x1028
	mDevInfoText[GPUDevExecutionCapabilites]  = GPUExtData(tintaGPUExt::str_gpu_device_execution_capabilities,GPUExtData::enExSizeT);//("Device execution capabilities: ");	// 				#define CL_DEVICE_EXECUTION_CAPABILITIES            0x1029
	mDevInfoText[GPUDevQueueProperties]  = GPUExtData(tintaGPUExt::str_gpu_device_queue_properties,GPUExtData::enExSizeT);//("Device queue properties: ");	// 				#define CL_DEVICE_QUEUE_PROPERTIES                  0x102A
	mDevInfoText[GPUDevName]  = GPUExtData(tintaGPUExt::str_gpu_device_name,GPUExtData::enExText);//("Device name: ");	// 				#define CL_DEVICE_NAME                              0x102B
	mDevInfoText[GPUDevVendor]  = GPUExtData(tintaGPUExt::str_gpu_device_vendor,GPUExtData::enExText);//("Device vendor: ");	// 				#define CL_DEVICE_VENDOR                            0x102C
	mDevInfoText[GPUDevDriverVersion]  = GPUExtData(tintaGPUExt::str_gpu_device_driver_version,GPUExtData::enExText);//("Device driver version: ");	// 				#define CL_DRIVER_VERSION                           0x102D
	mDevInfoText[GPUDevProfile]  = GPUExtData(tintaGPUExt::str_gpu_device_profile,GPUExtData::enExText);//("Device profile: ");	// 				#define CL_DEVICE_PROFILE                           0x102E
	mDevInfoText[GPUDevVersion]  = GPUExtData(tintaGPUExt::str_gpu_device_version,GPUExtData::enExText);//("Device version: ");	// 				#define CL_DEVICE_VERSION                           0x102F
	mDevInfoText[GPUDevExten]  = GPUExtData(tintaGPUExt::str_gpu_device_extensions,GPUExtData::enExText);//("Device extensions: ");	// 				#define CL_DEVICE_EXTENSIONS                        0x1030
	mDevInfoText[GPUDevPlatform		]  = GPUExtData(tintaGPUExt::str_gpu_device_platform,GPUExtData::enExUInt);//("Device platform: ");	// 				#define CL_DEVICE_PLATFORM                          0x1031	
	mDevInfoText[GPUDevPreferredVecWidthHalf]  = GPUExtData(tintaGPUExt::str_gpu_device_prereferred_vector_width_half,GPUExtData::enExSizeT);//("Device prereferred vector width half: ");	// 				#define CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF       0x1034
	mDevInfoText[GPUDevHostUnfiedMemory]  = GPUExtData(tintaGPUExt::str_gpu_device_host_unified_memory,GPUExtData::enExBool);//("Device host unified memory: ");	// 				#define CL_DEVICE_HOST_UNIFIED_MEMORY               0x1035
	mDevInfoText[GPUDevNativeVecWidthChar]  = GPUExtData(tintaGPUExt::str_gpu_device_native_vector_width_char,GPUExtData::enExSizeT);//("Device native vector width char: ");	// 				#define CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR          0x1036
	mDevInfoText[GPUDevNativeVecWidthShort]  = GPUExtData(tintaGPUExt::str_gpu_device_native_vector_width_short,GPUExtData::enExSizeT);//("Device native vector width short: ");	// 				#define CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT         0x1037
	mDevInfoText[GPUDevNativeVecWidthInt]  = GPUExtData(tintaGPUExt::str_gpu_device_native_vector_width_int,GPUExtData::enExSizeT);//("Device native vector width int: ");	// 				#define CL_DEVICE_NATIVE_VECTOR_WIDTH_INT           0x1038
	mDevInfoText[GPUDevNativeVecWidthLong]  = GPUExtData(tintaGPUExt::str_gpu_device_native_vector_width_long,GPUExtData::enExSizeT);//("Device native vector width long: ");	// 				#define CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG          0x1039
	mDevInfoText[GPUDevNativeVecWidthFloat]  = GPUExtData(tintaGPUExt::str_gpu_device_native_vector_width_float,GPUExtData::enExSizeT);//("Device native vector width float: ");	// 				#define CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT         0x103A
	mDevInfoText[GPUDevNativeVecWidthDouble]  = GPUExtData(tintaGPUExt::str_gpu_device_native_vector_width_double,GPUExtData::enExSizeT);//("Device native vector width double: ");	// 				#define CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE        0x103B
	mDevInfoText[GPUDevNativeVecWidthHalf]  = GPUExtData(tintaGPUExt::str_gpu_device_native_vector_width_half,GPUExtData::enExSizeT);//("Device native vector width half: ");	// 				#define CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF          0x103C
	mDevInfoText[GPUDevCLVersion]  = GPUExtData(tintaGPUExt::str_gpu_device_opencl_version,GPUExtData::enExText);//("Device opencl version: ");		// 				#define CL_DEVICE_OPENCL_C_VERSION                  0x103D

    mDevInfoText[GPUDevWorkGroupSize] = GPUExtData(tintaGPUExt::str_gpu_device_work_group_size, GPUExtData::enExSizeT);
    mDevInfoText[GPUDevWorkCompileGroupSize] = GPUExtData(tintaGPUExt::str_gpu_device_compile_work_group_size, GPUExtData::enExSizeT3);
    mDevInfoText[GPUDevWorkLocalMemSize] = GPUExtData(tintaGPUExt::str_gpu_device_local_mem_size, GPUExtData::enExULong);
    mDevInfoText[GPUDevWorkPreferedWorkGroupSize] = GPUExtData(tintaGPUExt::str_gpu_device_prefered_work_group_size, GPUExtData::enExSizeT);
    mDevInfoText[GPUDevWorkPrivateMemSize] = GPUExtData(tintaGPUExt::str_gpu_device_private_mem_size, GPUExtData::enExULong);

}

tintaGPUExt::~tintaGPUExt(void)
{
}
m_ulong32 tintaGPUExt::getPlatInfoCLField( GPUPlatInform field )const {
	
	m_ulong32 dataGet = 0x0000;


#ifdef USING_GPUCL
	switch( field ){
		case GPUPlatProfile:
			dataGet = CL_PLATFORM_PROFILE;
			break;
		case GPUPlatVersion:
			dataGet = CL_PLATFORM_VERSION;
			break;
		case GPUPlatVersionName:
			dataGet = CL_PLATFORM_NAME;
			break;
		case GPUPlatVersionVendor:
			dataGet = CL_PLATFORM_VENDOR;
			break;
		case GPUPlatExten:
			dataGet = CL_PLATFORM_EXTENSIONS;
			break;
		default:
			// dataGet = 0x0000; //all
			break;
	}

#else
	
#endif /*USING_GPUCL*/

	
	return dataGet;
}

m_ulong32 tintaGPUExt::getPlatformsIDs( )const {

	m_ulong32 rez = 0;
    mLastError = _M("");

#ifdef USING_GPUCL
		
	cl_uint num_platforms(0);
	cl_int err;
	
	/* Find number of platforms */
	err = clGetPlatformIDs(0, NULL, &num_platforms);		

	if( err < 0 ) {				
        mLastError = _M("Couldn't find any platforms.");
		return rez;
	}				
	rez = (m_ulong32)num_platforms;

#endif

	return rez;
}



m_ulong32 tintaGPUExt::getDeviceInfoCLField( GPUDevInform field )const {

	m_ulong32 dataGet = 0x0000;

#ifdef USING_GPUCL

	switch( field ){
		case GPUDevType:
			dataGet = CL_DEVICE_TYPE;  
			break;
		case GPUDevVendorId:
			dataGet = CL_DEVICE_VENDOR_ID;  
			break;
		case GPUDevMaxCaptureUnits:
			dataGet = CL_DEVICE_MAX_COMPUTE_UNITS;  
			break;		
		case GPUDevMaxWorkItemsDimentions:
			dataGet = CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS;  
			break;	
		case GPUDevMaxWorkGroupSize:
			dataGet = CL_DEVICE_MAX_WORK_GROUP_SIZE;  
			break;	

		case GPUDevMaxWorkItemSizes:
			dataGet = CL_DEVICE_MAX_WORK_ITEM_SIZES;  
			break;		
		case GPUDevMaxRefVecWidthChar:
			dataGet = CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR;  
			break;	

		case GPUDevMaxRefVecWidthShort:
			dataGet = CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT;  
			break;
		case GPUDevMaxRefVecWidthInt:
			dataGet = CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT;  
			break;	
		case GPUDevMaxRefVecWidthLong:
			dataGet = CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG;  
			break;	
		case GPUDevMaxRefVecWidthFloat:
			dataGet = CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT;  
			break;	
		case GPUDevMaxRefVecWidthDouble:
			dataGet = CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE;  
			break;		
		case GPUDevMaxClockFrequency:
			dataGet = CL_DEVICE_MAX_CLOCK_FREQUENCY;  
			break;			
		case GPUDevMaxArdessBits:
			dataGet = CL_DEVICE_ADDRESS_BITS;  
			break;
		case GPUDevMaxReadImgArgs:
			dataGet = CL_DEVICE_MAX_READ_IMAGE_ARGS;  
			break;
		case GPUDevMaxWriteImgArgs:
			dataGet = CL_DEVICE_MAX_WRITE_IMAGE_ARGS;  
			break;	
		case GPUDevMaxMemAllocSize:
			dataGet = CL_DEVICE_MAX_MEM_ALLOC_SIZE;  
			break;					
		case GPUDevMaxImage2dWidth:
			dataGet = CL_DEVICE_IMAGE2D_MAX_WIDTH;  
			break;
		case GPUDevMaxImage2dHeight:
			dataGet = CL_DEVICE_IMAGE2D_MAX_HEIGHT;  
			break;	
		case GPUDevMaxImage3dWidth:
			dataGet = CL_DEVICE_IMAGE3D_MAX_WIDTH;  
			break;
		case GPUDevMaxImage3dHeight:
			dataGet = CL_DEVICE_IMAGE3D_MAX_HEIGHT;  
			break;				
		case GPUDevMaxImage3dMaxDepth:
			dataGet = CL_DEVICE_IMAGE3D_MAX_DEPTH;  
			break;	
		case GPUDevImageSupport:
			dataGet = CL_DEVICE_IMAGE_SUPPORT;  
			break;	
		case GPUDevMaxParameterSize:
			dataGet = CL_DEVICE_MAX_PARAMETER_SIZE;  
			break;
		case GPUDevMaxSamplers:
			dataGet = CL_DEVICE_MAX_SAMPLERS;  
			break;
		case GPUDevMemBaseAdrAlign:
			dataGet = CL_DEVICE_MEM_BASE_ADDR_ALIGN;  
			break;			
		case GPUDevMinDataAlignSize:
			dataGet = CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE;  
			break;	
		case GPUDevSingleFPConfig:
			dataGet = CL_DEVICE_SINGLE_FP_CONFIG;  
			break;	
		case GPUDevGlobMemCacheType:
			dataGet = CL_DEVICE_GLOBAL_MEM_CACHE_TYPE;  
			break;
		case GPUDevGlobMemCacheLineSize:
			dataGet = CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE;  
			break;

		case GPUDevGlobMemCacheSize:
			dataGet = CL_DEVICE_GLOBAL_MEM_CACHE_SIZE;  
			break;
		case GPUDevGlobMemSize:
			dataGet = CL_DEVICE_GLOBAL_MEM_SIZE;  
			break;
		case GPUDevMaxConstBuffSize:
			dataGet = CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE;  
			break;
		case GPUDevMaxConstBuffArgs:
			dataGet = CL_DEVICE_MAX_CONSTANT_ARGS;  
			break;	
		case GPUDevLocalMemType:
			dataGet = CL_DEVICE_LOCAL_MEM_TYPE;  
			break;
		case GPUDevLocalMemSize:
			dataGet = CL_DEVICE_LOCAL_MEM_SIZE;  
			break;
		case GPUDevErrorCorrectionSupport:
			dataGet = CL_DEVICE_ERROR_CORRECTION_SUPPORT;  
			break;
		case GPUDevProfilingTimerResolution:
			dataGet = CL_DEVICE_PROFILING_TIMER_RESOLUTION;  
			break;

		case GPUDevEndianLittle:
			dataGet = CL_DEVICE_ENDIAN_LITTLE;  
			break;

		case GPUDevDeviceAvailable:
			dataGet = CL_DEVICE_AVAILABLE;  
			break;
		case GPUDevCompilerAvailable:
			dataGet = CL_DEVICE_COMPILER_AVAILABLE;  
			break;

		case GPUDevExecutionCapabilites:
			dataGet = CL_DEVICE_EXECUTION_CAPABILITIES;  
			break;
		case GPUDevQueueProperties:
			dataGet = CL_DEVICE_QUEUE_PROPERTIES;  
			break;

		case GPUDevName:
			dataGet = CL_DEVICE_NAME;  
			break;
		case GPUDevVendor:
			dataGet = CL_DEVICE_VENDOR;  
			break;

		case GPUDevDriverVersion:
			dataGet = CL_DRIVER_VERSION;  
			break;

		case GPUDevProfile:
			dataGet = CL_DEVICE_PROFILE;  
			break;

		case GPUDevVersion:
			dataGet = CL_DEVICE_VERSION;  
			break;

		case GPUDevExten:
			dataGet = CL_DEVICE_EXTENSIONS;  
			break;
		case GPUDevPlatform:
			dataGet = CL_DEVICE_PLATFORM;  
			break;
		case GPUDevPreferredVecWidthHalf:
			dataGet = CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF;  
			break;
		case GPUDevHostUnfiedMemory:
			dataGet = CL_DEVICE_HOST_UNIFIED_MEMORY;  
			break;
		case GPUDevNativeVecWidthChar:
			dataGet = CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR;  
			break;
		case GPUDevNativeVecWidthShort:
			dataGet = CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT;  
			break;
		case GPUDevNativeVecWidthInt:
			dataGet = CL_DEVICE_NATIVE_VECTOR_WIDTH_INT;  
			break;
		case GPUDevNativeVecWidthLong:
			dataGet = CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG;  
			break;
		case GPUDevNativeVecWidthFloat:
			dataGet = CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT;  
			break;
		case GPUDevNativeVecWidthDouble:
			dataGet = CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE;  
			break;
		case GPUDevNativeVecWidthHalf:
			dataGet = CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF;  
			break;
		case GPUDevCLVersion:
			dataGet = CL_DEVICE_OPENCL_C_VERSION;  
			break;
        case GPUDevWorkGroupSize:
            dataGet = CL_KERNEL_WORK_GROUP_SIZE;
            break;
        case GPUDevWorkCompileGroupSize:
            dataGet = CL_KERNEL_COMPILE_WORK_GROUP_SIZE;
            break;
        case GPUDevWorkLocalMemSize:
            dataGet = CL_KERNEL_LOCAL_MEM_SIZE;
            break;
        case GPUDevWorkPreferedWorkGroupSize:
            dataGet = CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE;
            break;
        case GPUDevWorkPrivateMemSize:
            dataGet = CL_KERNEL_PRIVATE_MEM_SIZE;
            break;

		default:
			// dataGet = 0x0000; //all
			break;	
		}
#else
	
	
#endif
	return dataGet;

}

String tintaGPUExt::getPlatformInfo(  ){

String rez;
mLastError = _M("");

#ifdef USING_GPUCL

	
	/* Host data structures */
	cl_platform_id *platforms;
	cl_uint num_platforms(0);
	cl_uint i; //, platform_index = -1;
	cl_int err = 0;
	
	
	
	/* Find number of platforms */
	err = clGetPlatformIDs(1, NULL, &num_platforms);		
	if(err < 0) {		
        mLastError = _M("Couldn't find any platforms.");
		//exit(1);		
		return rez;
	}									

	/* Access all installed platforms */
	//platforms = (cl_platform_id*) malloc(sizeof(cl_platform_id) * num_platforms);		

    platforms = ALLOC_T(cl_platform_id, num_platforms);

	clGetPlatformIDs(num_platforms, platforms, NULL);		
					
	size_t ext_size;   
	m_uint32 field = 0x0000;

	//String rezExt;//( ext_data );
	StringStream strData;
	/* Find extensions of all platforms */
	for( i=0; i<num_platforms; i++ ) {		
		
		strData<< tintaGPUExt::str_gpu_plat_text << _M("_") << i + 1 <<_M("\n");	
		

		for( m_uint32 k = 0; k < GPUDevAll; k++ ){
				
			tintaGPUExt::GPUPlatInform _data =  (tintaGPUExt::GPUPlatInform)k;
			field = getPlatInfoCLField( _data );
			/* Find size of extension data */
			err = clGetPlatformInfo( platforms[i],field, 0, NULL, &ext_size );		
			if( err < 0 ) {			
				break;
			}		
			//ext_data = (char*)malloc(ext_size);				
            UNIQPTRALLOC(char, ext_data, ALLOC_T(char, ext_size));
			clGetPlatformInfo( platforms[i], field, ext_size, ext_data.get(), NULL );
				
			strData << preparePlatTag(_data) << ext_data.get();
			strData<<_M("\n");			

		}

		
	}
	
	rez =  strData.str();
	//StringUtil::replace_all( rez, tintaGPUExt::str_space_tag, tintaGPUExt::str_new_line );
	
	
	FREE_T(platforms);
	

	
#else
	
#endif /*USING_GPUCL*/

	return rez;
	//return _M("");

}

String tintaGPUExt::getError() {
    return mLastError;
}

String tintaGPUExt::getDeviceInfo( m_uint32 platformId, m_uint32 *deviceId, void* kernel ) {

	String rez;
    mLastError = _M("");
#ifdef USING_GPUCL


	/* Host data structures */
	cl_device_id *devices;
	cl_uint num_devices;
	cl_int err; //, device_index = -1;
	cl_uint i;
	


	/* Host data structures */
	//cl_platform_id *platforms;
	cl_uint num_platforms = 0;
	

	/* Find number of platforms */
	err = clGetPlatformIDs(1, NULL, &num_platforms);	

	if( err < 0 || platformId >= num_platforms ) {		
        StringStreamBasic msg;
        msg << _M("Wrong platform id: ") << platformId;
        mLastError = msg.str();

		return rez;
	}									

	/* Access all installed platforms */
	//platforms = (cl_platform_id*)	malloc(sizeof(cl_platform_id) * num_platforms);		
    cl_platform_id *platforms = ALLOC_T(cl_platform_id, num_platforms);
    
    //UNIQPTRARRAY( cl_platform_id, platforms, arr, num_platforms);

	clGetPlatformIDs(num_platforms, platforms, NULL);	


	

	/* Find number of platforms */
	err = clGetDeviceIDs( platforms[platformId], CL_DEVICE_TYPE_GPU,  NULL, NULL, &num_devices);
	if(err < 0) {		
		
        StringStreamBasic msg;
       
        msg << _M("Couldn't find any devices on platform: ") << platformId;
        mLastError = msg.str();

		FREE_T(platforms);

		return rez;
	}									

	/* Access all installed platforms */
	//devices = ( cl_device_id* ) malloc( sizeof(cl_device_id) * num_devices );		
    devices = ALLOC_T( cl_device_id, num_devices );

	clGetDeviceIDs(  platforms[platformId], CL_DEVICE_TYPE_GPU, num_devices, devices, NULL);

   
    if ( deviceId && *(deviceId) >= num_devices) {

        StringStreamBasic msg;

        msg << _M("Wrong device id: ") << *deviceId;
        mLastError = msg.str();

        FREE_T(devices);
        FREE_T(platforms);

        return rez;
    }

						
	size_t ext_size;   
	m_uint32 field = 0x0000;

	//String rezExt;//( ext_data );
	StringStream strData;

	/* Find extensions of all platforms */
    m_uint32 devStart = deviceId ? *(deviceId) : 0;
    cl_kernel kernelCl = (cl_kernel)kernel;

	for( i = 0; i < num_devices; i++ ) {	

		strData<< tintaGPUExt::str_gpu_device << _M("_") << i <<_M("\n");

            m_uint32 begin = 0;
            m_uint32 end = GPUDevAll;

            if (kernel)
                begin = (m_uint32)GPUDevWorkGroupSize;
            else 
                end = (m_uint32)GPUDevWorkGroupSize;

			for( m_uint32 k = begin; k < end; k++ ){
				
				tintaGPUExt::GPUDevInform _data =  (tintaGPUExt::GPUDevInform)k;
				field = getDeviceInfoCLField( _data );

				GPUExtData::enType type = getDevDataType( _data );

				if( type == GPUExtData::enExText ){
                    					
					/* Find size of extension data */
					err = clGetDeviceInfo( devices[i] ,field, 0, NULL, &ext_size );		
                    UNIQPTRALLOC(char, ext_data, ALLOC_T(char, ext_size));
					if( err < 0 ) {			
						break;
					}		
					//ext_data = (char*)malloc(ext_size);				
					clGetDeviceInfo(devices[i], field, ext_size, ext_data.get(), NULL);
					strData << prepareDeviceTag(_data) << ext_data.get();
                   // strData << _M("\n");
					//delete[] ext_data;

				}			
				else if( type == GPUExtData::enExSizeT || type == GPUExtData::enExBool || type == GPUExtData::enExUInt){
					
                    size_t extData = 0;
                    
					/* Find size of extension data */
                    if( _data < GPUDevWorkGroupSize )
					    err = clGetDeviceInfo( devices[i] ,field, 0, NULL, &ext_size );		
                    else if( kernelCl )
                        err = clGetKernelWorkGroupInfo(kernelCl,devices[i], field, 0, NULL, &ext_size);

					if( err < 0 ) {	
                        assert(false);
						break;
					}		
                    err = -1;
                    if ( _data < GPUDevWorkGroupSize )
					    clGetDeviceInfo( devices[i] , field, sizeof(extData), &extData, NULL );
                    else if (kernelCl)
                        clGetKernelWorkGroupInfo(kernelCl, devices[i], field, sizeof(extData), &extData, NULL);
                    
                   
					strData<<prepareDeviceTag( _data )<< extData;

                    //strData << _M("\n");
				}
                else if (type ==  GPUExtData::enExULong)
                {
                    m_uint64 extData;                    
                    err = clGetKernelWorkGroupInfo(kernelCl, devices[i], field, 0, NULL, &ext_size);

                    if (err < 0) {
                        assert(false);
                        break;
                    }

                    clGetKernelWorkGroupInfo(kernelCl, devices[i], field, ext_size, &extData, NULL);
                    strData << prepareDeviceTag(_data) << extData;
                }
                else if (type == GPUExtData::enExSizeT3 ) {

                    size_t extData[3];                   
                  
                    err = clGetKernelWorkGroupInfo(kernelCl, devices[i], field, 0, NULL, &ext_size);

                    if (err < 0) {
                        assert(false);
                        break;
                    }                      
                    
                    clGetKernelWorkGroupInfo(kernelCl, devices[i], field, ext_size, &extData, NULL);
                    strData << prepareDeviceTag(_data) << extData[0] << _M(" ") << extData[1] << _M(" ") << extData[2];
                    
                }                
				else if( type ==  GPUExtData::enExOther){

					if(_data == GPUDevMaxWorkItemSizes ){

						size_t dims;
						/* Find size of extension data */
						err = clGetDeviceInfo( devices[i] ,CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, 0, NULL, &dims );		

						if( err < 0 || dims == 0) {
                            assert(false);
							break;
						}		

						clGetDeviceInfo( devices[i] , CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(dims), &dims, NULL );	

						size_t *extData = new size_t[dims];

						/* Find size of extension data */
						err = clGetDeviceInfo( devices[i] ,field, sizeof(size_t) * dims, extData, NULL );		

						if( err < 0 ) {	
							delete[] extData;
                            assert(false);
							break;
						}		

						strData<<prepareDeviceTag( _data )<< _M(": ");
						for(size_t d = 0; d < dims; d++){
							strData<<extData[d]<< _M(" "); //<<extData[1]<<_M(" ")<<extData[2];
						}
						

						delete[] extData;
					}
					else if( field == GPUDevType ){
						cl_device_type extData;
						/* Find size of extension data */
						err = clGetDeviceInfo( devices[i] ,field, 0, NULL, &ext_size );		

						if( err < 0 ) {		
                            assert(false);
							break;
						}		

						clGetDeviceInfo( devices[i] , field, sizeof(extData), &extData, NULL );	
						strData<<prepareDeviceTag( _data )<< extData;

					}

                   
				}
            strData << _M("\n");
			

            if ( deviceId )
                break;

		}
	}

	rez = strData.str();
	//StringUtil::replace_all( rez, tintaGPUExt::str_space_tag, _M("\n") );

    FREE_T( devices );
    FREE_T(platforms);


	

#endif /*USING_GPUCL*/

	return rez;


}

// GPUExtData_t tintaGPUExt::getPlatformInfoData( GPUPlatInform data ){
// 
// return GPUExtData_t;
// }
String tintaGPUExt::preparePlatTag( GPUPlatInform data )const {
	String tag( mPlatformInfoText.at( data ) );
	return tag;
}
String tintaGPUExt::prepareDeviceTag( GPUDevInform data )const {
	String tag( mDevInfoText.at( data ).mDataText );
	return tag;
}

tintaGPUExt::GPUExtData_t::enType	tintaGPUExt::getDevDataType( GPUDevInform data )const {
	
	return mDevInfoText.at( data ).mType;

}

template<> Tinta::tintaGPUExt* Tinta::Singleton<tintaGPUExt>::mPtr = 0;

} 