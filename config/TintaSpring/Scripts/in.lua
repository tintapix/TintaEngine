require "Scripts/lib/l_util"
require "paths"



-- что бы не забивать в генератор случайных чисел опять os.time() делаем это только один раз
if seedInit == nil then
	-- инициализируем генератор случайных чисел
	math.randomseed( os.time() )
	util.msg( os.time() )
	seedInit = true
end

function findNearest( box, xPos , yPos )	

	local q = box.elements(box)
	
	local halfQ = q/2
	
	local iter = 0
	local minLen = 99999999999
	local pos = 0
	
	for k = 0, halfQ - 1 do
		
		--util.msg(halfQ - 1)
		x = box.getvali32( box, iter )
		y = box.getvali32( box, iter + 1 )
		local len = s_get_length( x, y, xPos , yPos )		
		if  len < minLen then
		
			minLen = len
			pos = x
		end
		
		--[[len = s_get_length( 100 - x, 100 - y, xPos , yPos )		
		if  len < minLen then
		
			minLen = len
			pos = x
		end]]--
		
		iter = iter + 2
	end	
	return minLen
end


local imgPath = "c:/foto/test3.png"
local picImg = image.read( imgPath ,s_getex(imgPath) )

local boxId = s_boxFromImgRed(picImg)

--arrColor = s_getpallete(nameColors)
--arrColor = s_buildColorArray( arrColor, 32 )

local w = image.width()
local h = image.height()

if posWidth == nil then 
	posWidth = w
end

if posHeight == nil then 
	posHeight = h
end

local maxLen = 30

for h_ = 0, h-1 do	

	for w_ = 0, w-1 do									
		
		len = findNearest( box, w_, h_ )
		
		--image.setpixelf( w_,h_, 1.0 - math.min(len/maxLen,1.0), 1.0 - math.min(len/maxLen,1.0), 1.0 - math.min(len/maxLen, 1,1.0) )			
		
		--image.setpixelf( w_,h_, 1.0 - math.min(len/maxLen,1.0), 1.0 - math.min(len/maxLen,1.0), 1.0 - math.min(len/maxLen,1.0) )			
		--image.setpixelf( w_,h_, 0, math.cos(1.0 - math.min(len/maxLen,1.0)), 0 )			
		image.setpixelf( w_,h_, 0, 1.0 - math.min(len/maxLen,1.0), 0 )			
	
	end		
end	


local ext = s_getex(imgPath)
local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, main.uniqid(), ext )
image.save( s_getex( PathOut ), PathOut )



image.erase(box)
image.erase( picImg )

util.msg("***finished***")