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
nameColors = "2.png"
--local arrColor = s_getpallete(nameColors)
--r = s_buildColorArray( arrColor, 32)
--local outExt = "png"
--local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, main.uniqid(), outExt )
--s_randpixelsFromArray( 500,500, PathOut, r )
--s_randcolorpoins( 2000,2000, PathOut , 255, 255, 255, 255,0,0 , 400 )
--local maskPath = "D:/mya/textures/_A/IMG_6581.JPG" 
--function s_randpixelsFromArrayMask(wIn, hIn, pathdestIn, maskPathIn, factorIn, invertIn, arrayIn )
--s_randpixelsFromArray( 1400,1400, PathOut, r )


--[[
local outExt = "png"	
local coord = box.create( "vec3f_vector" )		
colors = 6 
w = 1000
h = 1000
for i = 1 , colors do
	box.pushback3f( coord, math.floor(math.random(0,w-1)), math.floor(math.random(0,h-1)), 0.0 )	
end
local palleteName = "6.png"
local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, main.uniqid(), outExt )
s_pointsGradient( PathOut, w, h, coord , palleteName, colors )
image.erase(coord)
]]--






local imageOffsetPath = "C:/foto/rez/1791818225_202141595735420_31105.png"
--s_vertgradientOffset( PathOut, imageOffsetPath, 0.071, 0, w, h, palleteName, arrayOutSize , false )
util.msg("Rendering into: ", PathOut)
local palleteName2 = "red_wall.png"
--s_vertgradient2Pallates( PathOut, imageOffsetPath, 0, w, h, palleteName, palleteName2, arrayOutSize, false )
-- Adds images tile, brick
-- "seamless" or "basic"
--local PathIn = "C:/foto/clean.png"
local Dir = "C:/foto/pattern/1"
local quantity = 10
local space  = 4
local typeArr = 0 --0 - tile
--s_addPatternImgOther( 2000, 2000, "seamless", Dir, "png", PathOut, quantity,space, typeArr , 1, false )

-- Add random images
-- "seamless" or "basic"
-- local PathIn = "C:/foto/rez/layer.png"
--s_addRandomImgOther( PathIn, "seamless", Dir, "png", PathOut, 50000, true )

local PathRadPixels = "C:/foto/bat.png"

local pallete = "6.png"
--local imagePathToInterp = "D:/LSD/364876478_2016822152245607_31057.png"
randomMoveStep = 5
between = 10
maxRandOffset = 0
-- function s_addPatternImg( xSize, ySize, imgMaskIn, patternsPathIn, addImgEx, pathOut, between, maxRandOffset, palleteName, randAlphaIn, typeArrange )

local channel = 5 -- [0]r,gbal
local Dir = "C:/foto/pattern"
--s_addPatternImgScale( 5000,5000, PathRadPixels, Dir, "png", PathOut, 100, 0 , pallete, channel, 0.15 )
--s_addPatternImgChannel( 7000,7000, PathRadPixels, Dir, "png", PathOut, 100, 0 , pallete, 2, channel,  true )
--s_addPatternImgInterpImg( 6000,6000, PathRadPixels, Dir, "png", PathOut, 1, 0 , imagePathToInterp, false, true )
--s_addPatternImgVert ( 1920,4000, 90	, Dir, "png", PathOut, 22, 0 , pallete, true )

--s_addPatternImgVertRand ( 4096, 2160, 20, Dir, "png", PathOut, pallete, 0.1, false, true )


--------------------------------------------









local PathIn = "c:/foto/#DSC08274.jpg"
local outExt = "png"
local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, main.uniqid(), outExt )
-- factor < 0.5 -> more sensetive to the light
--local rVal, gVal,bVal = util.c_unpackcolor3(0x2d4abf)
--s_maskchannel( pathIn, pathOut , rIn,gIn,bIn, channel, factor, invert, bAlphaTestIn )
--s_maskchannel( PathIn, PathOut, 0,0,0, 4, 0.5, false, true)





--local PathIn = "C:/foto/pixelart2.png"
--s_exportpixels( PathIn, "export.txt" )


