require "Scripts/lib/l_util"
require "Scripts/lib/patterns"

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

--[[
-- noise from color array
nameColors = "5.png"
local arrColor = s_getpallete(nameColors)
r = s_buildColorArray( arrColor, 100)
local outExt = "png"
local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, c_uniqid(), outExt )
--s_randpixelsFromArray( 1000,1000, PathOut, r )
--local maskPath = "D:/mya/textures/_A/IMG_6581.JPG" 
--function s_randpixelsFromArrayMask(wIn, hIn, pathdestIn, maskPathIn, factorIn, invertIn, arrayIn )
--s_randpixelsFromArray( 1400,1400, PathOut, r )
]]--


function s_pointsGradient( PathOut, w, h, pointsId , palleteName, arrayOutSize )

	local exImageOut = s_getex(PathOut)	
	
	local arrColor = s_getpallete(palleteName)
	
	local arrOut = s_buildColorArray( arrColor, arrayOutSize )	
	
	local picImg = c_createimg( w, h , "basic" ) -- c_readimg( imgPathIn, exImage, imageType )	

	
	
	local w = c_getwidth()
	local h = c_getheight()
	
	c_fillimgb( 255,255,255 )
	c_fillalphaf( 1.0 )
	
	local arrColor = s_getpallete(nameColors)
	arrColor = s_genColorsRandTable(0,255, arrayOutSize )
	
	local xCount = 0
	local yCount = 0
		
	
	local sizeArr = s_arrlength(arrOut)
	c_selimg( picImg )
	while xCount < w do			
	
		while yCount < h do		
		
			local elem = c_boxelements(pointsId)
			sumLen = 0
			for i = 0, elem - 1 do
				local x = xCount
				local y = yCount
				
				local xp, yp, zp = c_getvalbox3f( pointsId,i )
				sumLen = sumLen + s_get_length( x,y,xp,yp )				
			end
			
			local rp = 0
			local gp = 0
			local bp = 0			
			
			for i = 0, elem - 1 do
				local x = xCount
				local y = yCount
				
				local xp, yp, zp = c_getvalbox3f(pointsId,i)
				local pointLen = s_get_length(x,y,xp,yp )
				local bR,bG,bB = util.unpackcolor( arrColor[ i + 1 ] )
				
				rp = rp + ( 1.0 - (pointLen/sumLen) ) * bR
				gp = gp + ( 1.0 - (pointLen/sumLen) ) * bG
				bp = bp + ( 1.0 - (pointLen/sumLen) ) * bB			

				--util.msg( i, " ", 1.0 - (pointLen/sumLen) )			
					
			end			
			
			c_setpixelb( xCount, yCount, math.floor(rp),math.floor(gp),math.floor(bp), 1 )
			
			yCount = yCount + 1			
		end		
		yCount = 0
		xCount = xCount + 1		
	end		
	
	c_selimg( picImg )
	c_saveimg( exImageOut, PathOut )
	c_delimg( picImg )
	
	
	util.msg("***s_pointsGradient finished")
	
end



--[[
local outExt = "png"	
local coord = c_createbox( "vec3f_vector" )		
colors = 6 
w = 1000
h = 1000
for i = 1 , colors do
	c_pushbackbox3f( coord, math.floor(math.random(0,w-1)), math.floor(math.random(0,h-1)), 0.0 )	
end
local palleteName = "6.png"
local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, c_uniqid(), outExt )
s_pointsGradient( PathOut, w, h, coord , palleteName, colors )
c_delbox(coord)
]]--




-- GRADIENT
local outExt = "png"
local palleteName = "7.png"
local PathOut = string.format("%s/%s.%s", pic_dir_filtered, c_uniqid(), outExt )
local arrayOutSize = 20
local w = 1000	
local h = 1000
local xBeg = w * 0.5
local yBeg = h * 0.62
--local imgPath = "C:/foto/rez/445998174_20169511162756_6739red3.png"
--s_circlegradient( PathOut, w, h, xBeg, yBeg, palleteName, arrayOutSize , false)
--s_vertgradient( PathOut, w, h, palleteName, arrayOutSize , false )








