require "Scripts/lib/l_util"
require "paths"



-- что бы не забивать в генератор случайных чисел опять os.time() делаем это только один раз
if seedInit == nil then
	-- инициализируем генератор случайных чисел
	math.randomseed( os.time() )
	util.msg( os.time() )
	seedInit = true
end

image.eraseall( )


			
			


local width = 3200
local height = 3200
local points = 100
	
local pallete = "trend4.png"

local boxid = box.create("int32_vector")

random_points = false

if random_points == true then
	
	local arrColor =  s_getpallete( pallete ) --s_genColorsRandTable(0,255, 3 )  --
	local sizePallete = s_arrlength(arrColor)



	for i = 1, points do

		local randX = main.randintex(width * 0.08, width * 0.93)
		local randY = main.randintex(height * 0.08, height * 0.87)	
		
		box.pushbacki32(boxid, randX  )
		box.pushbacki32(boxid, randY )				
		local color = arrColor[main.randintex(1, sizePallete)]					
		box.pushbacki32(boxid, color )	
		
		--util.msg(" ",randX, " ", randY," ",string.format('%02X', (color)) )
		
	end
else

	local picImg = image.read( "c:/foto/test4.png", "png" )	

	width = image.width()
	height = image.height()

	
	for h_ = 0, height-1 do	
				for w_ = 0, width-1 do							
				
				local r,g,b,a = image.getpixelb( w_, h_ )					
				if a ~= 0  then					
				
					box.pushbacki32(boxid, w_  )
					box.pushbacki32(boxid, h_ )	
					
					local color = util.packcolor4( r, g, b, a )					
					box.pushbacki32(boxid, color )	
					
				end				
			end		
	end	
		
	image.erase(picImg)

end




local cl_name = "OCL/interp_points.cl"
ocl.create( cl_name, "image" )

local arraysize = box.elements(boxid)
util.msg(arraysize)

local picOut = image.create( width, height )

image.fillchannelf( 3, 1 )


ocl.setimage(cl_name, picOut,        0x01)
ocl.setvalue(cl_name, width )
ocl.setvalue(cl_name, height )
ocl.setarray(cl_name, boxid,  		 0x04)
ocl.setvalue(cl_name, arraysize )
ocl.setvalue(cl_name, image.channels(picOut) )

--ocl.setimage("image.cl", idSave,  0x02)


-- 1 dimension
ocl.execute( cl_name, width*height,0,0,  0, 0 )
--util.sleep(5000)

ocl.clear(cl_name)


--[[
local picOut = image.create( w, h )

image.fillchannelf( 3, 1 )

for w_ = 0, w -1 do	
	for h_ = 0, h-1 do		
	
			local len, index = minDist( w_,h_, boxid )			
			
			local rVal, gVal,bVal = util.unpackcolor3( box.getvali32( boxid, index + 2) )
			
			rVal, gVal,bVal = mixColors( rVal, gVal,bVal, len, w_,h_, index, boxid )
			
			image.setpixelb( w_, h_, rVal, gVal,bVal, 1 )				
	end
end

]]--

function save()
	local name = main.uniqid()
	--local name = "out"
	local PathOut = string.format("%s/%s.%s", pic_dir_filtered, name, "png" )
	image.save( "png", PathOut )
	util.msg("saved")
end


window.setkey(0x1D, "save()") -- ML_LCONTROL    = 0x1D,	

window.redraw()

--image.erase(picOut)

box.erase( boxid )
util.msg("***finished***")