--local PathIn = "d:/mya/IMG_20180914_141939_blur.jpg"--"d:/mya/IMG_20180621_182711_blur.jpg"
local PathIn = "c:/foto/h2.png"
local PathOut = string.format("%s/%s.%s", pic_dir_filtered, main.uniqid(), "png" )
local arrayOutSize = 240 
local palleteName = "3.png" -- may be null random then
local shuffle = false
local invert = true
local channel = 0	

palleteName = nil
--s_discretecolor( PathIn, PathOut, arrayOutSize, palleteName, shuffle, invert, channel )
--palleteName = nil
--1 step -- blurr first

--s_poster( PathIn, PathOut, palleteName, false, arrayOutSize ) --arrayOutSize, palleteName, shuffle, invert, channel, false )
--PathIn, PathOut, around, factor, passes,
-- clear image 
-- good after resizing comic  
--5  - for texture

PathIn = "C:/foto/3132729095_202181051655667_29611_p.png"
PathOut = string.format( "%s/%s.%s", pic_dir_filtered, "outline", "png" )


-- pixel art resize 2000х2000

--s_priorcolor(PathIn, PathOut, 4, 0.1, 10 )

--function s_priorcolor ( PathIn, PathOut, around, factor, passes, isOutline, rOut, gOut, bOut )
-- less around more passes - outline more accurate
-- good outline
--s_priorcolor2( PathIn, PathOut, 5, 0, 2, 0,255,0  ) 


--[[
local PathOut = string.format("%s/%s.%s", pic_dir_filtered, main.uniqid(), "png" )
local PathIn = "c:/foto/rez/3132729095_20161116173021302_21316.png"
local imgId = image.read( PathIn, s_getex(PathIn) )		
c_scaleimg( imgId, 400,400 , true )
image.save( s_getex( PathOut ), PathOut )
image.erase(imgId)
]]--

--[[
local outExt = "png"
local PathIn = "C:/foto/process/grid.png"
local PathOut = string.format("%s/%s.%s", pic_dir_filtered, main.uniqid(), outExt )
s_multiplyChannel( PathIn, PathOut, 1, 0.4, true )
]]--


--s_randpixels(PathIn,PathOut, -20, 20, -20, 20, -20, 20)

--[[
-- smooth cut
local outExt = "png"
local PathIn = "C:/foto/pen_test.jpg"
local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, main.uniqid(), outExt )
local tolerance = 0.8 -- 1 - max
local channel = 0
s_getStain( PathIn, PathOut, tolerance, channel )
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
local i = main.regextokenex( regex, textBox, 0	, 1 , "2", 1 )
--local i = main.regextoken( regex, "01/02/2003 blahblah 04/23/1999 blahblah 11/13/1981", "2", 1	)

local q = box.elements(i)
util.msg( "size: " , q )
for k = 0, q - 1 do
	util.msg(box.getvals(i,k))
end
image.erase(i)
image.erase(textBox)
]]--

--strs_box
--[[
local idBox = main.readfile("D:/mya/MolyWords/SpanishWords.txt", "utf8", true )
 --= box.create(box.create"str_vector")
--box.pushbacks( idBox, "El estúpido gordo cabrón" )
--val = box.getvals( idBox, 0 )
c_writetofile("D:/mya/MolyWords/SpanishWords2.txt", idBox )
--util.msg(val)
image.erase(idBox)
]]--


--[[
-- inject image
local PathDest = "C:/foto/rez/3132729095_2016923135120370_30014.png"
local PathSrc = "C:/foto/rez/3132729095_201695112114397_6186green2.png"
local idDest = image.read(PathDest, s_getex(PathDest), "seamless" )
local idSrc = image.read(PathSrc, s_getex(PathSrc) )
local exSave = "png"
s_injectImage( idDest, idSrc, 0,0, 5 )
local PathOut = string.format("%s/%s.%s", pic_dir_filtered, main.uniqid(), exSave )
image.select( idDest )

image.save( exSave, PathOut )
image.erase( idDest )	
image.erase( idSrc )	
util.msg("finished")
]]--

--[[
local picImageLogo = image.read("C:/foto/3132729095_2016519174559895_12102.png", "png")	
c_scaleimage( 0.2,  0.2 )
image.save( "png", "C:/foto/rez9/1_1.png" )
image.erase( picImageLogo )
util.msg("finished")
]]--