local PathIn = "C:/foto/IMG_20191126_153945.jpg"
local outExt = "png"
local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, c_uniqid(), outExt )
-- factor < 0.5 -> more sensetive to the light
--local rVal, gVal,bVal = util.c_unpackcolor(0x2d4abf)
--s_maskchannel( pathIn, pathOut , rIn,gIn,bIn, channel, factor, invert, bAlphaTestIn )
--s_maskchannel( PathIn, PathOut, 0,0,0, 0, 0.47, false, true)

-- Adds images tile, brick
-- "seamless" or "basic"
--local PathIn = "C:/foto/clean.png"
local Dir = "C:/foto/pattern/brick"
local ext = ".png"
local PathOut = string.format("%s/%s.%s", pic_dir_filtered, c_uniqid(), ext )
local quantity = 10
local space  = 4
local typeArr = 1 --0 - tile
--s_addPatternImgOther( 2000, 2000, "seamless", Dir, "png", PathOut, quantity,space, typeArr , 1, false )



--local PathIn = "C:/foto/pixelart2.png"
--s_exportpixels( PathIn, "export.txt" )


--local PathIn = "d:/mya/IMG_20180914_141939_blur.jpg"--"d:/mya/IMG_20180621_182711_blur.jpg"
local PathIn = "D:/mya/foto/IMG_20190723_181540.jpg"
local PathOut = string.format("%s/%s.%s", pic_dir_filtered, c_uniqid(), "png" )
local arrayOutSize = 32
local palleteName = "lsd2.png" -- may be null random then
local shuffle = false
local invert = true
local channel = 0

palleteName = nil
--s_discretecolor( PathIn, PathOut, arrayOutSize, palleteName, shuffle, invert, channel )
palleteName = nil
--1 step -- blurr first
--s_poster( PathIn, PathOut, palleteName, false, arrayOutSize ) --arrayOutSize, palleteName, shuffle, invert, channel, false )
--PathIn, PathOut, around, factor, passes,
-- clear image 
-- good after resizing comic  
--5  - for texture

PathIn = "C:/foto/rez/3132729095_201911713638824_12343.png"
PathOut = string.format( "%s/%s.%s", pic_dir_filtered, c_uniqid(), "png" )

-- pixel art resize 2000х2000
--s_priorcolor(PathIn, PathOut, 5, 0, 3 )

--s_priorcolor(PathIn, PathOut, 5, 0, 3 ) -- good after rotate
--s_priorcolor(PathIn, PathOut, 2, 0, 40  )
-- 1 clears all possible in max strength

-- outline
--3 step
--PathIn = PathOut
--PathIn = "C:/foto/rez/3132729095_2018112216844457_7967.png"
PathOut = string.format("%s/%s.%s", pic_dir_filtered, c_uniqid(), "png" )
--function s_priorcolor ( PathIn, PathOut, around, factor, passes, isOutline, rOut, gOut, bOut )
-- less around more passes - outline more accurate
 
--s_priorcolor( PathIn, PathOut, 5, 0, 7, true ,0,255,0  ) -- good outline

--s_priorcolor( PathIn, PathOut, 5, 0, 4, true ,0,255,0  )


--[[CLEAR for vector]]--
--[[
PathIn = "C:/foto/3132729095_2019111410138350_22887.png"
local outExt = "png"
local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, c_uniqid(), outExt )
--s_clearImage( imgPathIn, imgRezPathIn, pixAroundIn, rIn, gIn, bIn, factorMaxIn )
local img = c_readimg( PathIn , "png" )

-- val < factor
c_clearimage( 10, 0.1 )

c_saveimg( "png", PathOut )
c_delimg( img )
]]--

