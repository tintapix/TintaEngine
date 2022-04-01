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


			


local arrColor = nil 

--if palleteName ~= nil then
arrColor = s_getpallete("lsd4.png")
--end

local arrAllColors = s_buildColorArray( arrColor, 120 )			


local picOut = image.read( "c:/foto/#DSC082742.jpg", "jpg" )
w = image.width()
h = image.height()
--image.fillchannelf( 3, 1 )
factor  = 0.5
for w_ = 0, w -1 do	
	for h_ = 0, h-1 do		
	
			--local len, index = minDist( w_,h_, boxid )			
			
			local r,g,b,a = image.getpixelb( w_, h_ )	
			local pos, rezColor = s_findColorNearest( arrAllColors , util.packcolor3(r,g,b) )			
				
			
			local rVal, gVal,bVal = util.unpackcolor3( rezColor )
			
			--rVal = util.linterp()
			
			image.setpixelb( w_, h_, rVal, gVal,bVal, 1 )				
	end
end

box.erase( boxid )

window.redraw()

--image.save( "png", "c:/foto/res/out.png" )




util.msg("***finished***")