--[[
-- Inrerplate lightness !!!!!!!!!!!!!!
local Path = "C:/foto/rez/3132729095_2016111716583732_6685.png"
local Path2 = "C:/foto/rez/3132729095_2016101917938789_28639gray_green_wall.png"
PathOut = string.format("%s/%s.%s", pic_dir_filtered, main.uniqid(), "png" )
s_changeLightnessImg ( Path, Path2, PathOut, 0.3 )
util.msg("s_changeLightnessImg finished")
]]--


-- Random !!!!!!!!!!!!!!
-- Adds images interplate from image in random pattern mode from buffer(image on alpha) 
-- "seamless" or "basic"
--[[
local PathRadPixels = "C:/foto/3132729095_2019829162913698_15333.png"
local Dir = "C:/foto/pattern/2"
local ext = "png"--s_getex(PathIn)
local PathOut = string.format("%s/%s.%s", pic_dir_filtered, main.uniqid(), ext )
local picImg = image.read( "C:/foto/rez/3132729095_201973015515664_6977.png", "png" )	
-- function s_addImgRandom( width, height, patternsPathIn, addImgEx, pathOutIn, quantity, palleteNameIn, imgId )
s_addImgRandom( 0, 0,  Dir, "png", PathOut, 300 , "lava2.png", picImg, false )
image.erase(picImg)
]]--




--[[
local PathIn = "C:/foto/1.png"
PathOut = string.format( "%s/%s.%s", pic_dir_filtered, main.uniqid(), "png" )
local picImg = image.read( PathIn, "png" )	
image.clearstains( 40, 0.01 )
image.save( "png",PathOut )
--s_priorcolor(PathIn, PathOut, 5, 0, 3 )
image.erase(picImg)
]]--




--[[
-- "seamless" or "basic"
local PathIn = "C:/foto/rez/3132729095_2016615172522981_13706.png"
local Dir = "C:/foto/rez8"
local ext = s_getex(PathIn)
local PathOut = string.format("%s/%s.%s", pic_dir_filtered, main.uniqid(), ext )
s_addRandomImgFromOther( PathIn, "seamless", Dir, "png", PathOut, 20, 0.5 )
]]--


				

--[[
-- "seamless" or "basic"
local PathIn = "C:/foto/clean.png"
local Dir = "C:/foto/rez9"
local ext = s_getex(PathIn)
local PathOut = string.format("%s/%s.%s", pic_dir_filtered, main.uniqid(), ext )

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
local PathOut = string.format("%s/%s.%s", pic_dir_filtered, main.uniqid(), ext ) s_exchangeChannels( PathIn,  PathOut, 0 )
PathOut = string.format("%s/%s.%s", pic_dir_filtered, main.uniqid(), ext ) s_exchangeChannels( PathIn,  PathOut, 1 )
PathOut = string.format("%s/%s.%s", pic_dir_filtered, main.uniqid(), ext ) s_exchangeChannels( PathIn,  PathOut, 2 )
PathOut = string.format("%s/%s.%s", pic_dir_filtered, main.uniqid(), ext ) s_exchangeChannels( PathIn,  PathOut, 3 )
PathOut = string.format("%s/%s.%s", pic_dir_filtered, main.uniqid(), ext ) s_exchangeChannels( PathIn,  PathOut, 4 )
]]--



--[[
--c_f("image_filter_stainget_black.lua")
--c_f("add_logo.lua")
-- #delete pieces
--s_clearPicies( "C:/foto/rez/3132729095_20166210159902_17874.png","C:/foto/rez/3132729095_20166210159902_17874_1.png", 20 )
-- #fill holes
local PathOut = string.format("%s/%s.%s", pic_dir_filtered, main.uniqid(), "png" )
--local PathIn = "C:/foto/shutter/to upload/3132729095_20169182010419_146.png"
local PathIn = "F:/foto/tex/rez6/1059320600.png"
--local PathIn = "C:/foto/rez/3132729095_2016126175235512_19864.png"
--s_fillHoles(PathIn,PathOut, 3, 2 )
s_clearPicies( PathIn ,PathOut, 70 )	
util.msg("	s_clearPicies finished")	
]]--

