require "Scripts/lib/l_util"
require "Scripts/lib/patterns"

require "paths"
--[[
file = io.open( "coord.txt", "w" )
io.output( file )

function mask_out( pathIn)
		
	local picImg = image.read( pathIn, s_getex(pathIn) )	
			
	local w = image.width()

	local h = image.height()
	
	for w_ = 0, w - 1 do	
		for h_ = 0, h - 1 do		
		
			image.select( picImg )										
			
			local r,g,b,a = image.getpixelb( w_, h_ ) 				
			
			if a > 0.0 then 							
				--util.msg( " addObject(", w_, ",", h_ ,")")	
				io.write( string.format(" addObject( %d,%d,%d,%d,%d )\n", w_, h_, r,g,b ) )				
			end				
		end		
	end
		
	image.erase( picImg )
			
end

mask_out( "D:/mya/ShapeBuildWin32_2018_08_31_115142/s2/face2.png" )
util.msg(".....finished.....")

io.close(file)
]]--
--[[
template <typename T>
	T distance( const tintaVector3<T>& pt1, const tintaVector3<T>& pt2 ){
		T x = pt1.mx - pt2.mx;
		T y = pt1.my - pt2.my;	
		T z = pt1.mz - pt2.mz;
		
		return (T)sqrt ( (double)(x*x + y*y + z*z) );
	}   
	]]--
	
local x1 = 1.0
local y1 = 1.0
local z1 = 1.0

local x2 = 0.0
local y2 = 0.0
local z2 = 0.0

util.msg( " ", math.sqrt( (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) + (z1 - z2)*(z1 - z2)) )
util.msg( "---------------------")

r = 254
g = 253
b = 241
util.msg(" ", util.bytetofloat(r), ",", util.bytetofloat(g), ",", util.bytetofloat(b))


for i = 4416, 4490 do 
	i = i + 1
	util.msg(i)
end

