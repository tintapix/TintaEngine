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



function mixColors( r,g,b,a, minLen, x,y, index, boxId )

	local q = box.elements( boxId )
	local pos = 0
	
	rRez = r
	gRez = g
	bRez = b
	aRez = a
	
	for i = 0, q/3 - 1 do 
		
		--if index ~= pos then		
		
			local len = s_dist2(x,y, box.getvali32( boxId, pos), box.getvali32( boxId, pos + 1) )
			
			r2,g2,b2,a2 = util.unpackcolor4( box.getvali32( boxId, pos + 2) )
			factor = 1.8 -- def 1
			rRez = util.cosinterp(rRez, r2, (minLen/math.max(len*factor,1.0)))
			gRez = util.cosinterp(gRez, g2, (minLen/math.max(len*factor,1.0)))
			bRez = util.cosinterp(bRez, b2, (minLen/math.max(len*factor,1.0)))
			aRez = util.cosinterp(aRez, a2, (minLen/math.max(len*factor,1.0)))
			
			-- cool glitch
			--local factor = (len+ minLen)/2
			--rRez = util.cosinterp(rRez, r2, factor)
			--gRez = util.cosinterp(gRez, g2, factor)
			--bRez = util.cosinterp(bRez, b2, factor)	
				
			
		--end	
		
		pos = pos + 3
		
	end
	--util.msg("\n")			
	--util.msg(" ", rRez," ",gRez," ", bRez)
	return math.floor(rRez), math.floor(gRez), math.floor(bRez), math.floor(aRez)
end

local picImg = image.read( "c:/foto/test4.png", "png" )	

local w = image.width()
local h = image.height()

local boxid = box.create("int32_vector")

for h_ = 0, h-1 do	
			for w_ = 0, w-1 do							
			
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




local picOut = image.create( w, h )

image.fillchannelf( 3, 1 )

for w_ = 0, w -1 do	
	for h_ = 0, h-1 do		
	
			local len, index = minDist( w_,h_, boxid )			
			--local len= averDist( w_,h_, boxid )			
				
			
			local rVal, gVal,bVal,aVal = util.unpackcolor4( box.getvali32( boxid, index + 2) )
			--util.msg(rVal," ", gVal, " ",bVal, " ",aVal)
			rVal, gVal,bVal,aVal = mixColors( rVal, gVal,bVal,aVal,len, w_,h_, index, boxid )
			
			image.setpixelb( w_, h_, rVal, gVal,bVal, 1 )	
			image.setchannelb( w_, h_, 3, aVal )			
	end
end


box.erase( boxid )
util.msg("***redraw***")
--window.redraw()
image.save( "png", "c:/foto/rez/out.png" )




util.msg("***finished***")