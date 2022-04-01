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

require "paths"
require "Scripts/lib/l_util"

image.eraseall()

local begTime = os.clock()

if seedInit == nil then
	-- инициализируем генератор случайных чисел
	math.randomseed( os.time() )
	util.msg( os.time() )
	seedInit = true
end

--[[
	Умножает канал на F с уменьшением эффекта к макс значению: ch = ch * mix( factor, 1, ch/255 )
]]--

local kernelName = "gpu/mul_ch.cl"
ocl.create( kernelName, "image" )

local PathIn = "C:/foto/test_pixel.png"


local idRead = image.read( PathIn )

local w = image.width()
local h = image.height()

image.select(idRead)
util.msg( "idRead:  ", idRead," w h: ",w ," ",h , " channels ", image.channels( image.selected() ) )

local channel = 3
local factor = 0.5

ocl.setimage(kernelName, idRead,  0x01 )
ocl.setvalue(kernelName, channel )
ocl.setvalue(kernelName, factor )
ocl.setvalue(kernelName, image.channels( image.selected() ) )

-- 1 dimension

local stages = 2
for i = 1, stages do
	ocl.execute( kernelName, w*h,0,0,  0, 0 )
end

ocl.clear( kernelName )

--image.erase( idRead )

function save()
	image.select(idRead)
	local name = main.uniqid()
	--local name = "out"
	local PathOut = string.format("%s/%s.%s", pic_dir_filtered, name, "png" )
	image.save( "png", PathOut )
	util.msg("saved")
	image.eraseall()
end

	
window.setkey(0x1D, "save()") -- ML_LCONTROL    = 0x1D,	
image.select(idRead)
window.redraw()

util.msg("finished")