--[[
local path = "C:/Scripts/lib/l_util.lua"
local regex =  ""

local textBox = s_getStringArrayId(path, "utf8", true)

local qAll = c_boxelements( textBox )
--util.msg(qAll)


--for i = 0, qAll - 1 do 
	--local vec = c_regextokenex( regex, textBox, i "0", 1 )
	local vec = c_regextoken( regex, "function      s_fu_1_v2 ( param1) s_dd function s_fu_2_v2.png ( param1) _s_dd", "0", 1 )
	--local vec = c_regextoken( regex, "<title>  asss   </title>", "1", 1 )
	if vec ~= nil then
		local q = c_boxelements(vec)
		util.msg( "size: " , q )
		for k = 0, q - 1 do
			util.msg(c_getvalboxs(vec,k))d:
			
		end	
		c_delbox(vec)
	end
--end

c_delbox(textBox)

]]--

--c_assert(false, "unexecutable file!!!!!!!!")

--[[
local PathOut = string.format("%s/%s.%s", pic_dir_filtered, c_uniqid(), "png" )
local PathIn = "c:/foto/rez/3132729095_20161116173021302_21316.png"
local imgId = c_readimg( PathIn, s_getex(PathIn) )		
c_scaleimg( imgId, 400,400 , true )
c_saveimg( s_getex( PathOut ), PathOut )
c_delimg(imgId)
]]--

--[[
local outExt = "png"
local PathIn = "C:/foto/process/grid.png"
local PathOut = string.format("%s/%s.%s", pic_dir_filtered, c_uniqid(), outExt )
s_multiplyChannel( PathIn, PathOut, 1, 0.4, true )
]]--




--[[
local PathIn = "C:/foto/sky_1.png"
--local pathIn = "C:/foto/rez/3132729095_2017122812211713_11920.png"
local ext = s_getex(PathIn)
local pathOut = string.format("%s/%s.%s", pic_dir_filtered, c_uniqid(), "png" )
s_grayscale( PathIn, pathOut )
]]--



--s_randpixels(PathIn,PathOut, -20, 20, -20, 20, -20, 20)











--[[
-- smooth cut
local outExt = "png"
local PathIn = "C:/foto/pen_test.jpg"
local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, c_uniqid(), outExt )
local tolerance = 0.8 -- 1 - max
local channel = 0
s_getStain( PathIn, PathOut, tolerance, channel )
]]--




--[[
local path = "c:/foto/rez/445998174_2016930134610377_20433.png" 
local PathOut = string.format("%s/%s.%s", pic_dir_filtered, c_uniqid(), "png" )
s_resizeImage(path, PathOut, 100, 100)
]]--

--[[
-- injection test
local imgToRead = "c:/foto/rez/3132729095_2016103111588666_27159.png"
local picToGet = c_readimg( imgToRead, s_getex(imgToRead), "basic" )	 	
imgToRead = "C:/foto/pattern/poly90_1/2.png"
local addImg = c_readimg( imgToRead, s_getex(imgToRead), "basic" )
c_injectimg( addImg, picToGet, 150, 150, 1  )								
c_saveimg( "png", "c:/foto/rez/ass.png")	
c_delimg( picToGet )
c_delimg( addImg )
util.msg("finished")

]]--
--[[
local picAdd = c_readimg( "C:/foto/rect2985.png", "png" )
c_scaleimage(0.7, 0.7 )

c_saveimg( "png", "C:/foto/1_.png" )

c_delimg(picAdd)
util.msg("finished")
]]--

--c_f("export.lua")

--[[
match_default = 0x0000,
	match_not_bol = 0x0001,
	match_not_eol = 0x0002,
	match_not_bow = 0x0004,
	match_not_eow = 0x0008,
	match_any = 0x0010,
	match_not_null = 0x0020,
	match_continuous = 0x0040,
	_Match_partial = 0x0080,
	match_prev_avail = 0x0100,
	format_default = 0x0000,
	format_sed = 0x0400,
	format_no_copy = 0x0800,
	format_first_only = 0x1000,
	_Match_not_null = 0x2000
	
	]]--
