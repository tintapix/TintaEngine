require "Scripts/lib/l_util"
require "paths"


function s_scalepixels(PathIn, PathOut, factor, count )

local picImg = image.read( PathIn,  s_getex( PathIn ) )



for i = 1, count do

	local sizeW = image.width() * factor
	local sizeH = image.height() * factor

	local picImgOut = s_resizeImage(picImg, nil, math.floor(sizeW), math.floor(sizeH))
	util.msg(picImgOut)
	image.erase( picImg )
	picImg = picImgOut
	image.select( picImg )

	c_priorcolor( 4	, 0, 1  )
end

image.save( s_getex( PathOut ), PathOut )	
image.erase( picImg )	

end



local path = "c:/foto/rez/flower1.png" 
local PathOut = "c:/foto/rez/scale.png"

--s_scalepixels(path, PathOut, 2.0, 2 )


util.msg("*****scale finished*****")