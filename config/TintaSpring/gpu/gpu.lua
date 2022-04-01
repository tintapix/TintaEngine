

ocl.create("1.cl","vecadd")

local idA = c_createbox("int32_vector")
local idB = c_createbox("int32_vector")
local idC = c_createbox("int32_vector")
c_resizebox(idC, 10)


for i = 1, 10 do
	c_pushbackboxui8(idA, i )
	c_pushbackboxui8(idB, i + 100)
end


local size = c_boxelements(idC)




ocl.setarray("1.cl", idA,  0x04)
ocl.setarray("1.cl", idB,  0x04)
ocl.setarray("1.cl", idC,  0x02)
-- 1 dimension
ocl.execute("1.cl", 10,0, 2)

for i = 0, size-1 do
	local val = c_getvalboxi( idC, i )
	util.msg(val)
end


c_delallboxes()