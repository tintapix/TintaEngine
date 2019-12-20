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

local begTime = os.clock()

if seedInit == nil then
	-- инициализируем генератор случайных чисел
	math.randomseed( os.time() )
	util.msg( os.time() )
	seedInit = true
end

--if ocl.exist("image.cl") ~= true then
	ocl.create( "OCL/mask.cl", "image" )
--end


--local idA = c_createbox("uint8_vector")
--local idB = c_createbox("uint8_vector")

--c_resizebox(idA, 64000000)
--c_resizebox(idB, 64000000)

--[[for i = 1, 64000000 do
	c_pushbackboxui8( idA, math.random(0,255) )	
end
]]--

--ocl.setarray("image.cl", idA,  0x04)
--ocl.setarray("image.cl", idB,  0x02)

local PathIn = "C:/foto/IMG_20191126_231722.jpg"
local outExt = "png"

local arrColor = s_getpallete("3.png")
local sizeArr = s_arrlength(arrColor)
local arrayFactors = {0.4, 0.45, 0.5}

c_assert( sizeArr ==  s_arrlength(arrayFactors), "Массив коэфициентов и цветов не совпадает" )

for k = 1, sizeArr do	
	local PathOut = string.format( "%s/%s_%s.%s", pic_dir_filtered, c_uniqid(),k, outExt )
	--util.msg( PathOut )
	--util.msg( arrColor[k], " ", arrayFactors[k] )
		
	local idRead = c_readimg( PathIn )

	local w = c_getwidth()
	local h = c_getheight()

	
	--local idSave = c_createimg( w, h, "basic" )
	util.msg( "idRead:  ", idRead," w h: ",w ," ",h )

	ocl.setimage( "OCL/mask.cl", idRead,  0x01 )
	ocl.setvalue( "OCL/mask.cl", 0 )
	ocl.setvalue( "OCL/mask.cl", arrayFactors[k] )
	ocl.setvalue( "OCL/mask.cl", 3 )
	--ocl.setimage("image.cl", idSave,  0x02)
	-- 1 dimension
	ocl.execute( "OCL/mask.cl", w*h,0,0,  0, 0 )
	--util.sleep(5000)

	ocl.clear("OCL/mask.cl")

	c_selimg(idRead)

	c_saveimg( "jpg", PathOut )

	c_delallimg()
	
	util.msg( "saved" )
end


--[[




local idRead = c_readimg( PathIn )

local w = c_getwidth()
local h = c_getheight()

util.msg("point 1")

--local idSave = c_createimg( w, h, "basic" )
util.msg( "idRead:  ", idRead," w h: ",w ," ",h )

ocl.setimage("OCL/mask.cl", idRead,  0x01 )
ocl.setvalue("OCL/mask.cl",0)
ocl.setvalue("OCL/mask.cl",0.5)
ocl.setvalue("OCL/mask.cl",4)

--ocl.setimage("image.cl", idSave,  0x02)


-- 1 dimension
ocl.execute( "OCL/mask.cl", w*h,0,0,  0, 0 )
--util.sleep(5000)

ocl.clear("OCL/mask.cl")

c_selimg(idRead)

util.msg("point 2")


c_saveimg( "jpg", PathOut )

c_delallimg()


]]--

util.msg("finished")