--[[
local PathIn = "C:/foto/rez/s42_3.jpg"
local PathChannelIn = "C:/foto/rez/3132729095_20161121111542311_24773.png"
local PathOut = string.format("%s/%s.%s", pic_dir_filtered, main.uniqid(), "png" )
local channel = 3
s_changeAlphaChannel( PathIn, PathOut,  PathChannelIn, channel, 1, 0,0.4, 0,0 )
]]--



--[[
local PathIn = "C:/foto/rez/1_out_1.png"
local outExt = "png"
local PathOut = string.format("%s/%s.%s", pic_dir_filtered, main.uniqid(), outExt )
--s_clearImage( imgPathIn, imgRezPathIn, pixAroundIn, rIn, gIn, bIn, factorMaxIn )
s_clearImage(PathIn, PathOut, 8, 221,175,156, 0.4 )
]]--










--[[
util.msg("start executing")
local PathIn = "C:/foto/rez/3132729095_201811995429175_9587_2.png"
local outExt = "png"
local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, main.uniqid(), outExt )

s_fillmask( PathIn, PathOut, 0,0,0 )
]]--




--c_f( "demo_regex.lua" )


-- #Interpolate to color
--local PathIn = "/home/mixa/foto/process/s29_3132729095_2016412133017475_27643_little.png"
local PathIn = "C:/foto/rez/3132729095_2021823151543192_13215.png"
local ext = s_getex(PathIn)
local PathOut = string.format("%s/%s.%s", pic_dir_filtered, main.uniqid(), ext )

-- sun like
--local rVal, gVal,bVal = util.c_unpackcolor3(0xe45700)
--s_interpToColor( PathIn,  PathOut, rVal, -1,bVal, 0.8 )
--local rVal, gVal,bVal = util.unpackcolor3(0xffffff)
--s_interpToColor( PathIn, PathOut, 255, 0,0, 1.0 )




--[[
-- #s_interpToColorChannel to color
local PathIn = "C:/foto/process/1-00001.png"
local ext = s_getex(PathIn)
local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, main.uniqid(), ext )
util.msg("Rendering into: ", PathOut)
-- sun like

local rVal, gVal,bVal = util.unpackcolor3( 0xff0000 )
local channel = 0
--s_interpToColorChannelMap( PathInMap, PathIn, PathOut, rVal, gVal,bVal, channel, 1 , true)
s_interpToColorChannel( PathIn, PathOut, rVal, gVal,bVal, channel, 0, true)
util.msg("Interpolate to color finished ")

]]--


--[[
-- #s_interpToImageChannel to color
local PathIn = "C:/foto/process/1-00001.png"
local ext = s_getex(PathIn)
local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, main.uniqid(), ext )

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
local pathOut = string.format("%s/%s.%s", pic_dir_filtered, main.uniqid(), "png" )
s_channelToAlpha( PathIn, pathOut, 0, true )
]]--



-- #s_interpToColorsChannel to color
--local PathIn = "C:/foto/rez/3132729095_20171228122321822_21806_small.png"
local PathIn = "C:/foto/rez/interpolated_gpu.png"
local ext = s_getex(PathIn)
local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, main.uniqid(), ext )
--util.msg("Rendering into: ", PathOut)
-- sun like
--local rVal, gVal,bVal = util.unpackcolor3( 0x11ff00 )
--local rVal2, gVal2,bVal2 = util.unpackcolor3( 0xc000000 )
local channel = 0
--s_interpToColorsChannel( PathIn, PathOut, rVal, gVal,bVal, rVal2,gVal2,bVal2, channel, 1, false)


--s_interpToColorsByChannel( PathIn, PathOut, 0, 0, 0, 255, 10, 120, channel , 1 )



-- #s_interpToColorChannel to color
local PathIn = "C:/foto/rez/3132729095_2020112061235886_8717.png"
local PathInInterp = "C:/foto/rez/3132729095_2020123231412951_18891.png"
local ext = s_getex(PathIn)
local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, main.uniqid(), ext )
--util.msg("Rendering into: ", PathOut)
-- sun like
local rVal, gVal,bVal = util.unpackcolor3( 0xeff0060 )
--s_interpToColorChannelMap( PathInMap, PathIn, PathOut, rVal, gVal,bVal, channel, 1 , true)
--s_interpToImageColor( PathIn, PathInInterp, PathOut, 255, 0,0, true )
--s_interpToImage( PathIn, PathOut,  PathInInterp ,  1.0, false )







