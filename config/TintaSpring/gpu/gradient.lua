--[[
#define CL_SUCCESS                                  0
#define CL_DEVICE_NOT_FOUND                         -1
#define CL_DEVICE_NOT_AVAILABLE                     -2
#define CL_COMPILER_NOT_AVAILABLE                   -3
#define CL_MEM_OBJECT_ALLOCATION_FAILURE            -4
#define CL_OUT_OF_RESOURCES                         -5
#define CL_OUT_OF_HOST_MEMORY                       -6
#define CL_PROFILING_INFO_NOT_AVAILABLE             -7
#define CL_MEM_COPY_OVERLAP                         -8
#define CL_IMAGE_FORMAT_MISMATCH                    -9
#define CL_IMAGE_FORMAT_NOT_SUPPORTED               -10
#define CL_BUILD_PROGRAM_FAILURE                    -11
#define CL_MAP_FAILURE                              -12
#define CL_MISALIGNED_SUB_BUFFER_OFFSET             -13
#define CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST -14

#define CL_INVALID_VALUE                            -30
#define CL_INVALID_DEVICE_TYPE                      -31
#define CL_INVALID_PLATFORM                         -32
#define CL_INVALID_DEVICE                           -33
#define CL_INVALID_CONTEXT                          -34
#define CL_INVALID_QUEUE_PROPERTIES                 -35
#define CL_INVALID_COMMAND_QUEUE                    -36
#define CL_INVALID_HOST_PTR                         -37
#define CL_INVALID_MEM_OBJECT                       -38
#define CL_INVALID_IMAGE_FORMAT_DESCRIPTOR          -39
#define CL_INVALID_IMAGE_SIZE                       -40
#define CL_INVALID_SAMPLER                          -41
#define CL_INVALID_BINARY                           -42
#define CL_INVALID_BUILD_OPTIONS                    -43
#define CL_INVALID_PROGRAM                          -44
#define CL_INVALID_PROGRAM_EXECUTABLE               -45
#define CL_INVALID_KERNEL_NAME                      -46
#define CL_INVALID_KERNEL_DEFINITION                -47
#define CL_INVALID_KERNEL                           -48
#define CL_INVALID_ARG_INDEX                        -49
#define CL_INVALID_ARG_VALUE                        -50
#define CL_INVALID_ARG_SIZE                         -51
#define CL_INVALID_KERNEL_ARGS                      -52
#define CL_INVALID_WORK_DIMENSION                   -53
#define CL_INVALID_WORK_GROUP_SIZE                  -54
#define CL_INVALID_WORK_ITEM_SIZE                   -55
#define CL_INVALID_GLOBAL_OFFSET                    -56
#define CL_INVALID_EVENT_WAIT_LIST                  -57
#define CL_INVALID_EVENT                            -58
#define CL_INVALID_OPERATION                        -59
#define CL_INVALID_GL_OBJECT                        -60
#define CL_INVALID_BUFFER_SIZE                      -61
#define CL_INVALID_MIP_LEVEL                        -62
#define CL_INVALID_GLOBAL_WORK_SIZE                 -63
#define CL_INVALID_PROPERTY                         -64
]]--

require "Scripts/lib/utf8"
require "Scripts/lib/utf8data"
require "Scripts/lib/l_util"
require "paths"

local begTime = os.clock()

if seedInit == nil then
	-- инициализируем генератор случайных чисел
	math.randomseed( os.time() )
	util.msg( os.time() )
	seedInit = true
end

local cl_name = "OCL/gradient.cl"


local channel = 1

-- read red markers
local imgPath = "c:/foto/1.png"
local ext = s_getex( imgPath )
--local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, c_uniqid(), ext )
local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, "interpolated_gpu", ext )

local picImg = c_readimg( imgPath ,s_getex(imgPath) )

ocl.create( cl_name, "image" )

local w = c_getwidth()
local h = c_getheight()
local imgChannels = c_channels( c_getimgsel() )

--util.msg("point 1")



--local box = s_boxFromImgRedSeamless(picImg)
local box = s_boxFromImgRed(picImg)

--util.msg("point 2")

util.msg( "idRead:  ", picImg," w h: ",w ," ",h )
local arraysize = c_boxelements(box)
util.msg(arraysize)
ocl.setimage(cl_name, picImg,        0x01)
ocl.setvalue(cl_name, w )
ocl.setvalue(cl_name, h )
ocl.setarray(cl_name, box,  		 0x04)
ocl.setvalue(cl_name, arraysize )
ocl.setvalue(cl_name, channel )
ocl.setvalue(cl_name, imgChannels)
ocl.setvalue(cl_name, 500.0)

--util.msg(imgChannels)
--ocl.setimage("image.cl", idSave,  0x02)

util.msg("point 3")
-- 1 dimension
ocl.execute( cl_name, w*h,0,0,  0, 0 )
--util.sleep(5000)

util.msg("point 4")

ocl.clear(cl_name)

c_selimg(picImg)

c_saveimg( ext, PathOut )

c_delallimg()
c_delbox(box)

util.msg("finished")



