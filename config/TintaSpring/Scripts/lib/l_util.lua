require "Scripts/lib/utf8"
require "Scripts/lib/utf8data"
require "paths"


--s_arrlength(T)
--s_exportpixels( PathIn, PathOut )
--spairs(t, order)
--s_round(num, idp)
--s_findValue( arrIn , VIn)
--s_isnumber(v)
--s_isstring(v)
--s_findNearest( arr , val )
--s_max3( val1, val2, val3 )
--s_getex( path )
--s_delex( path )
--s_getbrightness( fR , fG, gB )
--s_getpallete( name )
--s_getRandomcolor( arr )
--s_get_base_out_path()
--clamp
--CatRomSpline
--s_randColorNear_3( base, minDev, maxDev )
--s_randChannelNear( base, minDev, maxDev )	
--s_posByDirection4(curxIn, curyIn, dirIn, stepIn, wIn, hIn )
--s_posByDirection8(curxIn, curyIn, dirIn, stepIn, wIn, hIn )
--s_dist2( x1, y1, x2, y2 )
--s_dist3( x1, y1, z1, x2, y2, z2 )
--s_findColorNearest( arr , val )
--s_randItem( inArr, inArrSize )
--s_genRandTable(in_min_val, in_max_val, in_count )
--s_genColorsRandTable( minVal, maxVal, in_count )
--s_printTimePoint( begTime, pointNum )
--s_drawlogo( srcImgIg )
--s_resaveImage( pathIn , pathOut, extIn, exOut )
--s_fileOperParallel( _pathIn, pathOut, extOut, strRequest, sameName  )
--s_exchangeChannels( pathIn, pathOut, param )
--s_numsorted( PathIn, ext, fullPath  )
--s_randpixels( pathIn, pathDestIn , rDivMinIn, rDivMaxIn, gDivMinIn, gDivMaxIn, bDivMinIn, bDivMaxIn )
--s_corktexture( width, height, pathDestIn, points, maxStep, offsetXIn, offsetYIn )
--s_randpixelsFromArray(wIn, hIn, pathdestIn , arrayIn )
--s_randpixelsFromArrayMask(wIn, hIn, pathdestIn, maskPathIn, factorIn, invertIn, arrayIn )
--s_maskchannel( pathIn, pathOut , rIn,gIn,bIn, channel, factor, invert, bAlphaTestIn )
--s_fillmask( pathIn, pathOut , rIn,gIn,bIn )
--s_getDiscreteColorByFactor( colors, arrsizeIn, factor  )
--s_buildColorArray( nodeColors, sizeColors )
--s_changecolor( pathIn,pathOut,rIn,gIn,bIn, r2In,g2In,b2In )
--s_channelToAlpha( pathIn, pathOut, channelIn, invert, minVal, maxVal )
--s_grayscale( pathIn, pathOut )
--s_interpToColor( pathIn,pathOut,rIn,gIn,bIn, factor )
--s_interpToColorChannel( pathIn,pathOut, rIn,gIn,bIn, channelIn, addFactor, invert )
--s_interpToImageChannel( pathIn,pathOut,pathInterp, channelIn, addFactor, invert )
--s_interpToImageColor( pathIn, pathInterp, pathOut, rIn, gIn, bIn, equal )
--s_interpToColorsChannel( pathIn,pathOut, rIn,gIn,bIn, rIn2,gIn2,bIn2, channelIn, addFactor, invert )
--s_interpToColorChannelMap( pathIn,pathInMap,pathOut, rIn,gIn,bIn, channelIn, intensity, invert )
--s_shuffleArr( arr )
--s_getStringArrayId( pathIn, code, bInOneLine )	
--s_maskImage( pathBrushIn, pathOutIn,  pathToCutIn, xPosIn, yPosIn, bRandIn )
--s_unmaskImage( pathBrushIn, pathOutIn, pathToCutIn,  xPosIn, yPosIn, bRandIn )
--s_multiplyChannel( imgPathIn, imgRezPathIn, channelIn, factorIn, bIncrIn )
--s_multiplyChannelImg(imgPath1In, imgPathIn2, imgRezPathIn, channelIn, factorIn,  bIncrIn )
--s_resizeImage(imgSrcIn, imgPathRezIn, xSizeIn, ySizeIn)
--s_interpToArrColorsDiscreteChannel( pathIn, pathOut , colorsIn, factorIn, invertIn, channel )	
--s_interpToArrColorsAlpha( pathIn, pathOut, colorsIn,  factorIn, invertIn)	
--s_clearImage( imgPathIn, imgRezPathIn, pixAroundIn, rIn, gIn, bIn, factorMaxIn )
--s_discretecolor( PathIn, PathOut, arrayOutSize, palleteName, shuffle, invert, channel, savePallete )
--c_resizepallete( pallete, sizePallete, pathOut )
--s_poster( pathIn, pathOut, palletePath , outline , palleteSize)
--s_priorcolor ( PathIn, PathOut, around, factor, passes )
--s_priorcolor2 ( PathIn, PathOut, around, factor, passes )
--s_boxFromAlpha( imgPath )
--clearAnimation( shotsDir, fileEx, outDir )
--s_boxFromImgRed( imgPath, posWidth, posHeight )


kpi = 3.1415926535897932384626433832795
kpi2 = 6.283185307179586476925286766559
maxColorDist = 441.6729

--[[
	Returns array length - use #T instead
]]--
function s_arrlength(T)
  local countOut = 0
  for _ in pairs(T) do countOut = countOut + 1 end
  return countOut
end

function s_exportpixels( PathIn, PathOut )

	local imgId = image.read( PathIn, s_getex(PathIn) )		
	
	local w = image.width()
	local h = image.height()
	
	
	local file = io.open(PathOut, "w")
	
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
				
				local r,g,b,a = image.getpixelf( w_, h_) 																		
				
				if a > 0 then			
					local data = string.format( "%d,%d,%d,%f,%f,%f", w_, h_, 0, r,g,b )	
					util.msg(data)
					file:write(data)
					file:write("\n")
				end
				
			end		
	end
	file:close()
	image.erase(imgId)
end