--[[


local regex = "(\\d{2})/(\\d{2})/(\\d{4})"
local textBox = s_getStringArrayId( "D:/mya/MolyWords/text.txt", "utf8", true )
util.msg( "...Read ....")
local i = c_regextokenex( regex, textBox, 0	, 1 , "2", 1 )
--local i = c_regextoken( regex, "01/02/2003 blahblah 04/23/1999 blahblah 11/13/1981", "2", 1	)

local q = c_boxelements(i)
util.msg( "size: " , q )
for k = 0, q - 1 do
	util.msg(c_getvalboxs(i,k))
end
c_delbox(i)
c_delbox(textBox)
]]--

--strs_box
--[[
local idBox = c_readfile("D:/mya/MolyWords/SpanishWords.txt", "utf8", true )
 --= c_createbox(c_createbox"str_vector")
--c_pushbackboxs( idBox, "El estúpido gordo cabrón" )
--val = c_getvalboxs( idBox, 0 )
c_writetofile("D:/mya/MolyWords/SpanishWords2.txt", idBox )
--util.msg(val)
c_delbox(idBox)
]]--


--[[
-- inject image
local PathDest = "C:/foto/rez/3132729095_2016923135120370_30014.png"
local PathSrc = "C:/foto/rez/3132729095_201695112114397_6186green2.png"
local idDest = c_readimg(PathDest, s_getex(PathDest), "seamless" )
local idSrc = c_readimg(PathSrc, s_getex(PathSrc) )
local exSave = "png"
s_injectImage( idDest, idSrc, 0,0, 5 )
local PathOut = string.format("%s/%s.%s", pic_dir_filtered, c_uniqid(), exSave )
c_selimg( idDest )

c_saveimg( exSave, PathOut )
c_delimg( idDest )	
c_delimg( idSrc )	
util.msg("finished")
]]--

--[[
local picImageLogo = c_readimg("C:/foto/3132729095_2016519174559895_12102.png", "png")	
c_scaleimage( 0.2,  0.2 )
c_saveimg( "png", "C:/foto/rez9/1_1.png" )
c_delimg( picImageLogo )
util.msg("finished")
]]--





-- for console
--c_f("#test.lua")
--c_f("#test_poisoned_quad_alpha_horisont.lua")
--c_f("image_filter_stainget_coffee.lua")
--c_f("p_image_filter_stainget_black.lua")
--c_f("buff_from_image_alpha.lua")
--c_f("tile_grunge.lua")
--c_f("add_logo.lua")
--c_f("p_fill_holes.lua")
--c_f("poisoned_image.lua")

--c_f("random_points_sqare.lua")




--[[
--randomise pixels

local PathIn = "C:/foto/rez/3132729095_201819132151440_19620.png"
local ext = s_getex(PathIn)
local PathOut = string.format("%s/%s.%s", pic_dir_filtered, c_uniqid(), ext )
s_randpixels( PathIn , PathOut, -100, 100,-100, 100,-100, 100 )
]]--


--[[
local PathOut = string.format("%s/%s.%s", pic_dir_filtered, c_uniqid(), "png" )
--function s_corktexture( width, height, pathDestIn, points, maxStep, offsetXIn, offsetYIn )
s_corktexture( 100,100, PathOut, 800, 10, 10,10 )
]]--




--[[
-- Add random images
-- "seamless" or "basic"
local PathIn = "C:/foto/rez/layer.png"
local Dir = "C:/foto/pattern/img"
local ext = s_getex(PathIn)
local PathOut = string.format("%s/%s.%s", pic_dir_filtered, c_uniqid(), ext )
s_addRandomImgOther( PathIn, "seamless", Dir, "png", PathOut, 50000, true )
]]--








