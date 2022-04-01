require "Scripts/lib/l_util"
require "paths"


require "Scripts/lib/l_util"
require "Scripts/lib/patterns"
require "Scripts/lib/interp"

require "paths"

local begTime = os.clock()
s_printTimePoint(begTime, 0)

-- что бы не забивать в генератор случайных чисел опять os.time() делаем это только один раз
if seedInit == nil then
	-- инициализируем генератор случайных чисел
	math.randomseed( os.time() )
	util.msg( os.time() )
	seedInit = true
end


-- noise from color array
nameColors = "5.png"
local arrColor = s_getpallete(nameColors)
r = s_buildColorArray( arrColor,8)
local outExt = "png"
local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, main.uniqid(), outExt )

local w = 2000	
local h = 2000

--s_randpixelsFromArray( w,h, PathOut, r )



-- GRADIENT and patterns


local PathOut = string.format("%s/%s.%s", pic_dir_filtered, "gradient", outExt )
local arrayOutSize = 80


--local imgPath = "C:/foto/rez/445998174_20169511162756_6739red3.png"
local xBeg = w * 0.5
local yBeg = h * 0.5
--s_circlegradient( PathOut, w, h, xBeg, yBeg, nameColors, arrayOutSize , true)
s_vertgradient(	 PathOut, w, h, nameColors, arrayOutSize , false )