function spairs(t, order)
    -- collect the keys
    local keys = {}
    for k in pairs(t) do keys[#keys+1] = k end

    -- if order function given, sort by it by passing the table and keys a, b,
    -- otherwise just sort the keys 
    if order then
        table.sort(keys, function(a,b) return order(t, a, b) end)
    else
        table.sort(keys)
    end

    -- return the iterator function
    local i = 0
    return function()
        i = i + 1
        if keys[i] then
            return keys[i], t[keys[i]]
        end
    end
end

function s_round(num, idp)
  local mult = 10^(idp or 0)
  return math.floor(num * mult + 0.5) / mult
end


--[[
	Returns position of the element in the array or -1
]]--
function s_findValue( arrIn , VIn)
	
	for i = 1, #arrIn do
		if arrIn[i] == VIn then
			return i
		end
	end
	
	return -1	
end

function s_isnumber(v)
	return type(v) == "number"
end

function s_isstring(v)
	return type(v) == "string"
end

--[[
	Find nearest value in the array and returns position 1...length
]]--
function s_findNearest( arr , val )

local v = 9007199254740994
local pos = nil
	 
	for i = 1, #arr do
		local  sub = math.abs(arr[i] - val)
		if  sub < v then
			v = sub
			pos = i
		end
	end
return pos
end



function s_max3( val1, val2, val3 )
 return math.max(math.max(val1, val2), val3)
end


--[[
 Returns extension part
]]--
function s_getex( path )
	return string.utf8sub( path , -3 ) 
end

--[[
 Deletes extension part
]]--
function s_delex( path )
	return string.utf8sub( path , 0, -5 ) 
end

--[[
	Returns brightness
]]--
function s_getbrightness( fR , fG, gB )
--Luminance (standard for certain colour spaces): (0.2126*R + 0.7152*G + 0.0722*B) [1]
--Luminance (perceived option 1): (0.299*R + 0.587*G + 0.114*B) [2]
--Luminance (perceived option 2, slower to calculate):  sqrt( 0.241*R^2 + 0.691*G^2 + 0.068*B^2 ) > sqrt( 0.299*R^2 + 0.587*G^2 + 0.114*B^2 )
	return ( 0.2126 * fR + 0.7152 * fG + 0.0722* gB )
end

--[[
	Creates pallete array from the image Wx1, where W is the color array size
]]--
function s_getpallete( name )	
	
	util.msg(name)
	local pallPath = string.format("%s/%s", palletes, name )
	util.msg(pallPath)
			
	local ext = s_getex(pallPath)
	
	--util.msg(pallPath, " ", ext)
	local pImg = image.read( pallPath, ext )
	
	local w = image.width()
	
	main.lassert( w > 0, "Wrong pallete image size name:", name, " path: ", pallPath )
	local arrRet = {}
	local wEnd = w - 1
	for w_ = 0, wEnd do	
	
		local r,g,b,a = image.getpixelb( w_, 0 )		
		arrRet[w_ + 1] = util.packcolor3( r, g, b )
		--util.msg("get ", r," ",g, " ",b)
	end
	
	image.erase( pImg )
	
	return arrRet;
end

function s_getpalletea( name )	
	
	util.msg(name)
	local pallPath = string.format("%s/%s", palletes, name )
	util.msg(pallPath)
			
	local ext = s_getex(pallPath)
	
	--util.msg(pallPath, " ", ext)
	local pImg = image.read( pallPath, ext )
	
	local w = image.width()
	
	main.lassert( w > 0, "Wrong pallete image size name:", name, " path: ", pallPath )
	local arrRet = {}
	local wEnd = w - 1
	for w_ = 0, wEnd do	
	
		local r,g,b,a = image.getpixelb( w_, 0 )		
		arrRet[w_ + 1] = util.packcolor4( r, g, b, a )
		--util.msg("get ", r," ",g, " ",b, " ", a)
		
		
		--r,g,b,a = util.unpackcolor4( w_, arrRet[1] )	
		--util.msg("unpack ", r," ",g, " ",b, " ", a)
	end
	
	image.erase( pImg )
	
	return arrRet;
end


function s_getRandomcolor( arr )

	local len = s_arrlength(arr)
	
	return arr[main.randint(1,len)]
end


local CROO  = -0.5
local CR01  = 1.5
local CR02  = -1.5
local CR03 = 0.5
local CR11 = -2.5
local CR10 = 1.0
local CR12 = 2.0
local CR13 = -0.5
local CR20 = -0.5
local CR21 = 0.0
local CR22 = 0.5
local CR23 = 0.0
local CR30 = 0.0
local CR31 = 1.0
local CR32 = 0.0
local CR33 = 0.0

function clamp( xC, a, b)
	
	--(x < a ? a : (x > b ? b : x));
	
	if xC < a then
		return a
	end
	
	if xC > b then
		return b
	end
	
	return xC
	
end


function CatRomSpline(xIn, nknots, knot )
	
	local span = 0
	
	local nspans = nknots - 3

    --factors of the cubic
	local cO
	local cl
	local c2
	local c3
	
    if nspans < 1 then
		return 0.0
	end
	
	if xIn == 1.0 then
		return knot[nknots - 1]
	end

	-- Find the appropriate 4-point span of the spline.
	xIn = clamp(xIn, 0, 1) * nspans;
	
	span = math.floor(xIn)
	
	if  span > nknots - 3 or span == nknots - 3  then
		span = nknots - 3
	end

	xIn = xIn - span
	
	--knot = knot + span
	util.msg(span)
	-- Evaluate factors at x using Horner’s method
	c3 = CROO*knot[span + 1] + CR01*knot[span + 2] + CR02*knot[span + 3] + CR03*knot[span + 4]
	c2 = CR10*knot[span + 1] + CR11*knot[span + 2] + CR12*knot[span + 3] + CR13*knot[span + 4]
	cl = CR20*knot[span + 1] + CR21*knot[span + 2] + CR22*knot[span + 3] + CR23*knot[span + 4]
	cO = CR30*knot[span + 1] + CR31*knot[span + 2] + CR32*knot[span + 3] + CR33*knot[span + 4]

	return ( (c3*xIn + c2)*xIn + cl )*xIn + cO
	
end


------------------------------------------------------------------------------------------------------------------------------------------------------------------------
function s_get_base_out_path()

	--str_day = os.date("%d")
	--str_mounth = os.date("%m")
	--str_year = os.date("%y")

	--base_path = string.format("%s%s%s%s%s%s%s%s", glob_base_path, "/", str_year, "_", str_mounth, "_", str_day , "/")
	base_path = string.format("%s%s%s%s",glob_base_path, "/","Molygon_TexSpring","/")
return base_path
end

------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- returns RGB color values based on one value with random deviation
function s_randColorNear_3( base, minDev, maxDev )

	
	main.lassert( minDev <=  maxDev , "s_randColorNear: ", "minDev >  maxDev" )		

	devs = {}
	
	for i = 1, 3 do
		devval = base + main.randint( minDev, maxDev )
		if devval > 255 then
			devval = 255
		end
		
		if devval < 0 then
			devval = 0		
		end
		
		devs[i] = devval
	end
	
return devs[1], devs[2], devs[3]

end

-- returns channel value values based on one value with random deviation
function s_randChannelNear( base, minDev, maxDev )	
	main.lassert( minDev <=  maxDev, "s_randChannelNear: ", "minDev >  maxDev" )		
	devval = math.max(0,math.min(base + main.randint( minDev, maxDev ), 255 ))
	
return devval

end--
------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--[[ 
	 return neibour pixel coordinate
	 dirIn: 1 - left, 2 - right, 3 - upp, 4 - down
 ]]--
function s_posByDirection4(curxIn, curyIn, dirIn, stepIn, wIn, hIn )

	if stepIn == nil then
		stepIn = 1
	end
	
	if dirIn == 1  then
		curxIn =  math.max(curxIn - stepIn,0)
	elseif dirIn == 2  then
		curxIn =  math.min(curxIn + stepIn, wIn - 1)
	elseif dirIn == 3  then
		curyIn =  math.max(curyIn - stepIn,0)
	elseif dirIn == 4  then
		curyIn = math.min(curyIn + stepIn, hIn - 1)
	end

	return curxIn, curyIn
end

--[[ 
	 return neibour pixel coordinate
	 dirIn: 1 - step left, 2 - right, 3 - upp, 4 - down, 5 - left up, 6 - right up, 7-left down, 8 - right down 
 ]]--
function s_posByDirection8(curxIn, curyIn, dirIn, stepIn, wIn, hIn )

			if stepIn == nil then
				stepIn = 1
			end
			
			if dirIn == 1  then
				curxIn =  math.max(curxIn - stepIn,0)
			elseif dirIn == 2  then
				curxIn =  math.min(curxIn + stepIn,wIn)
			elseif dirIn == 3  then
				curyIn =  math.max(curyIn - stepIn,0)
			elseif dirIn == 4  then
				curyIn =  math.min(curyIn + stepIn,hIn)
			elseif dirIn == 5  then
				curxIn =  math.max(curxIn - stepIn,0)
				curyIn =  math.max(curyIn - stepIn,0)
			elseif dirIn == 6  then
				curxIn =  math.min(curxIn + stepIn,wIn)
				curyIn =  math.max(curyIn - stepIn,0)
			elseif dirIn == 7  then
				curxIn =  math.max(curxIn - stepIn,0)
				curyIn =  math.min(curyIn + stepIn,hIn)
			elseif dirIn == 8  then
				curxIn =  math.min(curxIn + stepIn,wIn)
				curyIn =  math.min(curyIn + stepIn,hIn)
			end

			return curxIn, curyIn

end

------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--[[ 
	returns length between two points 
]]--
function s_dist2( x1, y1, x2, y2 )
	return math.sqrt(  util.pow( math.abs (x2 - x1) , 2 ) + util.pow( math.abs (y2 - y1), 2 ) )
end


--[[ 
	returns length between two points 
]]--
function s_dist3( x1, y1, z1, x2, y2, z2 )
	return math.sqrt(  util.pow( math.abs (x2 - x1) , 2 ) + util.pow( math.abs (y2 - y1), 2 ) + util.pow( math.abs (z2 - z1), 2 ) )
end



--[[
	Find nearest color in the array and returns position 1...length
	arr - array of the packed colors {0xffffff,...
	val - packed value color 0xffffff
	returns position and color
]]--
function s_findColorNearest( arr , val )

local v = 500 -- just in case > 360
local rezColor = val
local pos = nil	 
	for i = 1, #arr do	
		local rArr,gArr,bArr = util.unpackcolor3( arr[i] )		
		local r,g,b = util.unpackcolor3( val )
		local l = s_dist3( rArr,gArr,bArr, r,g,b )		
		if  l < v then
			v = l
			pos = i
			rezColor = arr[i]
		end
	end
return pos, rezColor
end


--[[ 
	returns random item from array
]]--
function s_randItem( inArr, inArrSize )
	rand = main.randint(1, inArrSize )
return inArr[rand]

end




--[[
	 function generates and sorts numbers on period
	 in_min_val - min val
	 in_max_val - max val
	 in_count - quantity
	 returns table
	 
	 using example:
	 tabPos = s_genRandTable( 0, 100, 5 )	
	 for count = 1, 5 do
		util.msg( tabPos[count] )
	 end
 ]]--
function s_genRandTable(in_min_val, in_max_val, in_count )

	
	main.lassert(in_max_val >= in_min_val and in_count >= 1, "s_genRandTable error: in_min_val ",in_min_val, " in_max_val ", in_max_val, " in_count ", in_count )		
	

	numbers = {}
	for count = 0, in_count do
		table.insert(numbers, main.randint(in_min_val, in_max_val))
	end

    table.sort(numbers)
	return numbers
end



--[[
	 function generates colors
	 
	 in_min_val - min val (0)
	 in_max_val - max val (255)
	 in_count - quantity
	 returns table
	 
	 using example:
	 tabPos = s_genRandTable( 0, 100, 5 )	
	 for count = 1, 5 do
		util.msg( tabPos[count] )
	 end
 ]]--
function s_genColorsRandTable( minVal, maxVal, in_count )
	main.lassert( minVal >= 0 and maxVal <= 255, "wrong range")
	numbers = {}
	for count = 0, in_count do
		local rRand = main.randint(minVal, maxVal)
		local gRand = main.randint(minVal, maxVal)
		local bRand = main.randint(minVal, maxVal)
		local packed = util.packcolor3( rRand, gRand, bRand )
		table.insert(numbers, packed )
	end
   
	return numbers
end



--[[ ------------------------------------------------------------------------------
	Prints time elapsed from begin time
	begTime - seconds value in begin
	pointNum = nil(optional) checkpoint value
	returns time elapsed
--]] ------------------------------------------------------------------------------
function s_printTimePoint( begTime, pointNum )

	
	main.lassert( begTime ~= nil, "s_printTimePoint: begTime == nil")
		
	
	
	tEnd = ( os.clock() - begTime)/60
	if	pointNum == nil then
		
		util.msg("Minutes elapsed: ", tEnd)
	else
		util.msg("Point: ",pointNum," Minutes elapsed: ",tEnd)
	end
	
	return tEnd
end



function s_resaveImage( pathIn , pathOut, extIn, exOut )
	
	local srcPath = pathIn 
	local picImg = image.read(srcPath,  extIn )

	image.select( picImg )

	local w = image.width()
	local h = image.height()

	local mainImage = image.create( w, h , "basic" )	
	image.fillb(255,255,255)
	image.select( mainImage )		

	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do					
				image.select( picImg )			
				local r,g,b,a = image.getpixelf( w_, h_) 
				
				image.select( mainImage )	
				image.setchannelf( w_, h_, 3, a )	
				image.setpixelf(w_, h_, r,g,b, 1 )				
			end		
	end

	image.select(mainImage)	
	image.save(exOut,pathOut)

	image.eraseall()
	util.msg( "*** resaving finished" )

end




--[[
  Execute file operation in parallel 
  If you need full paths of the files - put "#1" and "#2" for in and out files in to the request
  local strRequest = string.format( "c_executefunc( \"/filters/image_filters\", \"getStainBlack\",\"%s\", \"%s\", \"%s\", \"%s\") ", "#1", "#2", extIn, extOut ) 
]]--
function s_fileOperParallel( _pathIn, pathOut, extOut, strRequest, sameName  )
	
	local dirSrc = _pathIn 
	local pathDist = pathOut 
		
	local files  = util.countfiles( dirSrc, extIn )
	
	for i = 0, files - 1 do 
	
			local fileInFullPath = util.getfile( dirSrc, i ,extIn , false )
			local fileIn = util.getfile( dirSrc, i ,extIn , true )
			--util.msg(fileInFullPath)
			local nameOut = main.uniqid()
			
			if sameName == true then			
				nameOut = ""
			end
			
			local fileOutFullPath = string.format( '%s%s%s%s%s%s', pathDist, "/", s_delex(fileIn),nameOut,  ".", extOut )						
			--util.msg("FullPathIn: ",fileInFullPath )
			--util.msg("FullPathOut: ",fileOutFullPath )
			local strRequest = string.gsub( strRequest, "#1", fileInFullPath )
								
			strRequest = string.gsub( strRequest, "#2", fileOutFullPath )
			util.msg("s_fileOperParallel: ", strRequest)
			--c_executeparallel( strRequest, "")				
			main.ed( strRequest )				
				
	end


end --main







--[[
	param: 0 - grb, 1- gbr,  2 - brg, 3 - bgr, 4 - rbg 
]]--
function s_exchangeChannels( pathIn, pathOut, param )
	
	main.lassert( param >=0 and param < 5, "Wrong param value" )	
	
	local picImg = image.read(  pathIn, s_getex( pathIn ) )
	
	local w1 = image.width()
	local h1 = image.height()
	
	local mainImage = image.create( w1, h1 , "basic" )

	image.fillchannelf( 3,0 )
	
	for w_ = 0, w1 - 1 do			
			for h_ = 0, h1 - 1 do	
				image.select( picImg )				
				local r,g,b,a = image.getpixelf( w_, h_) 				

				image.select( mainImage )
				image.setchannelf( w_,h_,3, a )
				
				local rNew = r
				local gNew = g
				local bNew = b					
					
				if param == 0 then
					rNew = g
					gNew = r
					bNew = b	
				elseif param == 1 then				
					rNew = g
					gNew = b
					bNew = r				
				elseif param == 2 then
					rNew = b
					gNew = r
					bNew = g
				elseif param == 3 then				
					rNew = b
					gNew = g
					bNew = r					
				elseif param == 4 then
					rNew = r
					gNew = b
					bNew = g				
				end
				image.setpixelf( w_,h_, rNew,gNew,bNew, 1 )					
			end	
			
	end

	image.select( mainImage )
	image.save( s_getex( pathOut ), pathOut )
	image.erase( picImg )
	image.erase( mainImage )	
	util.msg("***Executing s_exchangeChannels() finished.")	
end




function s_numsorted( PathIn, ext, fullPath  )

	local filesStr = {}	
	
	local files  = util.countfiles( PathIn, ext )
	
	if fullPath == nil then
		fullPath = true
	end
	
	if files ~= nil and files > 2 then

		for i = 0, files - 1 do 		
		
				local fileInFullPath = util.getfile( PathIn, i ,ext , fullPath )				
				local num =  tonumber(string.match( fileInFullPath ,  "%d+" ))
				
				filesStr[num] = fileInFullPath				
				
		end					
		
	end	
	--[[for  i, val in ipairs(filesStr) do
		util.msg(val)
	end]]--
	
	
	return filesStr
end



function s_randcolorpoins( w,h, pathDestIn , backR, backG, backB, r,g,b , count )


	local outImage = image.create( w, h , "basic" )	
	image.fillb(backR, backG, backB)
	image.fillchannelf( 3,1 )
	
	for i = 0, count do						
		image.setpixelb( main.randint( 0, w-1 ), main.randint( 0, h-1 ), r,g,b, 1 )			
			
	end
	
	image.save( s_getex(pathDestIn), pathDestIn )
	
	image.erase( outImage )
	
	util.msg("finished")
end



function s_randpixelsFromArray(wIn, hIn, pathdestIn , arrayIn )
	
	local arrLen = s_arrlength( arrayIn )
	local outImage = image.create( wIn, hIn , "basic" )	

	local w = image.width()
	local h = image.height()	
	
	for w_ = 0, w - 1 do	
			for h_ = 0, h - 1 do				
				
				
				local pos = main.randint( 1, arrLen )	
				local color = arrayIn[pos]
				local r,g,b = util.unpackcolor3( color )
				
				image.setpixelb( w_, h_, r,g,b, 1 )				
									
				image.setchannelf( w_,h_,3, 1 )						
			end		
	end	
	image.save( s_getex(pathdestIn), pathdestIn )		
	image.erase( outImage )
	util.msg("s_randpixelsFromArray finished")
end


function s_randpixelsFromArrayMask(wIn, hIn, pathdestIn, maskPathIn, factorIn, invertIn, arrayIn )
	
	local arrLen = s_arrlength( arrayIn )
	local outImage = image.create( wIn, hIn , "basic" )	
	local w = image.width()
	local h = image.height()		
	
	local maskImg = s_maskchannel( maskPathIn, nil , 0,0,0, 4, factorIn, invertIn, false )
	local mw = image.width()
	local mh = image.height()	
	
	--local maskImg = image.read( maskPathIn, s_getex( maskPathIn ) )
	--local mw = image.width()
	--local mh = image.height()				
	local factorX = (mw - 1)/(w - 1)
	local factorY = (mh - 1)/(h - 1)
	for w_ = 0, w - 1 do	
			for h_ = 0, h - 1 do		
							
				image.select( maskImg )		
				
				local r,g,b,a = image.getpixelb( math.floor(w_ * factorX),math.floor(h_ * factorY) )					
				
					image.select( outImage )
					local maxRand = arrLen
					if a > 0.0 then					
						maxRand = math.ceil( maxRand / 3 )
					end
					
					local pos = main.randint( 1, maxRand )
					
					
					local color = arrayIn[pos]
					local r,g,b = util.unpackcolor3( color )
					
					image.setpixelb( w_, h_, r,g,b, 1 )				
										
					image.setchannelf( w_,h_,3, 1 )		
				
			end		
	end	
	image.select( outImage )
	image.save( s_getex(pathdestIn), pathdestIn )		
	image.erase( outImage )
	image.erase( maskImg )
	util.msg("finished")
end



--[[
	Get mask by channel
	channel: 0-r,1-g,2-b,3-a,4-light
	if pathOut is nil returns created image id	
]]--
function s_maskchannel( pathIn, pathOut , rIn,gIn,bIn, channel, factor, invert, bAlphaTestIn )
		
	local picImg = image.read( pathIn, s_getex(pathIn) )	
	
	local w = image.width()

	local h = image.height()

	local outImage = image.create( w, h , "basic" )	
	
	image.fillchannelf( 3,0 )
	
	local alphaFore = 1	
	local alphaBack = 0
	
	if invert == true then 
		util.msg("invert == true")
		alphaFore = 0	
		alphaBack = 1	
	end
	
	for w_ = 0, w-1 do	
		for h_ = 0, h-1 do		
		
			image.select( picImg )										
			
			local r,g,b,a = image.getpixelf( w_, h_ ) 
			
			local f = a -- default
			if channel ==  0 then
				f = r
			elseif channel ==  1 then
				f = g
			elseif channel ==  2 then
				f = b				
			elseif channel ==  3 then
				f = a				
			elseif channel ==  4 then
				local h,s,l = util.rgbtohsl( r,g,b )	
				f = l
			elseif channel ==  5 then					
				f = main.randfloat()
			end
			
			if bAlphaTestIn == false or a > 0.0 then 							
				image.select( outImage )										
				image.setpixelb( w_, h_, rIn,gIn,bIn, 1 )					
				
				if f < factor then						
					image.setchannelf(   w_, h_,3, alphaFore )				
				else						
					image.setchannelf(   w_, h_,3, alphaBack )				
				end		
			end				
		end		
	end
		
	image.erase( picImg )
	
	util.msg("***s_maskchannel finished")
	
	if pathOut ~= nil then
		image.select( outImage )	
		image.save( s_getex( pathOut ), pathOut )	
		image.erase( outImage )
	else	
		return outImage;
	end	
	
end

--[[
	Fills all pixels with alpha != 0		
]]--
function s_fillmask( pathIn, pathOut , rIn,gIn,bIn )
		
	local picImg = image.read( pathIn, s_getex(pathIn) )	
	
	local w = image.width()

	local h = image.height()

	local outImage = image.create( w, h , "basic" )	
	
	image.fillchannelf( 3,0 )	
	
	for w_ = 0, w-1 do	
		for h_ = 0, h-1 do		
		
			image.select( picImg )										
			
			local r,g,b,a = image.getpixelb( w_, h_ ) 				
			
			if a > 0.0 then 							
				image.select( outImage )										
				image.setchannelf( w_, h_,3, 1 )
				image.setpixelb( w_, h_, rIn,gIn,bIn, 1 )						
					
			end				
		end		
	end
		
	image.erase( picImg )
	
	util.msg("***s_fillmask finished")
	
	if pathOut ~= nil then
		image.select( outImage )	
		image.save( s_getex( pathOut ), pathOut )	
		image.erase( outImage )
	else	
		return outImage;
	end	
	
end




--[[
	Gets Discrete color from array by factor
	exm:
	colors[1] = 0x00ff00
	colors[2] = 0xff0f00
	colors[3] = 0xff0ff0
	size = 3	
	factor = 0.5 
	returns 2, colors[2]
]]--
function s_getDiscreteColorByFactor( colors, arrsizeIn, factor  )

	local color = 0
	local size = arrsizeIn --s_arrlength(colors)
	main.lassert( size >= 2 , "s_getDiscreteColorByFactor wrong array size" )		

	if factor == 1  then
		return size,colors[size]
	end

	if factor == 0 then
		return 2, colors[2]
	end
	
	local pos = math.floor( ( size ) * factor )		
	--util.msg( pos )	
	
	return pos + 1, colors[pos + 1]	
	--local relatFanc =  factor - ( 1 / ( size - 1 ) ) * ( pos - 1 )
	--local segmentFanc = 1 / ( size - 1 )	
	--util.msg(  relatFanc / ( ( 1 / segmentFanc ) * ( pos - 1 ) )   )	
end



function s_buildColorArrayOld( nodeColors, sizeColors )


local rez = {}
nodes = s_arrlength( nodeColors )

	arrR = {}
	arrG = {}
	arrB = {}
	
	arrR [1] = 0
	arrG [1] = 0
	arrB [1] = 0
	
	for i = 2, nodes do
		local rVal, gVal,bVal = util.unpackcolor3( nodeColors[i] )
		arrR [i] = util.bytetofloat( rVal )
		arrG [i] = util.bytetofloat( gVal )
		arrB [i] = util.bytetofloat( bVal )
		
		--[[c_addgeomprop1f( catromR, "knotes",  util.bytetofloat( rVal ) )
		c_addgeomprop1f( catromG, "knotes",  util.bytetofloat( gVal ) )
		c_addgeomprop1f( catromB, "knotes",  util.bytetofloat( bVal ) )
		]]--
	end
	local last = s_arrlength( arrR )
	arrR [last + 1] = 1.0
	arrG [last + 1] = 1.0
	arrB [last + 1] = 1.0
	last = s_arrlength( arrR )
	for k = 0, sizeColors - 1 do
		--[[local rCalc = c_calculate( catromR, k/sizeColors )		
		local gCalc =  c_calculate( catromG, k/sizeColors )
		local bCalc =  c_calculate( catromB, k/sizeColors )		
		]]--
		
		local rCalc = CatRomSpline( k/sizeColors, last, arrR )
		local gCalc = CatRomSpline( k/sizeColors, last, arrG )
		local bCalc = CatRomSpline( k/sizeColors, last, arrB )
		
		rez[ k + 1 ] = util.packcolor3( util.floattobyte(rCalc), util.floattobyte(gCalc), util.floattobyte(bCalc) )
	end

	return rez
end

function s_buildColorArray( nodeColors, sizeColors )

	
	local rez = {}
	nodes = s_arrlength( nodeColors )
	
	if sizeColors == nodes or sizeColors < nodes then	
		return nodeColors	
	end

	arrR = {}
	arrG = {}
	arrB = {}
	
	
	for i = 1, nodes do
		local rVal, gVal,bVal = util.unpackcolor3( nodeColors[i] )
		arrR [i] = rVal 
		arrG [i] = gVal 
		arrB [i] = bVal 
	end

	local step = sizeColors / ( nodes - 1 )
	
	for k = 1, sizeColors  do
		local pos = k - 1
		local gap = math.floor( pos / step )
		local factor = (k - math.floor( gap * step))/step
		--util.msg( i, " ", factor )
		
		--[[local rCalc = CatRomSpline( k/sizeColors, last, arrR )
		local gCalc = CatRomSpline( k/sizeColors, last, arrG )
		local bCalc = CatRomSpline( k/sizeColors, last, arrB )
		--]]
		local r  = util.cosinterp(arrR[gap+1], arrR[gap+2], factor )
		local g  = util.cosinterp(arrG[gap+1], arrG[gap+2], factor )
		local b  = util.cosinterp(arrB[gap+1], arrB[gap+2], factor )
		--util.msg(r, " ",g, " ",b)
		rez[ k ] = util.packcolor3(  math.floor(r), math.floor(g), math.floor(b) )
	end
	
	return rez
end

function s_buildColorArrayA( nodeColors, sizeColors )

	
	local rez = {}
	nodes = s_arrlength( nodeColors )
	
	if sizeColors == nodes or sizeColors < nodes then	
		return nodeColors	
	end

	arrR = {}
	arrG = {}
	arrB = {}
	arrA = {}
	
	for i = 1, nodes do
		local rVal, gVal,bVal, aVal = util.unpackcolor4( nodeColors[i] )
		arrR [i] = rVal 
		arrG [i] = gVal 
		arrB [i] = bVal 
		arrA [i] = aVal 
	end

	local step = sizeColors / ( nodes - 1 )
	
	for k = 1, sizeColors  do
		local pos = k - 1
		local gap = math.floor( pos / step )
		local factor = (k - math.floor( gap * step))/step
		--util.msg( i, " ", factor )
		
		--[[local rCalc = CatRomSpline( k/sizeColors, last, arrR )
		local gCalc = CatRomSpline( k/sizeColors, last, arrG )
		local bCalc = CatRomSpline( k/sizeColors, last, arrB )
		--]]
		local r  = util.cosinterp(arrR[gap+1], arrR[gap+2], factor )
		local g  = util.cosinterp(arrG[gap+1], arrG[gap+2], factor )
		local b  = util.cosinterp(arrB[gap+1], arrB[gap+2], factor )
		local a  = util.cosinterp(arrA[gap+1], arrA[gap+2], factor )
		--util.msg(r, " ",g, " ",b)
		rez[ k ] = util.packcolor4(  math.floor(r), math.floor(g), math.floor(b), math.floor(a) )
	end
	
	return rez
end


--[[
	Changes color to another
]]--
function s_changecolor( pathIn,pathOut,rIn,gIn,bIn, r2In,g2In,b2In )

	local exImage = s_getex( pathIn )	
	local picImg = image.read( pathIn, exImage )
	
	local w = image.width()
	local h = image.height()

	local outImage = image.create( w, h , "basic" )	
	
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				image.select( picImg )				
				local r,g,b,a = image.getpixelb( w_, h_ )	
				
				image.select( outImage )						
				image.setchannelb( w_,h_,3, a )
				if r == rIn and g == gIn and b == bIn then									
					image.setpixelb( w_,h_, r2In,g2In,b2In, 1 )
				else
					image.setpixelb( w_,h_, r,g,b, 1 )
				end
			end		
	end
	
	image.save( exImage, pathOut )
	image.erase( picImg )
	
	image.erase( outImage )
	util.msg("***s_changecolor finished")
end


--[[
	Changes channel to alpha
	channelIn:
	0  - r 
	1  - g
	2  - b
	3  - lightness
]]--
function s_channelToAlpha( pathIn, pathOut, channelIn, invert, minVal, maxVal )

	local exImage = s_getex( pathIn )	
	local picImg = image.read( pathIn, exImage )
	
	local w = image.width()
	local h = image.height()

	local minV = 0
	local maxV = 1
	if minVal ~= nil then
		minV = minVal
	end
	if maxVal ~= nil then
		maxV = maxVal
	end
	
	local outImage = image.create( w, h , "basic" )	
	image.fillchannelf( 3,0 )				
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				image.select( picImg )				
				local r,g,b,a = image.getpixelf( w_, h_ )	
				
				image.select( outImage )
				
				
				if channelIn == 0 then
					if invert == true then				
						a = 1 - r
					else
						a = r
					end
				elseif channelIn == 1 then
					if invert == true then				
						a = 1 - g
					else
						a = g
					end
				elseif channelIn == 2 then
				    if invert == true then				
						a = 1 - b
					else
						a = b
					end
				elseif channelIn == 3 then
				
				 local h,s,l = util.rgbtohsl( r,g,b )				
					if invert == true then				
						a = 1 - l
					else
						a = l
					end
				end
				
				if a > minV and a < maxV then
					image.setchannelf( w_,h_,3, a )		
				end			
				
								
				image.setpixelf( w_,h_, r,g,b, 1 )
			end		
	end
	
	image.save( s_getex( pathOut ), pathOut )
	image.erase( picImg )
	
	image.erase( outImage )
	util.msg("***s_channelToAlpha finished")
end

--[[
	Set RGB = light
	channelIn:
	0  - r 
	1  - g
	2  - b
	3  - lightness
]]--
function s_grayscale( pathIn, pathOut )

	local exImage = s_getex( pathIn )	
	local picImg = image.read( pathIn, exImage )
	
	local w = image.width()
	local h = image.height()

	
	local outImage = image.create( w, h , "basic" )	
	image.fillchannelf( 3,0 )				
	
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				image.select( picImg )
				
				local r,g,b,a = image.getpixelf( w_, h_ )	
				
				image.select( outImage )
				
				local h,s,l = util.rgbtohsl( r,g,b )		
				
				image.setchannelf( w_,h_,3, a )					
				image.setpixelf( w_,h_, l,l,l, 1 )
			end		
	end
	
	image.save( s_getex( pathOut ), pathOut )
	image.erase( picImg )
	
	image.erase( outImage )
	util.msg("***s_grayscale finished")
end

--[[
	Changes color to another
]]--
function s_interpToColor( pathIn,pathOut,rIn,gIn,bIn, factor )

	local exImage = s_getex( pathIn )
	local picImg = image.read( pathIn, exImage )
	
	
	local w = image.width()
	local h = image.height()

	local outImage = image.create( w, h , "basic" )	
	
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				image.select( picImg )				
				local r,g,b,a = image.getpixelb( w_, h_ )	
				
				image.select( outImage )						
				image.setchannelb( w_,h_, 3,a )												
				image.setpixelb( w_,h_, util.cosinterp(r,rIn,factor),util.cosinterp(g,gIn,factor),util.cosinterp(b,bIn,factor), 1 )			
				
			end		
	end
	image.erase( picImg )
	
	
--	util.msg("***s_interpToColor finished")
	if  s_isstring(pathOut) == true then	
		image.save( exImage, pathOut )
		image.erase( outImage )
	else
		return outImage
	end
end


--[[
	Changes color to another by channel factor
	channelIn - 0 - r
	channelIn - 1 - g
	channelIn - 2 - b
	channelIn - 3 - a	
	channelIn - 4 - light
	factor 0...1
]]--
function s_interpToColorChannel( pathIn,pathOut, rIn,gIn,bIn, channelIn, addFactor, invert )

	local picImg = nil	
	
	if  s_isstring(pathIn) == true then			
		picImg = image.read( pathIn,  s_getex( pathIn ) )
	else
		picImg = pathIn
	end
	image.select( picImg )
	local w = image.width()
	local h = image.height()
	
	local rInterp = util.bytetofloat(rIn)
	local gInterp = util.bytetofloat(gIn)
	local bInterp = util.bytetofloat(bIn)

	local outImage = image.create( w, h , "basic" )	
	
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				image.select( picImg )				
				local r,g,b,a = image.getpixelf( w_, h_ )	
				
				image.select( outImage )						
				image.setchannelf( w_,h_,3, a )		

				if channelIn ==  0 then
					if invert == true then
						factor = 1 - r					
					else
						factor = r					
					end
				elseif channelIn ==  1 then
					if invert == true then
						factor = 1 - g
					else
						factor = g
					end
				elseif channelIn ==  2 then
					if invert == true then
						factor = 1 - b
					else
						factor = b
					end
				elseif channelIn ==  3 then
					if invert == true then
						factor = 1 - a
					else
						factor = a
					end
				elseif channelIn ==  4 then
					local h,s,l = util.rgbtohsl( r,g,b )
					if invert == true then
						factor = 1 - l					
					else
						factor = l					
					end
				end		
				factor = factor * addFactor
				image.setpixelf( w_,h_, util.cosinterp(r,rInterp,factor),util.cosinterp(g,gInterp,factor),util.cosinterp(b,bInterp,factor), 1 )			
				
			end		
	end
	
	util.msg("***s_interpToColorChannel finished")
	
	
	if  s_isstring(pathIn) == true	then		
		image.erase( picImg )			
	end
		
	if pathOut ~= nil then
		image.select( outImage )
		image.save( s_getex( pathOut ), pathOut )
		image.erase( outImage )
	else
		return outImage
	end
	
end


--[[
	Changes color to another by channel factor
	channelIn - 0 - r
	channelIn - 1 - g
	channelIn - 2 - b
	channelIn - 3 - a	
	channelIn - 4 - light
	factor 0...1
]]--
function s_interpToImageChannel( pathIn,pathOut,pathInterp, channelIn, addFactor, invert )

	local picImg = nil	
	
	if  s_isstring(pathIn) == true then			
		picImg = image.read( pathIn,  s_getex( pathIn ) )
	else
		picImg = pathIn
	end
	image.select( picImg )
	local w = image.width()
	local h = image.height()
	
	local picImgInterp = nil
	
	if  s_isstring(pathInterp) == true then			
		picImgInterp = image.read( pathInterp, s_getex( pathInterp ) )	
	else
		picImgInterp = pathInterp
	end
	
	image.select( picImgInterp )
	
	local wInterp = image.width()
	local hInterp = image.height()
	
	

	local outImage = image.create( w, h , "basic" )	
	
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				image.select( picImg )				
				local r,g,b,a = image.getpixelf( w_, h_ )	
							
				-- take factor values from map
				image.select( picImgInterp )				
				local ri,gi,bi = image.getpixelf( w_% wInterp, h_% hInterp )			

				image.select( outImage )						
				image.setchannelf( w_,h_,3, a )	
				
				if channelIn ==  0 then
					if invert == true then
						factor = 1 - r					
					else
						factor = r					
					end
				elseif channelIn ==  1 then
					if invert == true then
						factor = 1 - g
					else
						factor = g
					end
				elseif channelIn ==  2 then
					if invert == true then
						factor = 1 - b
					else
						factor = b
					end
				elseif channelIn ==  3 then
					if invert == true then
						factor = 1 - a
					else
						factor = a
					end
				elseif channelIn ==  4 then
					local h,s,l = util.rgbtohsl( r,g,b )
					if invert == true then
						factor = 1 - l					
					else
						factor = l					
					end
				end		
				factor = factor * addFactor
				image.setpixelf( w_,h_, util.cosinterp(r,ri,factor),util.cosinterp(g,gi,factor),util.cosinterp(b,bi,factor), 1 )			
				
			end		
	end
	
	util.msg("***s_interpToImageChannel finished")
	
	
	if  s_isstring(pathIn) == true	then		
		image.erase( picImg )			
	end
	
	image.erase( picImgInterp )
		
	if pathOut ~= nil then
		image.select( outImage )
		image.save( s_getex( pathOut ), pathOut )
		image.erase( outImage )
	else
		return outImage
	end
	
end

--[[
	Replace one image woth pixels from another wheer pixel equal  rIn, gIn, bIn or not( equal == false )
	factor 0...1
]]--
function s_interpToImageColor( pathIn, pathInterp, pathOut, rIn, gIn, bIn, equal )

	if  s_isstring(pathIn) == true then			
		picImg = image.read( pathIn,  s_getex( pathIn ) )
	else
		picImg = pathIn
	end	
	
	image.select( picImg )	
	
	local w = image.width()
	local h = image.height()
	
	local picImgInterp = nil
	
	if  s_isstring(pathInterp) == true then			
		picImgInterp = image.read( pathInterp, s_getex( pathInterp ) )	
	else
		picImgInterp = pathInterp
	end
	
	image.select( picImgInterp )
	
	local wInterp = image.width()
	local hInterp = image.height()
	
	local x = math.min((wInterp - 1)/(w - 1),1)
	local y = math.min((hInterp - 1)/(h - 1),1)
	util.msg( x," ",y )
	
	local outImage = image.create( w, h , "basic" )	
	if equal == true or equal == nil then
		for w_ = 0, w-1 do	
				for h_ = 0, h-1 do		
				
					-- take color values
					image.select( picImg )				
					local rVal,gVal,bVal,aVal = image.getpixelb( w_, h_ )				
					
					-- take factor values from map
					image.select( picImgInterp )				
					local r,g,b,a = image.getpixelb( math.floor(w_ * x), math.floor(h_ * y) )
									
					image.select( outImage )				
						
					image.setchannelb( w_,h_, 3, aVal )		
					
					if rVal == rIn and gVal == gIn and bVal == bIn then
						image.setpixelb( w_,h_, r,g,b, 1 )			
					else
						image.setpixelb( w_,h_, rVal,gVal,bVal, 1 )			
					end
					
				end			
		end
	
	else
		for w_ = 0, w-1 do	
				for h_ = 0, h-1 do		
				
					-- take color values
					image.select( picImg )				
					local rVal,gVal,bVal,aVal = image.getpixelb( w_, h_ )				
					
					-- take factor values from map
					image.select( picImgInterp )				
					local r,g,b,a = image.getpixelb( math.floor(w_ * x), math.floor(h_ * y) )
									
					image.select( outImage )				
						
					image.setchannelf( w_,h_,3, aVal )		
					
					if rVal ~= rIn and gVal ~= gIn and bVal ~= bIn then
						image.setpixelb( w_,h_, r,g,b, 1 )			
					else
						image.setpixelb( w_,h_, rVal,gVal,bVal, 1 )			
					end
					
				end			
		end
	
	end
		
	if  s_isstring(pathIn) == true	then		
		image.erase( picImg )			
	end
	if  s_isstring(pathInterp) == true	then		
		image.erase( picImgInterp )			
	end	
	
	if pathOut ~= nil then
		image.select( outImage )
		image.save( s_getex( pathOut ), pathOut )
		image.erase( outImage )
	else
		return outImage
	end
	
	
	util.msg("***s_interpToImageColor finished")
	
end



--[[
	Changes color to another by channel factor
	channelIn - 0 - r
	channelIn - 1 - g
	channelIn - 2 - b
	channelIn - 3 - a	
	channelIn - 4 - light
	factor 0...1
]]--
function s_interpToColorsChannel( pathIn,pathOut, rIn,gIn,bIn, rIn2,gIn2,bIn2, channelIn, addFactor, invert )

	local picImg = nil	
	
	if  s_isstring(pathIn) == true then			
		picImg = image.read( pathIn,  s_getex( pathIn ) )
	else
		picImg = pathIn
	end
	image.select( picImg )
	local w = image.width()
	local h = image.height()
	
	local rInterp = util.bytetofloat(rIn)
	local gInterp = util.bytetofloat(gIn)
	local bInterp = util.bytetofloat(bIn)
	
	local rInterp2 = util.bytetofloat(rIn2)
	local gInterp2 = util.bytetofloat(gIn2)
	local bInterp2 = util.bytetofloat(bIn2)

	local outImage = image.create( w, h , "basic" )	
	
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				image.select( picImg )				
				local r,g,b,a = image.getpixelf( w_, h_ )	
				
				image.select( outImage )						
				image.setchannelf( w_,h_,3, a )		

				if channelIn ==  0 then
					if invert == true then
						factor = 1 - r					
					else
						factor = r					
					end
				elseif channelIn ==  1 then
					if invert == true then
						factor = 1 - g
					else
						factor = g
					end
				elseif channelIn ==  2 then
					if invert == true then
						factor = 1 - b
					else
						factor = b
					end
				elseif channelIn ==  3 then
					if invert == true then
						factor = 1 - a
					else
						factor = a
					end
				elseif channelIn ==  4 then
					local h,s,l = util.rgbtohsl( r,g,b )
					if invert == true then
						factor = 1 - l					
					else
						factor = l					
					end
				end		
				factor = factor * addFactor
				image.setpixelf( w_,h_, util.cosinterp(rInterp,rInterp2,factor),util.cosinterp(gInterp,gInterp2,factor),util.cosinterp(bInterp,bInterp2,factor), 1 )			
				
			end		
	end
	
	util.msg("***s_interpToColorChannel finished")
	
	
	if  s_isstring(pathIn) == true	then		
		image.erase( picImg )			
	end
		
	if pathOut ~= nil then
		image.select( outImage )
		image.save( s_getex( pathOut ), pathOut )
		image.erase( outImage )
	else
		return outImage
	end
	
end


--[[
	Changes color to another by channel factor, using map image
	
	channelIn - 0 - r
	channelIn - 1 - g
	channelIn - 2 - b
	channelIn - 3 - a	
	channelIn - 4 - light
]]--
function s_interpToColorChannelMap( pathIn,pathInMap,pathOut, rIn,gIn,bIn, channelIn, intensity, invert )

	if  s_isstring(pathIn) == true then			
		picImg = image.read( pathIn,  s_getex( pathIn ) )
	else
		picImg = pathIn
	end	
	
	image.select( picImg )	
	
	local w = image.width()
	local h = image.height()
	
	local picImgMap = nil
	
	if  s_isstring(pathInMap) == true then			
		picImgMap = image.read( pathInMap, s_getex( pathInMap ) )	
	else
		picImgMap = pathInMap
	end
	
	image.select( picImgMap )
	
	local wM = image.width()
	local hM = image.height()
	
	local x = (wM - 1)/(w - 1)
	local y = (hM - 1)/(h - 1)
	
	local rInterp = util.bytetofloat(rIn)
	local gInterp = util.bytetofloat(gIn)
	local bInterp = util.bytetofloat(bIn)

	local outImage = image.create( w, h , "basic" )	
	
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				-- take color values
				image.select( picImg )				
				local rVal,gVal,bVal,aVal = image.getpixelf( w_, h_ )				
				
				-- take factor values from map
				image.select( picImgMap )				
				local r,g,b,a = image.getpixelf( math.floor(w_ * x), math.floor(h_ * y) )
				
				image.select( outImage )				
					
				image.setchannelf( w_,h_,3, aVal )		

				if channelIn ==  0 then
					if invert == true then
						factor = 1 - r
					else
						factor = r
					end
						
				elseif channelIn ==  1 then
					if invert == true then
						factor = 1 - g
					else
						factor = g
					end
				elseif channelIn ==  2 then
					if invert == true then
						factor = 1 - b
					else
						factor = b
					end
				elseif channelIn ==  3 then
					if invert == true then	
						factor = 1 - a
					else
						factor = a
					end
				elseif channelIn ==  4 then
					local h,s,l = util.rgbtohsl( rVal,gVal,bVal )
					if invert == true then	
						factor = 1 - l					
					else
						factor = l					
					end
				end		
				factor = factor * intensity
				image.setpixelf( w_,h_, util.cosinterp(r,rInterp,factor),util.cosinterp(g,gInterp,factor),util.cosinterp(b,bInterp,factor), 1 )			
				
			end		
	end
		
	if  s_isstring(pathIn) == true	then		
		image.erase( picImg )			
	end
	if  s_isstring(pathInMap) == true	then		
		image.erase( picImgMap )			
	end	
	
	if pathOut ~= nil then
		image.select( outImage )
		image.save( s_getex( pathOut ), pathOut )
		image.erase( outImage )
	else
		return outImage
	end
	
	
	util.msg("***s_interpToColorChannelMap finished")
end

--[[
	shuffles array
]]--
function s_shuffleArr( arr )
	local size = s_arrlength(arr)
	--local arrOut = {}
	for i = 1, size do
		local posRand = main.randint(1,size)
		local val = arr[posRand]
		arr[posRand] = arr[i] 
		arr[i]  = val
	end	

end

--[[
	Reads text data from file and returns string vector id
	code: utf8, ascii
]]--
function s_getStringArrayId( pathIn, code, bInOneLine )	
	local idBox = main.readfile(pathIn, code, bInOneLine )
	return idBox
end


--[[
	Deletes pixels from image out of mask	
	pathBrushIn - path to the mask brush or nil for using xSizeIn and ySizeIn
]]--
function s_maskImage( pathBrushIn, pathOutIn,  pathToCutIn, xPosIn, yPosIn, bRandIn )
	
	
	local picBrushImg = nil 	
	if s_isstring( pathBrushIn ) == true then	
		picBrushImg = image.read( pathBrushIn, s_getex( pathBrushIn ) )	
	else
		picBrushImg = pathBrushIn
	end
	
	image.select( picBrushImg )
	
	local wB = image.width()
	local hB = image.height()
			
	local cutImg = 0
	if s_isstring( pathToCutIn ) == true  then	
		cutImg = image.read( pathToCutIn, s_getex(pathToCutIn) )	
	else
		cutImg = pathToCutIn
	end
	
	image.select( cutImg )
	
	local wC = image.width()
	local hC = image.height()
			
	local posX = xPosIn
	local posY = yPosIn
		
	if bRandIn == true then
		posX = main.randint( 0, wC )
		posY = main.randint( 0, hC )
	end
	
	local endW = math.min( wC, posX + wB )			
	local endH = math.min( hC, posY + hB )		
	
	local outImage = image.create( wC, hC , "basic" )
	
	image.fillchannelf( 3,0 )	
		
	for w_ = 0, endW - 1 do		
			for h_ = 0, endH - 1 do										
				image.select( picBrushImg )
				
				local r,g,b,a = image.getpixelf( w_, h_ )	
				
				if a > 0.0 then
					image.select( cutImg )					
					local rC,gC,bC,aC = image.getpixelf( posX + w_, posY + h_ )							
					
					image.select( outImage )	
					image.setpixelf( w_,h_, rC,gC,bC, aC )
					image.setchannelf( w_,h_,3, aC )																					
				end				
			end					
	end	
	
	if pathOutIn ~= nil then		
		image.select( outImage )
		image.save( s_getex( pathOutIn ), pathOutIn )		
		image.erase( outImage )
	end
	
	if s_isstring( pathBrushIn ) == true then
		image.erase( picBrushImg )
	end
	
	if s_isstring( pathToCutIn ) == true  then	
		image.erase( cutImg )	
	end
	
	util.msg("***s_maskImage finished")
	if pathOutIn == nil then 
		return outImage
	end
	
end




--[[
	Deletes pixles inside mask
    pathBrushIn - path to the mask
	idBrushIn - id of the mask image, if not nil pathBrushIn ignores 
	pathToCutIn - path to the image to be cut if idToCutIn is nil	
	idToCutIn- id of the image to be cut, if not nil pathToCutIn ignores
	bToFile - if true pathOutIn ignored
]]--
function s_unmaskImage( pathBrushIn, pathOutIn, pathToCutIn,  xPosIn, yPosIn, bRandIn )
		
	local picBrushImg = 0	
	if s_isstring(pathBrushIn) == true  then
		picBrushImg = image.read( pathBrushIn, s_getex(pathBrushIn) )
	else
		picBrushImg = idBrushIn
	end
	
	local wB = image.width()
	local hB = image.height()
	
	local cutImg = 0 
	if s_isstring(pathToCutIn) == true  then
		cutImg = image.read( pathToCutIn, s_getex(pathToCutIn) )			
	else
		cutImg = idToCutIn
	end
	
	local wC = image.width()
	local hC = image.height()
		
	
	local outImage = image.create( wB, hB , "basic" )
	image.fillchannelf( 3,0 )	
	
	local posX = xPosIn
	local posY = yPosIn
		
	if bRandIn == true then
		posX = main.randint(0, wC)
		posY = main.randint(0, hC)
	end
	
	local endW = 	math.min(wC,posX + wB)			
	local endH = 	math.min(hC,posY + hB)	
	
	for w_ = 0, endW-1 do	
			for h_ = 0, endH-1 do		
			
				image.select( picBrushImg )				
				local r,g,b,a = image.getpixelf( w_, h_ )				
	
				image.select( cutImg )						
				local rC,gC,bC,aC = image.getpixelf( posX, posY )					
				image.select( outImage )	
				image.setpixelf( w_,h_, rC,gC,bC, math.min(a,aC) )
				image.setchannelf( w_,h_,3, math.min(a,aC) )										
							
			end		
	end	
	
	if pathOutIn ~= nil then
		local exImage = s_getex( pathOutIn )
		image.select( outImage )
		image.save( exImage, pathOutIn )		
		image.erase( outImage )
	end
	
	image.erase( picBrushImg )	
	image.erase( cutImg )		
	
	util.msg("***s_unmaskImage finished")
	
	if pathOutIn == nil then
		return outImage
	end
end



--[[
	multiplies channel by factor 
	channelIn - 0 - r
	channelIn - 1 - g
	channelIn - 2 - b
	channelIn - 3 - a	
	channelIn - 4 - light
]]--
function s_multiplyChannel( imgPathIn, imgRezPathIn, channelIn, factorIn, bIncrIn )

	
	local imgId = image.read( imgPathIn, s_getex(imgPathIn) )	
	
	local w = image.width()
	local h = image.height()	
			
	local outImage = image.create( w, h , "basic" )
	image.fillchannelf( 3,1 )	
	local bIncr = false
	if bIncrIn ~= nil then
		bIncr = bIncrIn
	end
	
	
	for w_ = 0, w -1 do	
			for h_ = 0, h-1 do		
			
				image.select( imgId )				
				local r,g,b,a = image.getpixelf( w_, h_ )				
								
				local chFactor = 0
				if channelIn ==  0 then
					if bIncr == true then
						r =  math.min(r + ((1 - r) * factorIn),1 )										
					else
						r = math.min(r * factorIn,1)
					end
					
				elseif channelIn ==  1 then
					if bIncr == true then
						g =  math.min(g + ((1 - g) * factorIn),1)												
					else
						g = math.min(g * factorIn,1)
					end
				elseif channelIn ==  2 then
					if bIncr == true then
						b =  math.min( b + ((1 - b) * factorIn),1)
					else
						b = math.min(b * factorIn,1)
					end			
				elseif channelIn ==  3 then
					if bIncr == true then
						a = math.min(a + ((1 - a) * factorIn),1)												
					else
						a = math.min(a * factorIn,1)
					end	
				elseif channelIn ==  4 then
					local h,s,l = util.rgbtohsl( r,g,b )
					l = math.min(l * factorIn,1)					
					r,g,b = util.hsltorgb( h,s, l )					
				end		
				image.select( outImage )					
				
				image.setpixelf( w_,h_, r,g,b, a )													
				image.setchannelf( w_,h_,3, a )	
			end		
	end	
	
	if imgRezPathIn ~= nil then
		image.select( outImage )
		image.save( s_getex( imgRezPathIn ), imgRezPathIn )		
		image.erase( outImage )
	end
	
	image.erase( imgId )		
	
	if imgRezPathIn == nil then
		return outImage
	end
	util.msg("s_multiplyChannel finished")
	
end


--[[
	works like s_multiplyChannel instead of factor uses light from imgPathIn2
]]--
function s_multiplyChannelImg(imgPath1In, imgPathIn2, imgRezPathIn, channelIn, factorIn,  bIncrIn )

	
	local imgId1 = image.read( imgPath1In, s_getex(imgPath1In) )	
	
	
	local w1 = image.width()
	local h1 = image.height()	
	
	local imgId2 = image.read( imgPathIn2, s_getex(imgPathIn2) )
	
	local w2 = image.width()
	local h2 = image.height()	
		
	local wEnd = w1
	local hEnd = h1
	
	local outImage = image.create( wEnd, hEnd , "basic" )
	image.fillchannelf( 3,1 )	
	
	local bIncr = false
	if bIncrIn ~= nil then
		bIncr = bIncrIn
	end
	
	local factorX = (w2-1)/(w1-1)
	local factorY = (h2-1)/(h1-1)
	
	
	local factor = factorIn
					
	for w_ = 0, wEnd -1 do	
			for h_ = 0, hEnd-1 do		
			
				image.select( imgId2 )				
				local posX = w_ * factorX
				local posY = h_ * factorY
				
				--util.msgf(posX, " ",posY)
				
				local r2,g2,b2,a2 = image.getpixelf( math.floor(posX) , math.floor(posY) )				
				local h2,s2,l2 = util.rgbtohsl( r2,g2,b2 )	
				
				
				image.select( imgId1 )				
				local r1,g1,b1,a1 = image.getpixelf( w_, h_ )				
								
				local chFactor = 0
				if channelIn ==  0 then
					if bIncr == true then
						r1 =  util.cosinterp(r1,r1 + ((1 - r1) * l2) , factor)												
					else						
						r1 = util.cosinterp(r1,r1 * l2, factor)
					end
				elseif channelIn ==  1 then
					if bIncr == true then
						g1 =  util.cosinterp(g1,g1 + ((1 - g1) * l2)	, factor)											
					else
						g1 = util.cosinterp(g1,g1 * l2 ,factor)
					end
				elseif channelIn ==  2 then
					if bIncr == true then
						b1 = util.cosinterp(b1,b1 + ((1 - b1) * l2), factor)
					else
						b1 = util.cosinterp(b1,b1 * l2 , factor)
					end				
				elseif channelIn ==  3 then
					if bIncr == true then
						a1 = util.cosinterp(a1,a1 + ((1 - a1) * l2), factor)
					else
						a1 = util.cosinterp(a1,a1 * l2, factor)
					end		
				elseif channelIn ==  4 then
					local h,s,l = util.rgbtohsl( r1,g1,b1 )
					if bIncr == true then
						l =  util.cosinterp(l,l + ((1 - l) * l2)	, factor)
					else
						l = util.cosinterp(l, l * l2, factor)
					end		
					r1,g1,b1 = util.hsltorgb( h,s, l )									
				end			
				
				image.select( outImage )	
				
				image.setpixelf( w_,h_, r1,g1,b1, 1 )
				image.setchannelf( w_,h_,3, a1 )				
							
			end		
	end	
	
	if imgRezPathIn ~= nil then
		image.select( outImage )
		image.save( s_getex( imgRezPathIn ), imgRezPathIn )		
		image.erase( outImage )
	end
	
	image.erase( imgId1 )	
	image.erase( imgId2 )		
	
	if imgRezPathIn == nil then
		return outImage
	end
	util.msg("s_multiplyChannelImg finished")
end



function s_resizeImage(imgSrcIn, imgPathRezIn, xSizeIn, ySizeIn)

	main.lassert( xSizeIn> 0  and ySizeIn > 0 , "wrong result image size" )
	local imgId = 0
	if s_isstring(imgSrcIn) == true  then
		imgId = image.read( imgSrcIn, s_getex(imgSrcIn) )		
	else
		imgId = imgSrcIn
	end
	
	local w = image.width()
	local h = image.height()
	
		
	local outImage = image.create( xSizeIn, ySizeIn, "basic" )		
	local factorX = ( w - 1 )/( xSizeIn - 1 )
	local factorY = ( h - 1 )/( ySizeIn - 1 ) 
	
	for w_ = 0, xSizeIn - 1 do	
			for h_ = 0, ySizeIn - 1 do		
			
				image.select( imgId )	
				
				local r,g,b,a = image.getpixelb( math.floor(w_ * factorX), math.floor(h_ * factorY) )												
				image.select( outImage )
				
				image.setpixelb( w_,h_, r,g,b, 1 )
				image.setchannelb( w_,h_, 3, a )				
							
			end		
	end	
	
	
	
	if imgPathRezIn ~= nil then
		image.select( outImage )
		image.save( s_getex( imgPathRezIn ), imgPathRezIn )
		image.erase(outImage)
	else
		return outImage
	end
	
	
	
	
	if s_isstring(imgSrcIn) == true  then
		image.erase(imgId)
	end
	
	util.msg("s_resizeImage finished")
end




--[[
	channel - 0 - r
	channel - 1 - g
	channel - 2 - b
	channel - 3 - a
	channel - 4 - lightness
]]--
function s_interpToArrColorsDiscreteChannel( pathIn, pathOut , colorsIn, factorIn, invertIn, channel )	
	
	local exImage = s_getex(pathIn)
	
	local exImageOut = s_getex(pathOut)	
	
	local picImg = image.read( pathIn, exImage )	
	
	local w = image.width()

	local h = image.height()

	local outImage = image.create( w, h , "basic" )	
	
	local alphaFore = 1	
	local alphaBack = 0
	
	local arrSize = s_arrlength(colorsIn)
	util.msg( arrSize )
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				image.select( picImg )										
				
				local r,g,b,a = image.getpixelf( w_, h_ ) 
				
					
				local f = 0
				if channel ==  0 then
					f = r
				elseif channel ==  1 then
					f = g
				elseif channel ==  2 then
					f = b				
				elseif channel ==  3 then
					f = a				
				elseif channel ==  4 then
					local h,s,l = util.rgbtohsl( r,g,b )	
					f = l
				elseif channel ==  5 then					
					f = main.randfloat()				
				end
				
				if invertIn == true then
					f = 1 - f
				end
								
				image.select( outImage )			

				local pos, color = s_getDiscreteColorByFactor( colorsIn, arrSize, f )				
				local bR,bG,bB = util.unpackcolor3( color )
				
				local rMed = util.bytetofloat( bR )
				local gMed = util.bytetofloat( bG )
				local bMed = util.bytetofloat( bB )
	
				local vr = util.cosinterp( r,rMed, factorIn )				
				local vg = util.cosinterp( g,gMed, factorIn )
				local vb = util.cosinterp( b,bMed, factorIn )
				
				image.setpixelf( w_, h_, vr,vg,vb, 1 )					
									
				image.setchannelf(   w_, h_,3, a )								
			end		
	end
	
	image.save( exImageOut, pathOut )
	image.erase( picImg )
	
	image.erase( outImage )
	
end


--[[
	Builds array and interpolates by alpha value
]]--
function s_interpToArrColorsAlpha( pathIn, pathOut, colorsIn,  factorIn, invertIn)	
	
	local exImage = s_getex(pathIn)
	
	local exImageOut = s_getex(pathOut)	
	
	local picImg = image.read( pathIn, exImage )	
	
	local w = image.width()

	local h = image.height()

	local outImage = image.create( w, h , "basic" )	
	
	local alphaFore = 1	
	local alphaBack = 0
	
	
	local arrSize = s_arrlength(colorsIn)
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				image.select( picImg )										
				
				local r,g,b,a = image.getpixelf( w_, h_ ) 
				f = a
				
				
				
				if invertIn == true then
					f = 1 - f
				end
								
				image.select( outImage )			

				local pos, color = s_getDiscreteColorByFactor( colorsIn,arrSize, f )				
				local bR,bG,bB = util.unpackcolor3( color )
				
				local rMed = util.bytetofloat( bR )
				local gMed = util.bytetofloat( bG )
				local bMed = util.bytetofloat( bB )
	
				local vr = util.cosinterp( r,rMed, factorIn )				
				local vg = util.cosinterp( g,gMed, factorIn )
				local vb = util.cosinterp( b,bMed, factorIn )
				
				image.setpixelf( w_, h_, vr,vg,vb, 1 )					
									
				image.setchannelf(   w_, h_,3, a )								
			end		
	end
	
	image.save( exImageOut, pathOut )
	image.erase( picImg )
	
	image.erase( outImage )
	c_printmsg("***s_masklight finished")
end


function s_clearImage( imgPathIn, imgRezPathIn, pixAroundIn, rIn, gIn, bIn, factorMaxIn )

	
	local imgId = image.read( imgPathIn, s_getex(imgPathIn) )	
	
	local w = image.width()
	local h = image.height()	
			
	local outImage = image.create( w, h , "basic" )
	image.fillchannelf( 3,1 )	
		
		
	local pixAround = pixAroundIn * 2 + 1
	local factorMax = ( pixAround * pixAround ) - 1
				
	for w_ = 0, w -1 do	
			for h_ = 0, h-1 do		
			
				image.select( imgId )	
				
				local r,g,b,a = image.getpixelb( w_, h_ )									
				
				if r ~= rIn and g ~= gIn and b ~= bIn then 
				
					local countFactor = 0
					local wBegin =  math.max( 0, w_ - pixAround )
					local hBegin =  math.max( 0, h_ - pixAround )
					
					local wEnd =  math.min( w - 1, w_ + pixAround )
					local hEnd =  math.min( h - 1, h_ + pixAround )
					
					for wIn = wBegin, wEnd do	
						for hIn = hBegin, hEnd do							
							local rF,gF,bF = image.getpixelb( wIn, hIn )
							
							if rF == rIn and gF == gIn and bF == bIn then 
								countFactor = countFactor + 1
							end								
						end
					end
					
					if countFactor / factorMax >= factorMaxIn then
						r,g,b = rIn, gIn, bIn
					end
					
				end
				
				image.select( outImage )					
				
				image.setpixelb( w_,h_, r,g,b, 1 )													
				image.setchannelb( w_,h_, 3, a )			
			end		
	end	
	
	if imgRezPathIn ~= nil then
		image.select( outImage )
		image.save( s_getex( imgRezPathIn ), imgRezPathIn )		
		image.erase( outImage )
	end
	
	image.erase( imgId )		
	
	if imgRezPathIn == nil then
		return outImage
	end
	util.msg("s_clearimage finished")
	
end

-- outFileName nil
-- palleteName nil
-- savePallete nil
function s_discretecolor( PathIn, PathOut, arrayOutSize, palleteName, shuffle, invert, channel, savePallete )
	-- Get discrete color 
	--local PathIn = "D:/mya/textures/_A/IMG_6538.JPG"

	local outExt = outExt
	
	util.msg(palleteName)
	local nameColors = palleteName
	util.msg(nameColors)	

	local r = {}

	local arrColor = nil 

	if nameColors ~= nil then
		arrColor = s_getpallete(nameColors)
	end

	if arrColor == nil then
		arrColor = s_genColorsRandTable(0,255, arrayOutSize )
	end
	
	if savePallete == true then	
	
		local palleteImage = image.create( arrayOutSize, 1 , "basic" )			
		image.fillchannelf( 3,1 )	
		 for i = 1, arrayOutSize do
			local bR,bG,bB = util.unpackcolor3( arrColor[i] )
			
			image.setpixelb( i - 1, 0, bR,bG,bB, 1 )	
		 end
		 
		 local pathPallete = s_delex( PathOut )
		 pathPallete = string.format("%s_pallete.png", pathPallete)
		 image.save( "png", pathPallete )
		 image.erase( palleteImage )
		 
	end
	
	

	if shuffle == true then
		s_shuffleArr(arrColor)
	end

	
	arr = s_buildColorArray( arrColor, arrayOutSize )

	util.msg( "Rendering to: ", PathOut )

	local begTime = os.clock()
	--local dirEx = "png"
	--function s_interpToArrColorsDiscreteChannel( pathIn, pathOut , colorsIn, factorIn, invertIn, channel )
	s_interpToArrColorsDiscreteChannel( PathIn, PathOut, arr, 1.0, invert, channel )
	--s_interpToImagesByArray( PathIn, PathOut, arr, 0.5, false, 1, true, imgDir ,dirEx)
	
	local endTime = os.clock()
	util.msg( "Minutes elapsed: ",(endTime-begTime)/60 )
end


-- resizes pallete
-- pallete - path to the pallete or size for random pallete
-- sizePallete - new interpalated size of the pallete
-- if pathOut == nil returns image id
function c_resizepallete( pallete, sizePallete, pathOut )

	local PathOut = string.format("%s/%s.%s", pic_dir_filtered, main.uniqid(), "png" )
	local arrayOutSize = sizePallete
	
	local arrColor = nil 
	if s_isstring(pallete) == true then
		arrColor = s_getpallete( pallete )
	else 
		arrColor = s_genColorsRandTable(0,255, pallete ) 
	end
	local arrAllColors = s_buildColorArray( arrColor, arrayOutSize )

	local outImage = image.create( 1, arrayOutSize , "basic" )	
	
	image.fillchannelf( 3,1 )
	for h_ = 0, arrayOutSize - 1  do	
		local bR,bG,bB = util.unpackcolor3( arrAllColors[h_ + 1] )
		--util.msg( bR, " " , bG, " ", bB )
		image.setpixelb( 0, h_, bR,bG,bB, 1 )	
	end
	if pathOut ~= nil then
		image.save( s_getex( PathOut ), PathOut )		
		image.erase( outImage )
	else		
		return outImage
	end

end


function s_poster( pathIn, pathOut, palletePath , outline , palleteSize)

	local pallete = nil
	
	local imgIdCalc = image.read( pathIn, s_getex(pathIn) )	

	if palletePath == nil then
		util.msg( "pallete created" )
		-- good single objects pallete = image.getcolors( 30, 0.4 )	
		-- good foto bad contrast pallete = image.getcolors( 40, 0.2 )	
		-- good discrete colors   bear_hysteria pallete = image.getcolors( 10, 0.06 )	
		--pallete = image.getcolors( 40, 0.2 )	
		pallete = image.getcolors( palleteSize, 0.2 )	
	else		
		pallete = image.read( palletePath, s_getex(palletePath) )	
	end
	
	image.select( imgIdCalc )
	image.poster( pallete )
	
	c_priorcolor( 2, 0, 3  )
	
	image.clearstains( 6, 0 )
	
	image.select( pallete )
	
	-- outline
	if outline == true then
		local r,g,b = image.getpixelb(0,0)	
		image.select( imgIdCalc )
		c_priorcolor( 6, 0, 1, 0,115,0   )
	end
	image.select( imgIdCalc )
	

	image.save( "png",pathOut )

	--image.save( "png",PathOut )
	if palletePath == nil then
		image.erase(pallete)
	end
	--image.erase(imgId)
	image.erase(imgIdCalc)
end


function s_priorcolor ( PathIn, PathOut, around, factor, passes )

	local picImg = image.read( PathIn,  s_getex( PathIn ) )
	
	image.priorcolor( around, factor, passes  )
	
	
	image.save( s_getex( PathOut ), PathOut )		
	image.erase( picImg )	
end


function s_priorcolor2 ( PathIn, PathOut, around, factor, passes, r,g,b )

	local picImg = image.read( PathIn,  s_getex( PathIn ) )
	
	--c_priorcolor2( around, factor, passes  )
	
	--c_priorcolor( 2, 0, 3  )
	c_priorcolor2( around, factor, passes, r,g,b )
	

	--local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, main.uniqid(), "png")
	image.save( s_getex( PathOut ), PathOut )		
	image.erase( picImg )	
end




--[[
	returns filled  box  with pixel coordinates alpha > 0.0	
]]--
function s_boxFromAlpha( imgPath )

	local picImg = nil
	
	if s_isstring(imgPath) == true  then
		picImg = image.read( imgPath ,s_getex(imgPath) )
	else
		picImg = imgPath
	end	
	
	image.select(picImg)
	
	local w = image.width()
	local h = image.height()
	
	--local outImage = image.create( w, h , "basic" )	
	local boxId = box.create("vec3f_vector")
	
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				--image.select( picImg )				
				local r,g,b,a = image.getpixelb( w_, h_ )	
				
				if a > 0 then
					box.pushback3f( boxId, math.floor(w_), math.floor(h_), 0.0 )					
				end				
			end		
	end
	
	if s_isstring(imgPath) == true then
		image.erase( picImg )
	end
	
	
	util.msg("***s_boxFromAlpha finished")
	
	return  boxId	
end


--clearAnimation( "c:/foto/test", "png", "c:/foto/out" )
--[[
	clears images for gif animation
]]--
function clearAnimation( shotsDir, fileEx, outDir )

	local files  = util.countfiles( shotsDir, fileEx )	
	
	filesFull = s_numsorted( shotsDir, fileEx, false  )
	filesNotFull = s_numsorted( shotsDir, fileEx, true  )	
		
	main.lassert( s_arrlength(filesFull) > 2, "Files <= 2" )	
	
	-- first file is background
	local fileInFullPath = filesFull[1] --util.getfile( shotsDir, 0 ,fileEx , false )
	
	
	util.msg(fileInFullPath)
	local fileName = filesNotFull[1] --util.getfile( shotsDir, 0 ,fileEx , true )
	local before = image.read( fileInFullPath,  fileEx )
	util.msg(fileInFullPath)
	image.save( fileEx, string.format( "%s/%s", outDir, fileName )  )
	
	
	for i = 2, files do 
	
		fileInFullPath = filesFull[i] --util.getfile( shotsDir, i ,fileEx , false )
		util.msg(fileInFullPath)
		fileName = filesNotFull[i] --util.getfile( shotsDir, i ,fileEx , true )
		
		local current = image.read(fileInFullPath,  fileEx )		
		local toSave = image.read(fileInFullPath,  fileEx )
		
		
		
		local w1 = image.width()
		local h1 = image.height()		
		
		for w_ = 0, w1 - 1 do			
			for h_ = 0, h1 - 1 do	
			
				image.select( before )				
				local rB,gB,bB = image.getpixelb( w_, h_) 				
				
				image.select( current )				
				local rC,gC,bC = image.getpixelb( w_, h_) 				
				
				image.select( toSave )
				
				if 	rB ~= rC or gB ~= gC or bB ~= bC then
					image.setchannelf(w_, h_ ,3, 1.0)
					image.setpixelb( w_, h_ , rC,gC,bC, 1.0 )									
				else
					--util.msg(fileInFullPath, " image.setchannelf(w_, h_ , 0.0) ")
					image.setchannelf(w_, h_ ,3, 0.0 )					
					image.setpixelb( w_, h_ , 255,255,255, 1.0 )									
				end
			end
		end
		
		image.select( toSave )
		image.save( fileEx, string.format( "%s/%s", outDir, fileName ) )
		image.erase( toSave )
		image.erase( before )
		before = current	

	end
	image.erase( before )
		
end


--[[
	returns positions box from image with red markers
]]--
function s_boxFromImgRed( imgPath, posWidth, posHeight )

	local picImg = nil
	
	if s_isstring(imgPath) == true  then
		picImg = image.read( imgPath ,s_getex(imgPath) )
	else
		picImg = imgPath
	end	
	
	image.select(picImg)
	
	local w = image.width()
	local h = image.height()
	
	if posWidth == nil then 
		posWidth = w
	end
	
	if posHeight == nil then 
		posHeight = h
	end
	
	local boxid = box.create("int32_vector")
	
	
	for h_ = 0, h-1 do	
				for w_ = 0, w-1 do							
				
				local r,g,b,a = image.getpixelb( w_, h_ )					
				if r == 255 and g == 0 and b == 0  then					
				
					box.pushbacki32(boxid, math.floor((w_* posWidth)/w)  )
					box.pushbacki32(boxid, math.floor((h_* posHeight)/h) )					
					--util.msg(" ", math.floor((w_* posWidth)/w) , " ", math.floor((h_* posHeight)/h))
				end				
			end		
	end
	
	if s_isstring(imgPath) == true then
		image.erase( picImg )
	end
	
	
	
	return  boxid
end

--[[
	returns positions box from image with red markers
]]--
function s_boxFromImgRedSeamless( imgPath, posWidth, posHeight )

	local picImg = nil
	
	if s_isstring(imgPath) == true  then
		picImg = image.read( imgPath ,s_getex(imgPath) )
	else
		picImg = imgPath
	end	
	
	image.select(picImg)
	
	local w = image.width()
	local h = image.height()
	
	if posWidth == nil then 
		posWidth = w
	end
	
	if posHeight == nil then 
		posHeight = h
	end
	
	local boxId = box.create("int32_vector")
	
	
	for h_ = 0, h-1 do	
				for w_ = 0, w-1 do							
				
				local r,g,b,a = image.getpixelb( w_, h_ )					
				if r == 255 and g == 0 and b == 0  then					
				
					box.pushbacki32(boxId, math.floor((w_* posWidth)/w)  )
					box.pushbacki32(boxId, math.floor((h_* posHeight)/h) )	
					-- -x
					box.pushbacki32(boxId, math.floor((w_* posWidth)/w) - w  )
					box.pushbacki32(boxId, math.floor((h_* posHeight)/h) )
					-- +x
					box.pushbacki32(boxId, math.floor((w_* posWidth)/w) + w  )
					box.pushbacki32(boxId, math.floor((h_* posHeight)/h) )
					
					-- -y
					box.pushbacki32(boxId, math.floor((w_* posWidth)/w)  )
					box.pushbacki32(boxId, math.floor((h_* posHeight)/h) - h )
					-- +y
					box.pushbacki32(boxId, math.floor((w_* posWidth)/w)  )
					box.pushbacki32(boxId, math.floor((h_* posHeight)/h)  + h )
					
					--util.msg(" ", math.floor((w_* posWidth)/w) , " ", math.floor((h_* posHeight)/h))
				end				
			end		
	end
	
	if s_isstring(imgPath) == true then
		image.erase( picImg )
	end
	
	
	util.msg("***s_boxFromImgRedSeamless finished")
	
	return  box	
end

--[[
	draws the outline on the alpha edge  rO,gO,bO byte 255
]]--
function outlineAlphaPixel( imgPath, imgOut, rO,gO,bO,  layers )


	local imgId = image.read( imgPath, s_getex(imgPath) )	
	
	
	
	local w = image.width()
	local h = image.height()		
	local outImage = image.create( w, h , "basic" )
	image.fillchannelb( 3,0 )	
	
	
	for i = 1, layers do
		
		if imgId == nil then
			imgId = outImage
			outImage = image.create( w, h , "basic" )
			image.fillchannelb( 3,0 )	
		end
		
		for w_ = 0, w -1 do	
				for h_ = 0, h-1 do				
						
					image.select( imgId )
					local rCur,gCur,bCur,aCur = image.getpixelb( w_, h_ )									
					
					if aCur == 0 then
						r,g,b,aL = image.getpixelb( math.max(0,w_ - 1), h_ )
						r,g,b,aLU = image.getpixelb( math.max(0,w_ - 1), math.max(0,h_ - 1) )						
						r,g,b,aU = image.getpixelb( w_, math.max(0,h_ - 1) )
						r,g,b,aRU = image.getpixelb( math.min(w - 1, w_ + 1), math.max(0,h_ - 1) )
						r,g,b,aR = image.getpixelb( math.min(w - 1, w_ + 1), h_ )
						r,g,b,aRD = image.getpixelb( math.min(w - 1, w_ + 1), math.min(h - 1,h_ + 1) )
						r,g,b,aD = image.getpixelb( w_, math.min(h - 1,h_ + 1) )
						r,g,b,aLD = image.getpixelb( math.max(0,w_ - 1), math.min(h - 1,h_ + 1) )
						
						if aL ~= 0 or aLU ~= 0  or aU ~= 0 or aRU ~= 0 or aR ~= 0 or aRD ~= 0 or aD ~= 0 or aLD ~= 0 then
							--util.msg(i)
							image.select( outImage )
							image.setchannelb( w_,h_, 3, 255 )
							image.setpixelb( w_,h_, rO,gO,bO, 1) 						
						end
					else
							image.select( outImage )
							image.setchannelb( w_,h_, 3, 255 )
							image.setpixelb( w_,h_, rCur,gCur,bCur, 1 )
					end		
									
					--image.setchannelb( w_,h_, 3, a )			
				end		
		end	
		
		image.erase( imgId )
		imgId = nil
	end
	
	image.select( outImage )
	image.save( s_getex( imgOut ), imgOut )		
	
	image.erase( outImage )	

end