--[[
-- Inrerplate lightness !!!!!!!!!!!!!!
local Path = "C:/foto/rez/3132729095_2016111716583732_6685.png"
local Path2 = "C:/foto/rez/3132729095_2016101917938789_28639gray_green_wall.png"
PathOut = string.format("%s/%s.%s", pic_dir_filtered, c_uniqid(), "png" )
s_changeLightnessImg ( Path, Path2, PathOut, 0.3 )
util.msg("s_changeLightnessImg finished")
]]--


-- Random !!!!!!!!!!!!!!
-- Adds images interplate from image in random pattern mode from buffer(image on alpha) 
-- "seamless" or "basic"

local PathRadPixels = "C:/foto/3132729095_2019829162913698_15333.png"
local Dir = "C:/foto/pattern/2"
local ext = "png"--s_getex(PathIn)
local PathOut = string.format("%s/%s.%s", pic_dir_filtered, c_uniqid(), ext )
local picImg = c_readimg( "C:/foto/rez/3132729095_201973015515664_6977.png", "png" )	
-- function s_addImgRandom( width, height, patternsPathIn, addImgEx, pathOutIn, quantity, palleteNameIn, imgId )
s_addImgRandom( 0, 0,  Dir, "png", PathOut, 300 , "lava2.png", picImg, false )
c_delimg(picImg)





--[[
local PathIn = "C:/foto/rez/3132729095_20191113102147906_25660.png"
PathOut = string.format( "%s/%s.%s", pic_dir_filtered, c_uniqid(), "png" )
--c_clearimage( 4, 0.1 )
s_priorcolor(PathIn, PathOut, 5, 0, 3 )
]]--





--[[
-- "seamless" or "basic"
local PathIn = "C:/foto/rez/3132729095_2016615172522981_13706.png"
local Dir = "C:/foto/rez8"
local ext = s_getex(PathIn)
local PathOut = string.format("%s/%s.%s", pic_dir_filtered, c_uniqid(), ext )
s_addRandomImgFromOther( PathIn, "seamless", Dir, "png", PathOut, 20, 0.5 )
]]--


				

--[[
-- "seamless" or "basic"
local PathIn = "C:/foto/clean.png"
local Dir = "C:/foto/rez9"
local ext = s_getex(PathIn)
local PathOut = string.format("%s/%s.%s", pic_dir_filtered, c_uniqid(), ext )

-- memory leaks scale &????????????????
s_addPatternImgOtherGradient( PathIn, "basic", Dir, "png", PathOut, 90 , 50*1.1, 1 )
--s_addPatternImgOtherGradient( PathIn, "basic", Dir, "png", PathOut, 80 , 50*2, 0 )
]]--




--c_executeshell("xcopy d:\\help.txt d:\\work")




--[[
--#Replace channels
util.msg("started")
local PathIn = "D:/LSD/896982166_201783017414975_10384red2.png"
--local PathIn = "D:/mya/textures/_A/test.JPG"
local ext = s_getex(PathIn)
-- param: 0 - grb, 1- gbr,  2 - brg, 3 - bgr, 4 - rbg 
local PathOut = string.format("%s/%s.%s", pic_dir_filtered, c_uniqid(), ext ) s_exchangeChannels( PathIn,  PathOut, 0 )
PathOut = string.format("%s/%s.%s", pic_dir_filtered, c_uniqid(), ext ) s_exchangeChannels( PathIn,  PathOut, 1 )
PathOut = string.format("%s/%s.%s", pic_dir_filtered, c_uniqid(), ext ) s_exchangeChannels( PathIn,  PathOut, 2 )
PathOut = string.format("%s/%s.%s", pic_dir_filtered, c_uniqid(), ext ) s_exchangeChannels( PathIn,  PathOut, 3 )
PathOut = string.format("%s/%s.%s", pic_dir_filtered, c_uniqid(), ext ) s_exchangeChannels( PathIn,  PathOut, 4 )
]]--



