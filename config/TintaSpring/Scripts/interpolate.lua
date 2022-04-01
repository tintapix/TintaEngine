require "Scripts/lib/l_util"
require "paths"



-- что бы не забивать в генератор случайных чисел опять os.time() делаем это только один раз
if seedInit == nil then
	-- инициализируем генератор случайных чисел
	math.randomseed( os.time() )
	util.msg( os.time() )
	seedInit = true
end

local posWidth = nil
local posHeight = nil

function findNearest( boxId, xPos , yPos )	

	local q = box.elements(boxId)
	
	local halfQ = q/2
	
	local iter = 0
	local minLen = 99999999999
	local pos = 0
	--util.msg(halfQ)
	for k = 0, halfQ - 1 do
		
		
		x = box.getvali32( boxId, iter )
		y = box.getvali32( boxId, iter + 1 )
		local len = s_get_length( x, y, xPos , yPos )		
		if  len < minLen then
		
			minLen = len
			pos = x
		end
		
		
		
		iter = iter + 2
	end	
	return minLen
end


local imgPath = "c:/foto/graph.png"
local picImg = image.read( imgPath ,s_getex(imgPath) )

--local box = s_boxFromImgRedSeamless(picImg)
local boxId = s_boxFromImgRed(picImg)



local w = image.width()
local h = image.height()

if posWidth == nil then 
	posWidth = w
end

if posHeight == nil then 
	posHeight = h
end

local maxLen = 20

for h_ = 0, h-1 do	

	for w_ = 0, w-1 do									
		
		len = findNearest( boxId, h_ , w_ )
		
		--image.setpixelf( w_,h_, 1.0 - math.min(len/maxLen,1.0), 1.0 - math.min(len/maxLen,1.0), 1.0 - math.min(len/maxLen, 1,1.0) )			
		
		image.setpixelf( w_,h_, 0, 1.0 - math.min(len/maxLen,1.0), 0 )			
		--image.setpixelf( w_,h_, 0, math.cos(1.0 - math.min(len/maxLen,1.0)), 0 )			
	
	end		
end	


local ext = s_getex(imgPath)
local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, main.uniqid(), ext )
image.save( s_getex( PathOut ), PathOut )



image.erase(boxId)
image.erase( picImg )

util.msg("***finished***")