require "Scripts/lib/l_util"
require "paths"



-- что бы не забивать в генератор случайных чисел опять os.time() делаем это только один раз
if seedInit == nil then
	-- инициализируем генератор случайных чисел
	math.randomseed( os.time() )
	util.msg( os.time() )
	seedInit = true
end

--image.eraseall( )


			
			

function minDist( x,y, boxid )

	local minDist = 999999999.0;
	local index = -1
	
	
	local q = box.elements( boxid )
	--util.msg(" ",q )
	local pos = 0
	for i = 0, q/3 - 1 do 
		
		local len = s_dist2(x,y, box.getvali32( boxid, pos), box.getvali32( boxid, pos + 1) )
		
		if len < minDist then
		minDist = len
		
		index = pos
		
		--util.msg(" ", minDist," ", box.getvali32( boxid, pos)," ",box.getvali32( boxid, pos + 1) )
		end
		pos = pos + 3
		
	end
	return minDist, index
end



function mixColors( r,g,b, minLen, x,y, index, boxId )

	local q = box.elements( boxId )
	local pos = 0
	
	rRez = r
	gRez = g
	bRez = b
	
	for i = 0, q/3 - 1 do 
		
		--if index ~= pos then		
		
			local len = s_dist2(x,y, box.getvali32( boxId, pos), box.getvali32( boxId, pos + 1) )
			
			r2,g2,b2 = util.unpackcolor3( box.getvali32( boxId, pos + 2) )
			-- 					
			rRez = util.cosinterp(rRez, r2, (minLen/math.max(len,1.0)))
			gRez = util.cosinterp(gRez, g2, (minLen/math.max(len,1.0)))
			bRez = util.cosinterp(bRez, b2, (minLen/math.max(len,1.0)))
			
			-- cube
			--local cubeEdge = 1.3 -- -> 2
			--rRez = util.cosinterp(rRez, r2, (minLen/cubeEdge/math.max(len,1.0)))
			--gRez = util.cosinterp(gRez, g2, (minLen/ cubeEdge/math.max(len,1.0)))
			--bRez = util.cosinterp(bRez, b2, (minLen/ cubeEdge/math.max(len,1.0)))
			
			-- cool glitch
			--local factor = (len+ minLen)/30
			--rRez = util.cosinterp(rRez, r2, factor)
			--gRez = util.cosinterp(gRez, g2, factor)
			--bRez = util.cosinterp(bRez, b2, factor)	
				
			
		--end	
		
		pos = pos + 3
		
	end
	--util.msg("\n")			
	--util.msg(" ", rRez," ",gRez," ", bRez)
	return math.floor(rRez), math.floor(gRez), math.floor(bRez)
end
	

local width = 100
local height = 100
local points = 10
	
local pallete = "6.png"
local picImg = image.create(width,height) --image.read( "c:/foto/test4.png", "png" )	



local w = image.width()
local h = image.height()

local boxid = box.create("int32_vector")
image.fillchannelf( 3, 0.0 )

local arrColor =  s_getpallete( pallete ) -- s_genColorsRandTable(0,255, 12 ) -- s_getpallete( pallete ) --
local sizePallete = s_arrlength(arrColor)

--util.msg(sizePallete)
image.select(picImg)
for i = 1, points do

	local randX = main.randintex(0, w - 1)
	local randY = main.randintex(0, h - 1)
	
	local bR,bG,bB = util.unpackcolor3( arrColor[main.randintex(1, sizePallete)] )
	--util.msg(" ",bR," ",bG," ",bB)
	image.setpixelb( randX,randY, bR,bG,bB, 1 )		
	image.setchannelf(randX, randY, 3, 1.0 )	
	
end

for h_ = 0, h-1 do	
			for w_ = 0, w-1 do							
			
			local r,g,b,a = image.getpixelb( w_, h_ )					
			if a ~= 0  then					
			
				box.pushbacki32(boxid, w_  )
				box.pushbacki32(boxid, h_ )	
				
				local color = util.packcolor3( r, g, b )					
				box.pushbacki32(boxid, color )	
				
			end				
		end		
end	
	
image.erase(picImg)



local picOut = image.create( w, h )

image.fillchannelf( 3, 1 )

for w_ = 0, w -1 do	
	for h_ = 0, h-1 do		
	
			local len, index = minDist( w_,h_, boxid )			
			--local len= averDist( w_,h_, boxid )			
				
			
			local rVal, gVal,bVal = util.unpackcolor3( box.getvali32( boxid, index + 2) )
			
			rVal, gVal,bVal = mixColors( rVal, gVal,bVal, len, w_,h_, index, boxid )
			
			image.setpixelb( w_, h_, rVal, gVal,bVal, 1 )				
	end
end


box.erase( boxid )


util.msg("***redraw***")
--window.redraw()

local PathOut = string.format("%s/%s.%s", pic_dir_filtered, main.uniqid(), "png" )
function save()
	local name = main.uniqid()
	--local name = "out"
	local PathOut = string.format("%s/%s.%s", pic_dir_filtered, name, "png" )
	image.save( "png", PathOut )
	util.msg("saved")
	image.eraseall()
end


window.setkey(0x1D, "save()") -- ML_LCONTROL    = 0x1D,	

window.redraw()
	
util.msg("***finished***")