--[[
--c_f("image_filter_stainget_black.lua")
--c_f("add_logo.lua")
-- #delete pieces
--s_clearPicies( "C:/foto/rez/3132729095_20166210159902_17874.png","C:/foto/rez/3132729095_20166210159902_17874_1.png", 20 )
-- #fill holes
local PathOut = string.format("%s/%s.%s", pic_dir_filtered, c_uniqid(), "png" )
--local PathIn = "C:/foto/shutter/to upload/3132729095_20169182010419_146.png"
local PathIn = "F:/foto/tex/rez6/1059320600.png"
--local PathIn = "C:/foto/rez/3132729095_2016126175235512_19864.png"
--s_fillHoles(PathIn,PathOut, 3, 2 )
s_clearPicies( PathIn ,PathOut, 70 )	
util.msg("s_clearPicies finished")	
]]--

--[[
local PathIn = "C:/foto/rez/s42_3.jpg"
local PathChannelIn = "C:/foto/rez/3132729095_20161121111542311_24773.png"
local PathOut = string.format("%s/%s.%s", pic_dir_filtered, c_uniqid(), "png" )
local channel = 3
s_changeAlphaChannel( PathIn, PathOut,  PathChannelIn, channel, 1, 0,0.4, 0,0 )
]]--

-- s_saveBufferToFile
--[[ 
local idBuff  = s_bufffromImageAlpha("C:/foto/tile_grunge 100100_3132729095_20162311283000C64DB01396.png", "png")
c_fillholes(idBuff, 1, 1 )
s_saveBufferToFile(idBuff, false, "C:/foto/out.png")
]]--
--0,3931161002882851402114091028217


--[[
local PathIn = "C:/foto/rez/1_out_1.png"
local outExt = "png"
local PathOut = string.format("%s/%s.%s", pic_dir_filtered, c_uniqid(), outExt )
--s_clearImage( imgPathIn, imgRezPathIn, pixAroundIn, rIn, gIn, bIn, factorMaxIn )
s_clearImage(PathIn, PathOut, 8, 221,175,156, 0.4 )
]]--



-- Pattern row
-- Adds images interplate from image in pattern mode
-- "seamless" or "basic"
--local PathIn = "C:/foto/clean10k.png"

local PathRadPixels = "C:/foto/pyromid.png"
local Dir = "C:/foto/pattern/p30"

local ext = "png"--s_getex(PathIn)local 
PathOut = string.format("%s/%s.%s", pic_dir_filtered, c_uniqid(), ext )
local pallete = "13_red.png"
--local imagePathToInterp = "D:/LSD/364876478_2016822152245607_31057.png"
randomMoveStep = 5
--function s_addPatternImg( xSize, ySize, imgMaskIn, patternsPathIn, addImgEx, pathOut, between, maxRandOffset, palleteName, randAlphaIn, typeArrange )
--s_addPatternImg( 4096,2160, PathRadPixels, Dir, "png", PathOut, 63, 0 , pallete, false , 1 )
local channel = 5 -- [0]r,gbal
--s_addPatternImgScale( 5000,5000, PathRadPixels, Dir, "png", PathOut, 100, 0 , pallete, channel, 0.15 )
--s_addPatternImgChannel( 7000,7000, PathRadPixels, Dir, "png", PathOut, 100, 0 , pallete, 2, channel,  true )
--s_addPatternImgInterpImg( 6000,6000, PathRadPixels, Dir, "png", PathOut, 1, 0 , imagePathToInterp, false, true )

--function s_addPatternImgVert( xSize, ySize, countAddIn,  imgDirPath, addImgEx, pathOut, between, maxRandOffset, palleteName, invertIn)
--s_addPatternImgVert ( 3264,5000, 100, Dir, "png", PathOut, 8, 0 , pallete, true )

--s_addPatternImgVertRand ( 4096, 2160, 20, Dir, "png", PathOut, pallete, 0.1, false, true )
util.msg("Pattern row finished")








