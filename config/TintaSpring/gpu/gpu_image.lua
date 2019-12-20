local begTime = os.clock()


if seedInit == nil then
	-- инициализируем генератор случайных чисел
	math.randomseed( os.time() )
	util.msg( os.time() )
	seedInit = true
end

--if ocl.exist("image.cl") ~= true then
	ocl.create( "OCL/image.cl", "image" )
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

local idRead = c_readimg( "c:/foto/DSC08345.JPG","JPG" )


local w = c_getwidth()
local h = c_getheight()

util.msg("point 1")

local idSave = c_createimg( w, h, "basic" )
util.msg("idRead:  ",idRead," w h: ", w," ",h)

ocl.setimage("image.cl", idRead,  0x04)
ocl.setimage("image.cl", idSave,  0x02)


-- 1 dimension
ocl.execute( "image.cl", w*h,0,0,  1, 4 )
--util.sleep(5000)

ocl.clear("image.cl")

c_selimg(idSave)

util.msg("point 2")
c_saveimg( "jpg", "c:/foto/out.jpg" )




c_delallimg()
util.msg("finished")