local PathIn = "C:/foto/rez/3132729095_202161601251_977.png"
--local pathIn = "C:/foto/rez/3132729095_2017122812211713_11920.png"
local ext = s_getex(PathIn)
local pathOut = string.format("%s/%s.%s", pic_dir_filtered, main.uniqid(), "png" )
--s_channelToAlpha( PathIn, pathOut, 0, true )




--[[
 imgSizeW = 2000 
 imgSizeH = 2000 
 
 
 PathImg = "C:/foto/rez/896982166_20184315746180_29193.png"
 
 local picImg = image.read( PathImg, "png" )	
 
imgSizeW = image.width()
imgSizeH = image.height()
 
 local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, main.uniqid(), "png" )
 
 --s_genColorsRandTable(0,255, arrayOutSize )
 
 --local img1 = c_resizepallete( 4, imgSizeH, nil ) --image.read( "C:/foto/rez/3132729095_2018103115462373_4243.png", "png" )	
 local img1 = c_resizepallete( 200, imgSizeH, nil ) --image.read( "C:/foto/rez/3132729095_2018103115462373_4243.png", "png" )	
 util.msg("pallete 1 created")
 local img2 = c_resizepallete( 200, imgSizeH, nil ) --image.read( "C:/foto/rez/3132729095_2018103115451809_2416.png", "png" )	
 util.msg("pallete 2 created")
 
 local outImage = image.create( imgSizeW, imgSizeH , "basic" )
image.fillchannelf( 3, 1 )
local w = image.width()
local h = image.height()				
		
local rimOld = nil 
local gimOld = nil 
local bimOld = nil
	
		
		
	for h_ = 0, h-1 do		
		
		image.select( img1 )
		local r1,g1,b1 = image.getpixelf( 0, h_ )
		
		image.select( img2 )		
		local r2,g2,b2 = image.getpixelf( 0, h_ )
		
		
		for w_ = 0, w -1 do	
		
		local ri =  util.cosinterp(r1,r2 , w_/imgSizeW)
		local gi =  util.cosinterp(g1,g2 , w_/imgSizeW)
		local bi =  util.cosinterp(b1,b2 , w_/imgSizeW)
		
		image.select( outImage )	
		image.setpixelf( w_, h_, ri,gi,bi, 1 )
		
		end		
end	
 
 image.erase( img1 )
 image.erase( img2 )
 
 image.select( outImage )
 image.save( s_getex( PathOut ), PathOut )		
image.erase( outImage )
image.erase( picImg )

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
		idPred = image.read( filePred, ext )
	else
		image.erase( idPred )
		util.msg( idPred )
		idPred = idNext				
	end
	
		
	idNext = image.read(  fileInFullPath, ext )	
	
	if filePred == nil then
		--end				
		--util.msg( idPred, " " , idNext  )		
		c_injectimg(idPred, idNext, 0,0, factor )
		image.select( idNext )			
		image.save( ext, fileInFullPath )		
		
		
	end
	
end

image.erase( idPred )	
image.erase( idNext )
]]--

--clearAnimation( "c:/foto/gif", "png", "c:/foto/out" )





--[[
c = 704 
for i = 0 , 75 do



	util.msg("[BridgeCoordMinMinus370Ind2_2]")
	util.msg("ID = ",c )
	util.msg("Type = 0")
	util.msg("DisplayedName = BridgeCoordMinMinus370Ind2_2")
	util.msg("Description = BridgeCoordMinMinus370Ind2_2")
	util.msg("ShowMessageBox = 0")
	util.msg("InvertedState = 0")
	util.msg("AlertValue = 0")
	util.msg("\n")
	
	c = c + 1

end
]]--

-- pixel art resize 2000х2000
-- s_priorcolor2( PathIn, PathOut, 6, 0, 5 )

s_printTimePoint( begTime, 1 )