--[[
util.msg("start executing")
local PathIn = "C:/foto/rez/3132729095_201811995429175_9587_2.png"
local outExt = "png"
local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, c_uniqid(), outExt )

s_fillmask( PathIn, PathOut, 0,0,0 )
]]--




--c_f( "demo_regex.lua" )

--[[
-- #Interpolate to color
--local PathIn = "/home/mixa/foto/process/s29_3132729095_2016412133017475_27643_little.png"
local PathIn = "C:/foto/rez/3132729095_2018112294737272_5645.png"
local ext = s_getex(PathIn)
local PathOut = string.format("%s/%s.%s", pic_dir_filtered, c_uniqid(), ext )
util.msg("Rendering into: ", PathOut)
-- sun like
--local rVal, gVal,bVal = util.c_unpackcolor(0xe45700)
--s_interpToColor( PathIn,  PathOut, rVal, -1,bVal, 0.8 )
local rVal, gVal,bVal = util.unpackcolor(0xffffff)
s_interpToColor( PathIn, PathOut, rVal, gVal,bVal, 1.0 )
util.msg("Interpolate to color finished ")
]]--


--[[
-- #s_interpToColorChannel to color
local PathIn = "C:/foto/process/1-00001.png"
local ext = s_getex(PathIn)
local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, c_uniqid(), ext )
util.msg("Rendering into: ", PathOut)
-- sun like

local rVal, gVal,bVal = util.unpackcolor( 0xff0000 )
local channel = 0
--s_interpToColorChannelMap( PathInMap, PathIn, PathOut, rVal, gVal,bVal, channel, 1 , true)
s_interpToColorChannel( PathIn, PathOut, rVal, gVal,bVal, channel, 0, true)
util.msg("Interpolate to color finished ")

]]--


--[[
-- #s_interpToImageChannel to color
local PathIn = "C:/foto/process/1-00001.png"
local ext = s_getex(PathIn)
local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, c_uniqid(), ext )

local PathInterp = "C:/foto/3132729095_201812512202546_22901.png"
util.msg("Rendering into: ", PathOut)

local channel = 0
--s_interpToColorChannelMap( PathInMap, PathIn, PathOut, rVal, gVal,bVal, channel, 1 , true)
s_interpToImageChannel( PathIn, PathOut,PathInterp, channel, 1, true)
util.msg("Interpolate to color finished ")
]]--



--[[
local PathIn = "C:/foto/rez/896982166_2019451536015_25396.png"
--local pathIn = "C:/foto/rez/3132729095_2017122812211713_11920.png"
local ext = s_getex(PathIn)
local pathOut = string.format("%s/%s.%s", pic_dir_filtered, c_uniqid(), "png" )
s_channelToAlpha( PathIn, pathOut, 0, true )
]]--


--[[
-- #s_interpToColorsChannel to color
--local PathIn = "C:/foto/rez/3132729095_20171228122321822_21806_small.png"
local PathIn = "C:/foto/rez/3132729095_20171020111945580_27673.png"
local ext = s_getex(PathIn)
local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, c_uniqid(), ext )
util.msg("Rendering into: ", PathOut)
-- sun like
local rVal, gVal,bVal = util.unpackcolor( 0x11ff00 )
local rVal2, gVal2,bVal2 = util.unpackcolor( 0xc000000 )
local channel = 3
s_interpToColorsChannel( PathIn, PathOut, rVal, gVal,bVal, rVal2,gVal2,bVal2, channel, 1, false)
util.msg("Interpolate to color finished ")
]]--

--[[
-- #s_interpToColorChannel to color
local PathIn = "C:/foto/process/1-00001.png"
local PathInInterp = "C:/foto/indicator.png"
local ext = s_getex(PathIn)
local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, c_uniqid(), ext )
util.msg("Rendering into: ", PathOut)
-- sun like
local rVal, gVal,bVal = util.unpackcolor( 0xeff0060 )
--s_interpToColorChannelMap( PathInMap, PathIn, PathOut, rVal, gVal,bVal, channel, 1 , true)
s_interpToImageColor( PathIn, PathInInterp, PathOut, rVal, gVal,bVal, true )
util.msg("s_interpToImageColor to color finished ")
]]--





