require "Scripts/lib/l_util"
require "paths"



-- что бы не забивать в генератор случайных чисел опять os.time() делаем это только один раз
if seedInit == nil then
	-- инициализируем генератор случайных чисел
	math.randomseed( os.time() )
	util.msg( os.time() )
	seedInit = true
end

local imgPath = "c:/foto/rez/2.png"
local picImg = image.read( imgPath ,s_getex(imgPath) )



local w = image.width()
local h = image.height()

if posWidth == nil then 
	posWidth = w
end

if posHeight == nil then 
	posHeight = h
end

local step = 40
local randEdge = 40
for h_ = 0, h-1 do	

	for w_ = 0, w-1 do												
		
		--image.setpixelf( w_,h_, 0, 1.0 - math.min(len/maxLen,1.0), 0 )
					
					local rCur,gCur,bCur,aCur = image.getpixelb( w_, h_ )									
					
					if rCur ~= 0 then
						r,g,b,aL = image.getpixelb( math.max(0,w_ - 1), h_ )
						if r == 0 and math.random(0,100) > randEdge then
							--image.setchannelb( math.max(0,w_ - 1), h_, 0, math.max(rCur - step, 0) )							
							image.setchannelb( math.max(0,w_ - 1), h_, 1, math.max(rCur - step, 0) )							
							
						end						
						
						--[[r,g,b,aLU = image.getpixelb( math.max(0,w_ - 1), math.max(0,h_ - 1) )	
						
						if r == 0 then
							--image.setchannelb( math.max(0,w_ - 1), math.max(0,h_ - 1), 0, math.max(rCur - step, 0) )
							image.setchannelb( math.max(0,w_ - 1), math.max(0,h_ - 1), 1, math.max(rCur - step, 0) )
							
						end
						]]--
						
						r,g,b,aU = image.getpixelb( w_, math.max(0,h_ - 1) )
						
						if r == 0 and math.random(0,100) > randEdge then
							--image.setchannelb( w_, math.max(0,h_ - 1), 0, math.max(rCur - step, 0) )
							image.setchannelb( w_, math.max(0,h_ - 1), 1, math.max(rCur - step, 0) )
							
						end
						
						--[[r,g,b,aRU = image.getpixelb( math.min(w - 1, w_ + 1), math.max(0,h_ - 1) )
						
						if r == 0 then
							--image.setchannelb( math.min(w - 1, w_ + 1), math.max(0,h_ - 1), 0, math.max(rCur - step, 0) )
							image.setchannelb( math.min(w - 1, w_ + 1), math.max(0,h_ - 1), 1, math.max(rCur - step, 0) )
						end
						]]--
						
						r,g,b,aR = image.getpixelb( math.min(w - 1, w_ + 1), h_ )
						if r == 0 and math.random(0,100) > randEdge then
							--image.setchannelb( math.min(w - 1, w_ + 1), h_, 0, math.max(rCur - step, 0) )
							image.setchannelb( math.min(w - 1, w_ + 1), h_, 1, math.max(rCur - step, 0) )
							
						end
						
						--[[r,g,b,aRD = image.getpixelb( math.min(w - 1, w_ + 1), math.min(h - 1,h_ + 1) )
						if r == 0 then
							--image.setchannelb( math.min(w - 1, w_ + 1), math.min(h - 1,h_ + 1), 0, math.max(rCur - step, 0) )
							image.setchannelb( math.min(w - 1, w_ + 1), math.min(h - 1,h_ + 1), 1, math.max(rCur - step, 0) )
							
						end
						]]--
						
						r,g,b,aD = image.getpixelb( w_, math.min(h - 1,h_ + 1) )
						if r == 0 and math.random(0,100) > randEdge then
							--image.setchannelb( w_, math.min(h - 1,h_ + 1), 0, math.max(rCur - step, 0) )
							image.setchannelb( w_, math.min(h - 1,h_ + 1), 1, math.max(rCur - step, 0) )
							
						end
						
						--[[r,g,b,aLD = image.getpixelb( math.max(0,w_ - 1), math.min(h - 1,h_ + 1) )
						if r == 0 then
							--image.setchannelb( math.max(0,w_ - 1), math.min(h - 1,h_ + 1), 0, math.max(rCur - step, 0) )
							image.setchannelb( math.max(0,w_ - 1), math.min(h - 1,h_ + 1), 1, math.max(rCur - step, 0) )
						end	
						]]--						
						
					
					end		
	
	end		
end	

for h_ = 0, h-1 do	

	for w_ = 0, w-1 do
		
		local rCur,gCur,bCur,aCur = image.getpixelb( w_, h_ )				
		if gCur > 0 then 
			image.setchannelb( w_, h_ , 0, gCur )
			image.setchannelb( w_, h_ , 1, 0 )
		end
		
	end
end
--fillchannelb( 1,0 )

local ext = s_getex(imgPath)
image.save( s_getex( imgPath ), imgPath )

image.erase( picImg )

util.msg("***finished***")