--[[
 imgSizeW = 2000 
 imgSizeH = 2000 
 
 
 PathImg = "C:/foto/rez/896982166_20184315746180_29193.png"
 
 local picImg = c_readimg( PathImg, "png" )	
 
imgSizeW = c_getwidth()
imgSizeH = c_getheight()
 
 local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, c_uniqid(), "png" )
 
 --s_genColorsRandTable(0,255, arrayOutSize )
 
 --local img1 = c_resizepallete( 4, imgSizeH, nil ) --c_readimg( "C:/foto/rez/3132729095_2018103115462373_4243.png", "png" )	
 local img1 = c_resizepallete( 200, imgSizeH, nil ) --c_readimg( "C:/foto/rez/3132729095_2018103115462373_4243.png", "png" )	
 util.msg("pallete 1 created")
 local img2 = c_resizepallete( 200, imgSizeH, nil ) --c_readimg( "C:/foto/rez/3132729095_2018103115451809_2416.png", "png" )	
 util.msg("pallete 2 created")
 
 local outImage = c_createimg( imgSizeW, imgSizeH , "basic" )
 c_fillalphaf( 1 )
local w = c_getwidth()
local h = c_getheight()				
		
local rimOld = nil 
local gimOld = nil 
local bimOld = nil
	
		
		
	for h_ = 0, h-1 do		
		
		c_selimg( img1 )
		local r1,g1,b1 = c_getpixelf( 0, h_ )
		
		c_selimg( img2 )		
		local r2,g2,b2 = c_getpixelf( 0, h_ )
		
		
		for w_ = 0, w -1 do	
		
		local ri =  util.cosinterp(r1,r2 , w_/imgSizeW)
		local gi =  util.cosinterp(g1,g2 , w_/imgSizeW)
		local bi =  util.cosinterp(b1,b2 , w_/imgSizeW)
		
		c_selimg( outImage )	
		c_setpixelf( w_, h_, ri,gi,bi, 1 )
		
		end		
end	
 
 c_delimg( img1 )
 c_delimg( img2 )
 
 c_selimg( outImage )
 c_saveimg( s_getex( PathOut ), PathOut )		
c_delimg( outImage )
c_delimg( picImg )

]]--

--[[
local DirectoryIn = "D:/mycut"
local arr = s_numsorted( DirectoryIn, "png" )

local ext = "png"
local DirectoryOut = "D:/mycut_out"
local factor = 0.15

local idPred = nil
local idNext = nil

for k,v in spairs( arr ) do
 
	--util.msg( v )
	local fileInFullPath = v --util.getfile( DirectoryIn, i ,ext , false )
	--local fileIn = util.getfile( DirectoryIn, i ,ext , true )			
	--util.msg( fileInFullPath )
	local filePred = nil
	if idPred == nil then
		filePred = v --util.getfile( DirectoryIn, i - 1 ,ext , false )
		idPred = c_readimg( filePred, ext )
	else
		c_delimg( idPred )
		util.msg( idPred )
		idPred = idNext				
	end
	
		
	idNext = c_readimg(  fileInFullPath, ext )	
	
	if filePred == nil then
		--end				
		--util.msg( idPred, " " , idNext  )		
		c_injectimg(idPred, idNext, 0,0, factor )
		c_selimg( idNext )			
		c_saveimg( ext, fileInFullPath )		
		
		
	end
	
end

c_delimg( idPred )	
c_delimg( idNext )
]]--



s_printTimePoint(begTime, 1)
	


-- pixel art resize 2000х2000
--s_priorcolor2( PathIn, PathOut, 4, 0, 10 )