require "Scripts/lib/utf8"
require "Scripts/lib/utf8data"
require "paths"



kpi = 3.1415926535897932384626433832795

kpi2 = 6.283185307179586476925286766559

--[[
	Returns array length - use #T instead
]]--
function s_arrlength(T)
  local countOut = 0
  for _ in pairs(T) do countOut = countOut + 1 end
  return countOut
end

function s_exportpixels( PathIn, PathOut )

	local imgId = c_readimg( PathIn, s_getex(PathIn) )		
	
	local w = c_getwidth()
	local h = c_getheight()
	
	
	local file = io.open(PathOut, "w")
	
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
				
				local r,g,b,a = c_getpixelf( w_, h_) 																		
				
				if a > 0 then			
					local data = string.format( "%d,%d,%d,%f,%f,%f", w_, h_, 0, r,g,b )	
					util.msg(data)
					file:write(data)
					file:write("\n")
				end
				
			end		
	end
	file:close()
	c_delimg(imgId)
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
	local pImg = c_readimg( pallPath, ext )
	
	local w = c_getwidth()
	
	c_assert( w > 0, "Wrong pallete image size name:", name, " path: ", pallPath )
	local arrRet = {}
	local wEnd = w - 1
	for w_ = 0, wEnd do	
		local r,g,b,a = c_getpixelb( w_, 0 )
		
		arrRet[w_ + 1] = util.packcolor( r, g, b )
	end
	
	c_delimg( pImg )
	
	return arrRet;
end


function s_getRandomcolor( arr )

	local len = s_arrlength(arr)
	
	return arr[c_randint(1,len)]
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

	
	c_assert( minDev <=  maxDev , "s_randColorNear: ", "minDev >  maxDev" )		

	devs = {}
	
	for i = 1, 3 do
		devval = base + c_randint( minDev, maxDev )
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
	c_assert( minDev <=  maxDev, "s_randChannelNear: ", "minDev >  maxDev" )		
	devval = math.max(0,math.min(base + c_randint( minDev, maxDev ), 255 ))
	
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
		curxIn =  math.min(curxIn + stepIn, wIn)
	elseif dirIn == 3  then
		curyIn =  math.max(curyIn - stepIn,0)
	elseif dirIn == 4  then
		curyIn = math.min(curyIn + stepIn, hIn)
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
function s_get_length( x1, y1, x2, y2 )
	return math.sqrt(  util.pow( math.abs (x2 - x1) , 2 ) + util.pow( math.abs (y2 - y1), 2 ) )
end


--[[ 
	returns length between two points 
]]--
function s_getlenXYZ( x1, y1, z1, x2, y2, z2 )
	return math.sqrt(  util.c_pow( math.abs (x2 - x1) , 2 ) + util.c_pow( math.abs (y2 - y1), 2 ) + util.c_pow( math.abs (z2 - z1), 2 ) )
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
		local rArr,gArr,bArr = util.unpackcolor( arr[i] )		
		local r,g,b = util.unpackcolor( val )
		local l = s_getlenXYZ( rArr,gArr,bArr, r,g,b )		
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
	rand = c_randint(1, inArrSize )
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

	
	c_assert(in_max_val >= in_min_val and in_count >= 1, "s_genRandTable error: in_min_val ",in_min_val, " in_max_val ", in_max_val, " in_count ", in_count )		
	

	numbers = {}
	for count = 0, in_count do
		table.insert(numbers, c_randint(in_min_val, in_max_val))
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
	c_assert( minVal >= 0 and maxVal <= 255, "wrong range")
	numbers = {}
	for count = 0, in_count do
		local rRand = c_randint(minVal, maxVal)
		local gRand = c_randint(minVal, maxVal)
		local bRand = c_randint(minVal, maxVal)
		local packed = util.packcolor( rRand, gRand, bRand )
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

	
	c_assert( begTime ~= nil, "s_printTimePoint: begTime == nil")
		
	
	
	tEnd = ( os.clock() - begTime)/60
	if	pointNum == nil then
		
		util.msg("Minutes elapsed: ", tEnd)
	else
		util.msg("Point: ",pointNum," Minutes elapsed: ",tEnd)
	end
	
	return tEnd
end


function s_drawlogo( srcImgIg )

	c_selimg( srcImgIg )
	local wSrc = c_getwidth() 
	local hSrc = c_getheight() 
	
	local picImageLogo = c_readimg( glob_logo_red_small_path , "png" )
	c_selimg( picImageLogo )
	local wLogo = c_getwidth() 
	local hLogo = c_getheight() 
	
	c_resizeimg( srcImgIg, 0,wSrc, 0, hSrc + hLogo )
	
	hSrc = hSrc + hLogo 
	
	local xOffset = 0
	local yOffset = 0

	if wSrc > wLogo and hSrc > hLogo then
		xOffset  = wSrc - wLogo
		yOffset  = hSrc - hLogo
	end
	
	c_injectimg(picImageLogo, srcImgIg, xOffset,yOffset)
	
	c_delimg( picImageLogo )
		
	c_selimg( srcImgIg )
end

function s_resaveImage( pathIn , pathOut, extIn, exOut )
	
	local srcPath = pathIn 
	local picImg = c_readimg(srcPath,  extIn )

	c_selimg( picImg )

	local w = c_getwidth()
	local h = c_getheight()

	local mainImage = c_createimg( w, h , "basic" )	
	c_fillimgb(255,255,255)
	c_selimg( mainImage )		

	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do					
				c_selimg( picImg )			
				local r,g,b,a = c_getpixelf( w_, h_) 
				
				c_selimg( mainImage )	
				c_setalphaf( w_, h_, a )	
				c_setpixelf(w_, h_, r,g,b, 1 )				
			end		
	end

	c_selimg(mainImage)	
	c_saveimg(exOut,pathOut)

	c_delallimg()
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
			local nameOut = c_uniqid()
			
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
			c_ed( strRequest )				
				
	end


end --main







--[[
	param: 0 - grb, 1- gbr,  2 - brg, 3 - bgr, 4 - rbg 
]]--
function s_exchangeChannels( pathIn, pathOut, param )
	
	c_assert( param >=0 and param < 5, "Wrong param value" )	
	
	local picImg = c_readimg(  pathIn, s_getex( pathIn ) )
	
	local w1 = c_getwidth()
	local h1 = c_getheight()
	
	local mainImage = c_createimg( w1, h1 , "basic" )

	c_fillalphaf( 0 )
	
	for w_ = 0, w1 - 1 do			
			for h_ = 0, h1 - 1 do	
				c_selimg( picImg )				
				local r,g,b,a = c_getpixelf( w_, h_) 				

				c_selimg( mainImage )
				c_setalphaf( w_,h_, a )
				
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
				c_setpixelf( w_,h_, rNew,gNew,bNew, 1 )					
			end	
			
	end

	c_selimg( mainImage )
	c_saveimg( s_getex( pathOut ), pathOut )
	c_delimg( picImg )
	c_delimg( mainImage )	
	util.msg("***Executing s_exchangeChannels() finished.")	
end




function s_numsorted( PathIn, ext  )

	local filesStr = {}	
	
	local files  = util.countfiles( PathIn, ext )

	if files ~= nil and files > 2 then

		for i = 1, files - 1 do 		
		
				local fileInFullPath = util.getfile( PathIn, i ,ext , false )				
				local num =  tonumber(string.match( fileInFullPath ,  "%d+" ))
				
				filesStr[num] = fileInFullPath				
				
		end					
		
	end	
	for  i, val in ipairs(filesStr) do
		util.msg(val)
	end
	
	return filesStr
end


function s_randpixels( pathIn, pathDestIn , rDivMinIn, rDivMaxIn, gDivMinIn, gDivMaxIn, bDivMinIn, bDivMaxIn )

	local picImg = 0 

	
	if s_isstring(pathIn) == true  then
		picImg = c_readimg( pathIn ,s_getex(pathIn) )
	else
		picImg = pathIn
	end
	
	
	local w = c_getwidth()
	local h = c_getheight()

	local outImage = c_createimg( w, h , "basic" )	
	
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				c_selimg( picImg )										
				
				local r,g,b,a = c_getpixelb( w_, h_) 													
				
				c_selimg( outImage )
				
				local outr = s_randChannelNear(r, rDivMinIn, rDivMaxIn )
				local outg = s_randChannelNear(g, gDivMinIn, gDivMaxIn )
				local outb = s_randChannelNear(b, bDivMinIn, bDivMaxIn )
				
				c_setpixelb( w_, h_, outr,outg,outb, 1 )				
									
				c_setalphaf( w_,h_, util.bytetofloat( a ) )				
				
			end		
	end
	
	c_saveimg( s_getex(pathDestIn), pathDestIn )
	
	if s_isstring(pathIn) == true then
		c_delimg( picImg )
	end
	
	c_delimg( outImage )
	util.msg("finished")
end

function s_corktexture( width, height, pathDestIn, points, maxStep, offsetXIn, offsetYIn )

	local picImg = 0 
	
	local offsetX = 0
	local offsetY = 0
	
	if offsetXIn ~= nil then
		offsetX = offsetXIn
	end
	
	if offsetYIn ~= nil then
		offsetY = offsetYIn
	end

	local picImg = c_createimg( width, height , "seamless" )
	
	c_fillimgb(255,255,255)
	c_fillalphaf( 1 )
	
	local w = c_getwidth()
	local h = c_getheight()	
	
	for i = 1, points do		

		local xPos = c_randint(offsetX, w - offsetX)
		local yPos = c_randint(offsetY, h - offsetY)
		c_setpixelb( xPos,yPos, 0,0,0, 1 )		
		
		for r = 1, maxStep do		
			
			xPos, yPos = s_posByDirection8( xPos, yPos, c_randint( 1,8 ), 1, w, h )	
			--xPos, yPos = s_posByDirection4( xPos, yPos, c_randint( 1,4 ), 1, w, h )	
			
			c_setpixelb( xPos, yPos, 0,0,0, 1 )											
		end
		--c_setalphaf( w_,h_, util.bytetofloat( a ) )				
	end
				
	c_saveimg( s_getex(pathDestIn), pathDestIn )
	c_delimg( picImg )	
	
	util.msg("s_corktexture")
end

function s_randpixelsFromArray(wIn, hIn, pathdestIn , arrayIn )
	
	local arrLen = s_arrlength( arrayIn )
	local outImage = c_createimg( wIn, hIn , "basic" )	

	local w = c_getwidth()
	local h = c_getheight()	
	
	for w_ = 0, w - 1 do	
			for h_ = 0, h - 1 do				
				
				
				local pos = c_randint( 1, arrLen )	
				local color = arrayIn[pos]
				local r,g,b = util.unpackcolor( color )
				
				c_setpixelb( w_, h_, r,g,b, 1 )				
									
				c_setalphaf( w_,h_, 1 )						
			end		
	end	
	c_saveimg( s_getex(pathdestIn), pathdestIn )		
	c_delimg( outImage )
	util.msg("s_randpixelsFromArray finished")
end


function s_randpixelsFromArrayMask(wIn, hIn, pathdestIn, maskPathIn, factorIn, invertIn, arrayIn )
	
	local arrLen = s_arrlength( arrayIn )
	local outImage = c_createimg( wIn, hIn , "basic" )	
	local w = c_getwidth()
	local h = c_getheight()		
	
	local maskImg = s_maskchannel( maskPathIn, nil , 0,0,0, 4, factorIn, invertIn, false )
	local mw = c_getwidth()
	local mh = c_getheight()	
	
	--local maskImg = c_readimg( maskPathIn, s_getex( maskPathIn ) )
	--local mw = c_getwidth()
	--local mh = c_getheight()				
	local factorX = (mw - 1)/(w - 1)
	local factorY = (mh - 1)/(h - 1)
	for w_ = 0, w - 1 do	
			for h_ = 0, h - 1 do		
							
				c_selimg( maskImg )		
				
				local r,g,b,a = c_getpixelb( math.floor(w_ * factorX),math.floor(h_ * factorY) )					
				
					c_selimg( outImage )
					local maxRand = arrLen
					if a > 0.0 then					
						maxRand = math.ceil( maxRand / 3 )
					end
					
					local pos = c_randint( 1, maxRand )
					
					
					local color = arrayIn[pos]
					local r,g,b = util.unpackcolor( color )
					
					c_setpixelb( w_, h_, r,g,b, 1 )				
										
					c_setalphaf( w_,h_, 1 )		
				
			end		
	end	
	c_selimg( outImage )
	c_saveimg( s_getex(pathdestIn), pathdestIn )		
	c_delimg( outImage )
	c_delimg( maskImg )
	util.msg("finished")
end



--[[
	Get mask by channel
	channel: 0-r,1-g,2-b,3-a,4-light
	if pathOut is nil returns created image id	
]]--
function s_maskchannel( pathIn, pathOut , rIn,gIn,bIn, channel, factor, invert, bAlphaTestIn )
		
	local picImg = c_readimg( pathIn, s_getex(pathIn) )	
	
	local w = c_getwidth()

	local h = c_getheight()

	local outImage = c_createimg( w, h , "basic" )	
	
	c_fillalphaf( 0 )
	
	local alphaFore = 1	
	local alphaBack = 0
	
	if invert == true then 
		util.msg("invert == true")
		alphaFore = 0	
		alphaBack = 1	
	end
	
	for w_ = 0, w-1 do	
		for h_ = 0, h-1 do		
		
			c_selimg( picImg )										
			
			local r,g,b,a = c_getpixelf( w_, h_ ) 
			
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
				f = c_randfloat()
			end
			
			if bAlphaTestIn == false or a > 0.0 then 							
				c_selimg( outImage )										
				c_setpixelb( w_, h_, rIn,gIn,bIn, 1 )					
				
				if f < factor then						
					c_setalphaf(   w_, h_, alphaFore )				
				else						
					c_setalphaf(   w_, h_, alphaBack )				
				end		
			end				
		end		
	end
		
	c_delimg( picImg )
	
	util.msg("***s_maskchannel finished")
	
	if pathOut ~= nil then
		c_selimg( outImage )	
		c_saveimg( s_getex( pathOut ), pathOut )	
		c_delimg( outImage )
	else	
		return outImage;
	end	
	
end

--[[
	Fills all pixels with alpha != 0		
]]--
function s_fillmask( pathIn, pathOut , rIn,gIn,bIn )
		
	local picImg = c_readimg( pathIn, s_getex(pathIn) )	
	
	local w = c_getwidth()

	local h = c_getheight()

	local outImage = c_createimg( w, h , "basic" )	
	
	c_fillalphaf( 0 )	
	
	for w_ = 0, w-1 do	
		for h_ = 0, h-1 do		
		
			c_selimg( picImg )										
			
			local r,g,b,a = c_getpixelb( w_, h_ ) 				
			
			if a > 0.0 then 							
				c_selimg( outImage )										
				c_setalphaf( w_, h_, 1 )
				c_setpixelb( w_, h_, rIn,gIn,bIn, 1 )						
					
			end				
		end		
	end
		
	c_delimg( picImg )
	
	util.msg("***s_fillmask finished")
	
	if pathOut ~= nil then
		c_selimg( outImage )	
		c_saveimg( s_getex( pathOut ), pathOut )	
		c_delimg( outImage )
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
	c_assert( size >= 2 , "s_getDiscreteColorByFactor wrong array size" )		

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



function s_buildColorArray( nodeColors, sizeColors )


local rez = {}
nodes = s_arrlength( nodeColors )
local catromR = c_creategeom( "geom_catrom" )	
local catromG = c_creategeom( "geom_catrom" )	
local catromB = c_creategeom( "geom_catrom" )	

--c_addgeomprop1f( catromR, "knotes",  0    )
--c_addgeomprop1f( catromG, "knotes",  0    )
--c_addgeomprop1f( catromB, "knotes",  0    )

for i = 1, nodes do
	local rVal, gVal,bVal = util.unpackcolor( nodeColors[i] )
	c_addgeomprop1f( catromR, "knotes",  util.bytetofloat( rVal ) )
	c_addgeomprop1f( catromG, "knotes",  util.bytetofloat( gVal ) )
	c_addgeomprop1f( catromB, "knotes",  util.bytetofloat( bVal ) )
end
--c_addgeomprop1f( catromR, "knotes",  0    )
--c_addgeomprop1f( catromG, "knotes",  0    )
--c_addgeomprop1f( catromB, "knotes",  0    )

	for k = 0, sizeColors - 1 do
		local rCalc = c_calculate( catromR, k/sizeColors )		
		local gCalc =  c_calculate( catromG, k/sizeColors )
		local bCalc =  c_calculate( catromB, k/sizeColors )

		--util.msg(rCalc, " ", gCalc, " ", bCalc )
		
		rez[ k + 1 ] = util.packcolor( util.floattobyte(rCalc), util.floattobyte(gCalc), util.floattobyte(bCalc) )
	end
	
c_delgeom(catromR)		
c_delgeom(catromG)		
c_delgeom(catromB)	
--[[
for i = 1, sizeColors do
	util.msg( rez[i] )
end
]]--

return rez
end


--[[
	Changes color to another
]]--
function s_changecolor( pathIn,pathOut,rIn,gIn,bIn, r2In,g2In,b2In )

	local exImage = s_getex( pathIn )	
	local picImg = c_readimg( pathIn, exImage )
	
	local w = c_getwidth()
	local h = c_getheight()

	local outImage = c_createimg( w, h , "basic" )	
	
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				c_selimg( picImg )				
				local r,g,b,a = c_getpixelb( w_, h_ )	
				
				c_selimg( outImage )						
				c_setalphab( w_,h_, a )
				if r == rIn and g == gIn and b == bIn then									
					c_setpixelb( w_,h_, r2In,g2In,b2In, 1 )
				else
					c_setpixelb( w_,h_, r,g,b, 1 )
				end
			end		
	end
	
	c_saveimg( exImage, pathOut )
	c_delimg( picImg )
	
	c_delimg( outImage )
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
	local picImg = c_readimg( pathIn, exImage )
	
	local w = c_getwidth()
	local h = c_getheight()

	local minV = 0
	local maxV = 1
	if minVal ~= nil then
		minV = minVal
	end
	if maxVal ~= nil then
		maxV = maxVal
	end
	
	local outImage = c_createimg( w, h , "basic" )	
	c_fillalphaf( 0 )				
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				c_selimg( picImg )				
				local r,g,b,a = c_getpixelf( w_, h_ )	
				
				c_selimg( outImage )
				
				
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
					c_setalphaf( w_,h_, a )		
				end			
				
								
				c_setpixelf( w_,h_, r,g,b, 1 )
			end		
	end
	
	c_saveimg( s_getex( pathOut ), pathOut )
	c_delimg( picImg )
	
	c_delimg( outImage )
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
	local picImg = c_readimg( pathIn, exImage )
	
	local w = c_getwidth()
	local h = c_getheight()

	
	local outImage = c_createimg( w, h , "basic" )	
	c_fillalphaf( 0 )				
	
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				c_selimg( picImg )
				
				local r,g,b,a = c_getpixelf( w_, h_ )	
				
				c_selimg( outImage )
				
				local h,s,l = util.rgbtohsl( r,g,b )		
				
				c_setalphaf( w_,h_, a )					
				c_setpixelf( w_,h_, l,l,l, 1 )
			end		
	end
	
	c_saveimg( s_getex( pathOut ), pathOut )
	c_delimg( picImg )
	
	c_delimg( outImage )
	util.msg("***s_grayscale finished")
end

--[[
	Changes color to another
]]--
function s_interpToColor( pathIn,pathOut,rIn,gIn,bIn, factor )

	local exImage = s_getex( pathIn )
	local picImg = c_readimg( pathIn, exImage )
	
	
	local w = c_getwidth()
	local h = c_getheight()

	local outImage = c_createimg( w, h , "basic" )	
	
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				c_selimg( picImg )				
				local r,g,b,a = c_getpixelb( w_, h_ )	
				
				c_selimg( outImage )						
				c_setalphab( w_,h_, a )												
				c_setpixelb( w_,h_, util.cosinterp(r,rIn,factor),util.cosinterp(g,gIn,factor),util.cosinterp(b,bIn,factor), 1 )			
				
			end		
	end
	c_delimg( picImg )
	
	
--	util.msg("***s_interpToColor finished")
	if  s_isstring(pathOut) == true then	
		c_saveimg( exImage, pathOut )
		c_delimg( outImage )
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
		picImg = c_readimg( pathIn,  s_getex( pathIn ) )
	else
		picImg = pathIn
	end
	c_selimg( picImg )
	local w = c_getwidth()
	local h = c_getheight()
	
	local rInterp = util.bytetofloat(rIn)
	local gInterp = util.bytetofloat(gIn)
	local bInterp = util.bytetofloat(bIn)

	local outImage = c_createimg( w, h , "basic" )	
	
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				c_selimg( picImg )				
				local r,g,b,a = c_getpixelf( w_, h_ )	
				
				c_selimg( outImage )						
				c_setalphaf( w_,h_, a )		

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
				c_setpixelf( w_,h_, util.cosinterp(r,rInterp,factor),util.cosinterp(g,gInterp,factor),util.cosinterp(b,bInterp,factor), 1 )			
				
			end		
	end
	
	util.msg("***s_interpToColorChannel finished")
	
	
	if  s_isstring(pathIn) == true	then		
		c_delimg( picImg )			
	end
		
	if pathOut ~= nil then
		c_selimg( outImage )
		c_saveimg( s_getex( pathOut ), pathOut )
		c_delimg( outImage )
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
		picImg = c_readimg( pathIn,  s_getex( pathIn ) )
	else
		picImg = pathIn
	end
	c_selimg( picImg )
	local w = c_getwidth()
	local h = c_getheight()
	
	local picImgInterp = nil
	
	if  s_isstring(pathInterp) == true then			
		picImgInterp = c_readimg( pathInterp, s_getex( pathInterp ) )	
	else
		picImgInterp = pathInterp
	end
	
	c_selimg( picImgInterp )
	
	local wInterp = c_getwidth()
	local hInterp = c_getheight()
	
	

	local outImage = c_createimg( w, h , "basic" )	
	
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				c_selimg( picImg )				
				local r,g,b,a = c_getpixelf( w_, h_ )	
							
				-- take factor values from map
				c_selimg( picImgInterp )				
				local ri,gi,bi = c_getpixelf( w_% wInterp, h_% hInterp )			

				c_selimg( outImage )						
				c_setalphaf( w_,h_, a )	
				
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
				c_setpixelf( w_,h_, util.cosinterp(r,ri,factor),util.cosinterp(g,gi,factor),util.cosinterp(b,bi,factor), 1 )			
				
			end		
	end
	
	util.msg("***s_interpToImageChannel finished")
	
	
	if  s_isstring(pathIn) == true	then		
		c_delimg( picImg )			
	end
	
	c_delimg( picImgInterp )
		
	if pathOut ~= nil then
		c_selimg( outImage )
		c_saveimg( s_getex( pathOut ), pathOut )
		c_delimg( outImage )
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
		picImg = c_readimg( pathIn,  s_getex( pathIn ) )
	else
		picImg = pathIn
	end	
	
	c_selimg( picImg )	
	
	local w = c_getwidth()
	local h = c_getheight()
	
	local picImgInterp = nil
	
	if  s_isstring(pathInterp) == true then			
		picImgInterp = c_readimg( pathInterp, s_getex( pathInterp ) )	
	else
		picImgInterp = pathInterp
	end
	
	c_selimg( picImgInterp )
	
	local wInterp = c_getwidth()
	local hInterp = c_getheight()
	
	local x = math.min((wInterp - 1)/(w - 1),1)
	local y = math.min((hInterp - 1)/(h - 1),1)
	util.msg( x," ",y )
	
	local outImage = c_createimg( w, h , "basic" )	
	if equal == true or equal == nil then
		for w_ = 0, w-1 do	
				for h_ = 0, h-1 do		
				
					-- take color values
					c_selimg( picImg )				
					local rVal,gVal,bVal,aVal = c_getpixelb( w_, h_ )				
					
					-- take factor values from map
					c_selimg( picImgInterp )				
					local r,g,b,a = c_getpixelb( math.floor(w_ * x), math.floor(h_ * y) )
									
					c_selimg( outImage )				
						
					c_setalphab( w_,h_, aVal )		
					
					if rVal == rIn and gVal == gIn and bVal == bIn then
						c_setpixelb( w_,h_, r,g,b, 1 )			
					else
						c_setpixelb( w_,h_, rVal,gVal,bVal, 1 )			
					end
					
				end			
		end
	
	else
		for w_ = 0, w-1 do	
				for h_ = 0, h-1 do		
				
					-- take color values
					c_selimg( picImg )				
					local rVal,gVal,bVal,aVal = c_getpixelb( w_, h_ )				
					
					-- take factor values from map
					c_selimg( picImgInterp )				
					local r,g,b,a = c_getpixelb( math.floor(w_ * x), math.floor(h_ * y) )
									
					c_selimg( outImage )				
						
					c_setalphaf( w_,h_, aVal )		
					
					if rVal ~= rIn and gVal ~= gIn and bVal ~= bIn then
						c_setpixelb( w_,h_, r,g,b, 1 )			
					else
						c_setpixelb( w_,h_, rVal,gVal,bVal, 1 )			
					end
					
				end			
		end
	
	end
		
	if  s_isstring(pathIn) == true	then		
		c_delimg( picImg )			
	end
	if  s_isstring(pathInterp) == true	then		
		c_delimg( picImgInterp )			
	end	
	
	if pathOut ~= nil then
		c_selimg( outImage )
		c_saveimg( s_getex( pathOut ), pathOut )
		c_delimg( outImage )
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
		picImg = c_readimg( pathIn,  s_getex( pathIn ) )
	else
		picImg = pathIn
	end
	c_selimg( picImg )
	local w = c_getwidth()
	local h = c_getheight()
	
	local rInterp = util.bytetofloat(rIn)
	local gInterp = util.bytetofloat(gIn)
	local bInterp = util.bytetofloat(bIn)
	
	local rInterp2 = util.bytetofloat(rIn2)
	local gInterp2 = util.bytetofloat(gIn2)
	local bInterp2 = util.bytetofloat(bIn2)

	local outImage = c_createimg( w, h , "basic" )	
	
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				c_selimg( picImg )				
				local r,g,b,a = c_getpixelf( w_, h_ )	
				
				c_selimg( outImage )						
				c_setalphaf( w_,h_, a )		

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
				c_setpixelf( w_,h_, util.cosinterp(rInterp,rInterp2,factor),util.cosinterp(gInterp,gInterp2,factor),util.cosinterp(bInterp,bInterp2,factor), 1 )			
				
			end		
	end
	
	util.msg("***s_interpToColorChannel finished")
	
	
	if  s_isstring(pathIn) == true	then		
		c_delimg( picImg )			
	end
		
	if pathOut ~= nil then
		c_selimg( outImage )
		c_saveimg( s_getex( pathOut ), pathOut )
		c_delimg( outImage )
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
		picImg = c_readimg( pathIn,  s_getex( pathIn ) )
	else
		picImg = pathIn
	end	
	
	c_selimg( picImg )	
	
	local w = c_getwidth()
	local h = c_getheight()
	
	local picImgMap = nil
	
	if  s_isstring(pathInMap) == true then			
		picImgMap = c_readimg( pathInMap, s_getex( pathInMap ) )	
	else
		picImgMap = pathInMap
	end
	
	c_selimg( picImgMap )
	
	local wM = c_getwidth()
	local hM = c_getheight()
	
	local x = (wM - 1)/(w - 1)
	local y = (hM - 1)/(h - 1)
	
	local rInterp = util.bytetofloat(rIn)
	local gInterp = util.bytetofloat(gIn)
	local bInterp = util.bytetofloat(bIn)

	local outImage = c_createimg( w, h , "basic" )	
	
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				-- take color values
				c_selimg( picImg )				
				local rVal,gVal,bVal,aVal = c_getpixelf( w_, h_ )				
				
				-- take factor values from map
				c_selimg( picImgMap )				
				local r,g,b,a = c_getpixelf( math.floor(w_ * x), math.floor(h_ * y) )
				
				c_selimg( outImage )				
					
				c_setalphaf( w_,h_, aVal )		

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
				c_setpixelf( w_,h_, util.cosinterp(r,rInterp,factor),util.cosinterp(g,gInterp,factor),util.cosinterp(b,bInterp,factor), 1 )			
				
			end		
	end
		
	if  s_isstring(pathIn) == true	then		
		c_delimg( picImg )			
	end
	if  s_isstring(pathInMap) == true	then		
		c_delimg( picImgMap )			
	end	
	
	if pathOut ~= nil then
		c_selimg( outImage )
		c_saveimg( s_getex( pathOut ), pathOut )
		c_delimg( outImage )
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
		local posRand = c_randint(1,size)
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
	local idBox = c_readfile(pathIn, code, bInOneLine )
	return idBox
end


--[[
	Deletes pixels from image out of mask	
	pathBrushIn - path to the mask brush or nil for using xSizeIn and ySizeIn
]]--
function s_maskImage( pathBrushIn, pathOutIn,  pathToCutIn, xPosIn, yPosIn, bRandIn )
	
	
	local picBrushImg = nil 	
	if s_isstring( pathBrushIn ) == true then	
		picBrushImg = c_readimg( pathBrushIn, s_getex( pathBrushIn ) )	
	else
		picBrushImg = pathBrushIn
	end
	
	c_selimg( picBrushImg )
	
	local wB = c_getwidth()
	local hB = c_getheight()
			
	local cutImg = 0
	if s_isstring( pathToCutIn ) == true  then	
		cutImg = c_readimg( pathToCutIn, s_getex(pathToCutIn) )	
	else
		cutImg = pathToCutIn
	end
	
	c_selimg( cutImg )
	
	local wC = c_getwidth()
	local hC = c_getheight()
			
	local posX = xPosIn
	local posY = yPosIn
		
	if bRandIn == true then
		posX = c_randint( 0, wC )
		posY = c_randint( 0, hC )
	end
	
	local endW = math.min( wC, posX + wB )			
	local endH = math.min( hC, posY + hB )		
	
	local outImage = c_createimg( wC, hC , "basic" )
	
	c_fillalphaf( 0 )	
		
	for w_ = 0, endW - 1 do		
			for h_ = 0, endH - 1 do										
				c_selimg( picBrushImg )
				
				local r,g,b,a = c_getpixelf( w_, h_ )	
				
				if a > 0.0 then
					c_selimg( cutImg )					
					local rC,gC,bC,aC = c_getpixelf( posX + w_, posY + h_ )							
					
					c_selimg( outImage )	
					c_setpixelf( w_,h_, rC,gC,bC, aC )
					c_setalphaf( w_,h_, aC )																					
				end				
			end					
	end	
	
	if pathOutIn ~= nil then		
		c_selimg( outImage )
		c_saveimg( s_getex( pathOutIn ), pathOutIn )		
		c_delimg( outImage )
	end
	
	if s_isstring( pathBrushIn ) == true then
		c_delimg( picBrushImg )
	end
	
	if s_isstring( pathToCutIn ) == true  then	
		c_delimg( cutImg )	
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
		picBrushImg = c_readimg( pathBrushIn, s_getex(pathBrushIn) )
	else
		picBrushImg = idBrushIn
	end
	
	local wB = c_getwidth()
	local hB = c_getheight()
	
	local cutImg = 0 
	if s_isstring(pathToCutIn) == true  then
		cutImg = c_readimg( pathToCutIn, s_getex(pathToCutIn) )			
	else
		cutImg = idToCutIn
	end
	
	local wC = c_getwidth()
	local hC = c_getheight()
		
	
	local outImage = c_createimg( wB, hB , "basic" )
	c_fillalphaf( 0 )	
	
	local posX = xPosIn
	local posY = yPosIn
		
	if bRandIn == true then
		posX = c_randint(0, wC)
		posY = c_randint(0, hC)
	end
	
	local endW = 	math.min(wC,posX + wB)			
	local endH = 	math.min(hC,posY + hB)	
	
	for w_ = 0, endW-1 do	
			for h_ = 0, endH-1 do		
			
				c_selimg( picBrushImg )				
				local r,g,b,a = c_getpixelf( w_, h_ )				
	
				c_selimg( cutImg )						
				local rC,gC,bC,aC = c_getpixelf( posX, posY )					
				c_selimg( outImage )	
				c_setpixelf( w_,h_, rC,gC,bC, math.min(a,aC) )
				c_setalphaf( w_,h_, math.min(a,aC) )										
							
			end		
	end	
	
	if pathOutIn ~= nil then
		local exImage = s_getex( pathOutIn )
		c_selimg( outImage )
		c_saveimg( exImage, pathOutIn )		
		c_delimg( outImage )
	end
	
	c_delimg( picBrushImg )	
	c_delimg( cutImg )		
	
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

	
	local imgId = c_readimg( imgPathIn, s_getex(imgPathIn) )	
	
	local w = c_getwidth()
	local h = c_getheight()	
			
	local outImage = c_createimg( w, h , "basic" )
	c_fillalphaf( 1 )	
	local bIncr = false
	if bIncrIn ~= nil then
		bIncr = bIncrIn
	end
	
	
	for w_ = 0, w -1 do	
			for h_ = 0, h-1 do		
			
				c_selimg( imgId )				
				local r,g,b,a = c_getpixelf( w_, h_ )				
								
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
				c_selimg( outImage )					
				
				c_setpixelf( w_,h_, r,g,b, a )													
				c_setalphaf( w_,h_, a )	
			end		
	end	
	
	if imgRezPathIn ~= nil then
		c_selimg( outImage )
		c_saveimg( s_getex( imgRezPathIn ), imgRezPathIn )		
		c_delimg( outImage )
	end
	
	c_delimg( imgId )		
	
	if imgRezPathIn == nil then
		return outImage
	end
	util.msg("s_multiplyChannel finished")
	
end


--[[
	works like s_multiplyChannel instead of factor uses light from imgPathIn2
]]--
function s_multiplyChannelImg(imgPath1In, imgPathIn2, imgRezPathIn, channelIn, factorIn,  bIncrIn )

	
	local imgId1 = c_readimg( imgPath1In, s_getex(imgPath1In) )	
	
	
	local w1 = c_getwidth()
	local h1 = c_getheight()	
	
	local imgId2 = c_readimg( imgPathIn2, s_getex(imgPathIn2) )
	
	local w2 = c_getwidth()
	local h2 = c_getheight()	
		
	local wEnd = w1
	local hEnd = h1
	
	local outImage = c_createimg( wEnd, hEnd , "basic" )
	c_fillalphaf( 1 )	
	
	local bIncr = false
	if bIncrIn ~= nil then
		bIncr = bIncrIn
	end
	
	local factorX = (w2-1)/(w1-1)
	local factorY = (h2-1)/(h1-1)
	
	
	local factor = factorIn
					
	for w_ = 0, wEnd -1 do	
			for h_ = 0, hEnd-1 do		
			
				c_selimg( imgId2 )				
				local posX = w_ * factorX
				local posY = h_ * factorY
				
				--util.msgf(posX, " ",posY)
				
				local r2,g2,b2,a2 = c_getpixelf( math.floor(posX) , math.floor(posY) )				
				local h2,s2,l2 = util.rgbtohsl( r2,g2,b2 )	
				
				
				c_selimg( imgId1 )				
				local r1,g1,b1,a1 = c_getpixelf( w_, h_ )				
								
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
				
				c_selimg( outImage )	
				
				c_setpixelf( w_,h_, r1,g1,b1, 1 )
				c_setalphaf( w_,h_, a1 )				
							
			end		
	end	
	
	if imgRezPathIn ~= nil then
		c_selimg( outImage )
		c_saveimg( s_getex( imgRezPathIn ), imgRezPathIn )		
		c_delimg( outImage )
	end
	
	c_delimg( imgId1 )	
	c_delimg( imgId2 )		
	
	if imgRezPathIn == nil then
		return outImage
	end
	util.msg("s_multiplyChannelImg finished")
end



function s_resizeImage(imgSrcIn, imgPathRezIn, xSizeIn, ySizeIn)

	c_assert( xSizeIn> 0  and ySizeIn > 0 , "wrong result image size" )
	local imgId = 0
	if s_isstring(imgSrcIn) == true  then
		imgId = c_readimg( imgSrcIn, s_getex(imgSrcIn) )		
	else
		imgId = imgSrcIn
	end
	--[[
	local w = c_getwidth()
	local h = c_getheight()
	
		
	local outImage = c_createimg( xSizeIn, ySizeIn, "basic" )		
	local factorX = ( w - 1 )/( xSizeIn - 1 )
	local factorY = ( h - 1 )/( ySizeIn - 1 ) 
	
	for w_ = 0, xSizeIn - 1 do	
			for h_ = 0, ySizeIn - 1 do		
			
				c_selimg( imgId )	
				
				local r,g,b,a = c_getpixelb( math.floor(w_ * factorX), math.floor(h_ * factorY) )												
				c_selimg( outImage )
				
				c_setpixelb( w_,h_, r,g,b, 1 )
				c_setalphab( w_,h_, a )				
							
			end		
	end	
	
	
	
	if imgPathRezIn ~= nil then
		c_selimg( outImage )
		c_saveimg( s_getex( imgPathRezIn ), imgPathRezIn )
		c_delimg(outImage)
	else
		return outImage
	end
	]]--
	local outImage = c_scaleimg(imgId, xSizeIn, ySizeIn , false)
	
	if imgPathRezIn ~= nil then
		c_selimg( outImage )
		c_saveimg( s_getex( imgPathRezIn ), imgPathRezIn )
		c_delimg(outImage)
	else
		return outImage
	end
	
	
	if s_isstring(imgSrcIn) == true  then
		c_delimg(imgId)
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
	
	local picImg = c_readimg( pathIn, exImage )	
	
	local w = c_getwidth()

	local h = c_getheight()

	local outImage = c_createimg( w, h , "basic" )	
	
	local alphaFore = 1	
	local alphaBack = 0
	
	local arrSize = s_arrlength(colorsIn)
	util.msg( arrSize )
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				c_selimg( picImg )										
				
				local r,g,b,a = c_getpixelf( w_, h_ ) 
				
					
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
					f = c_randfloat()				
				end
				
				if invertIn == true then
					f = 1 - f
				end
								
				c_selimg( outImage )			

				local pos, color = s_getDiscreteColorByFactor( colorsIn, arrSize, f )				
				local bR,bG,bB = util.unpackcolor( color )
				
				local rMed = util.bytetofloat( bR )
				local gMed = util.bytetofloat( bG )
				local bMed = util.bytetofloat( bB )
	
				local vr = util.cosinterp( r,rMed, factorIn )				
				local vg = util.cosinterp( g,gMed, factorIn )
				local vb = util.cosinterp( b,bMed, factorIn )
				
				c_setpixelf( w_, h_, vr,vg,vb, 1 )					
									
				c_setalphaf(   w_, h_, a )								
			end		
	end
	
	c_saveimg( exImageOut, pathOut )
	c_delimg( picImg )
	
	c_delimg( outImage )
	
end


--[[
	Builds array and interpolates by alpha value
]]--
function s_interpToArrColorsAlpha( pathIn, pathOut, colorsIn,  factorIn, invertIn)	
	
	local exImage = s_getex(pathIn)
	
	local exImageOut = s_getex(pathOut)	
	
	local picImg = c_readimg( pathIn, exImage )	
	
	local w = c_getwidth()

	local h = c_getheight()

	local outImage = c_createimg( w, h , "basic" )	
	
	local alphaFore = 1	
	local alphaBack = 0
	
	
	local arrSize = s_arrlength(colorsIn)
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				c_selimg( picImg )										
				
				local r,g,b,a = c_getpixelf( w_, h_ ) 
				f = a
				
				
				
				if invertIn == true then
					f = 1 - f
				end
								
				c_selimg( outImage )			

				local pos, color = s_getDiscreteColorByFactor( colorsIn,arrSize, f )				
				local bR,bG,bB = util.unpackcolor( color )
				
				local rMed = util.bytetofloat( bR )
				local gMed = util.bytetofloat( bG )
				local bMed = util.bytetofloat( bB )
	
				local vr = util.cosinterp( r,rMed, factorIn )				
				local vg = util.cosinterp( g,gMed, factorIn )
				local vb = util.cosinterp( b,bMed, factorIn )
				
				c_setpixelf( w_, h_, vr,vg,vb, 1 )					
									
				c_setalphaf(   w_, h_, a )								
			end		
	end
	
	c_saveimg( exImageOut, pathOut )
	c_delimg( picImg )
	
	c_delimg( outImage )
	c_printmsg("***s_masklight finished")
end


function s_clearImage( imgPathIn, imgRezPathIn, pixAroundIn, rIn, gIn, bIn, factorMaxIn )

	
	local imgId = c_readimg( imgPathIn, s_getex(imgPathIn) )	
	
	local w = c_getwidth()
	local h = c_getheight()	
			
	local outImage = c_createimg( w, h , "basic" )
	c_fillalphaf( 1 )	
		
		
	local pixAround = pixAroundIn * 2 + 1
	local factorMax = ( pixAround * pixAround ) - 1
				
	for w_ = 0, w -1 do	
			for h_ = 0, h-1 do		
			
				c_selimg( imgId )	
				
				local r,g,b,a = c_getpixelb( w_, h_ )									
				
				if r ~= rIn and g ~= gIn and b ~= bIn then 
				
					local countFactor = 0
					local wBegin =  math.max( 0, w_ - pixAround )
					local hBegin =  math.max( 0, h_ - pixAround )
					
					local wEnd =  math.min( w - 1, w_ + pixAround )
					local hEnd =  math.min( h - 1, h_ + pixAround )
					
					for wIn = wBegin, wEnd do	
						for hIn = hBegin, hEnd do							
							local rF,gF,bF = c_getpixelb( wIn, hIn )
							
							if rF == rIn and gF == gIn and bF == bIn then 
								countFactor = countFactor + 1
							end								
						end
					end
					
					if countFactor / factorMax >= factorMaxIn then
						r,g,b = rIn, gIn, bIn
					end
					
				end
				
				c_selimg( outImage )					
				
				c_setpixelb( w_,h_, r,g,b, 1 )													
				c_setalphab( w_,h_, a )			
			end		
	end	
	
	if imgRezPathIn ~= nil then
		c_selimg( outImage )
		c_saveimg( s_getex( imgRezPathIn ), imgRezPathIn )		
		c_delimg( outImage )
	end
	
	c_delimg( imgId )		
	
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
	
		local palleteImage = c_createimg( arrayOutSize, 1 , "basic" )			
		c_fillalphaf( 1 )	
		 for i = 1, arrayOutSize do
			local bR,bG,bB = util.unpackcolor( arrColor[i] )
			
			c_setpixelb( i - 1, 0, bR,bG,bB, 1 )	
		 end
		 
		 local pathPallete = s_delex( PathOut )
		 pathPallete = string.format("%s_pallete.png", pathPallete)
		 c_saveimg( "png", pathPallete )
		 c_delimg( palleteImage )
		 
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

	local PathOut = string.format("%s/%s.%s", pic_dir_filtered, c_uniqid(), "png" )
	local arrayOutSize = sizePallete
	
	local arrColor = nil 
	if s_isstring(pallete) == true then
		arrColor = s_getpallete( pallete )
	else 
		arrColor = s_genColorsRandTable(0,255, pallete ) 
	end
	local arrAllColors = s_buildColorArray( arrColor, arrayOutSize )

	local outImage = c_createimg( 1, arrayOutSize , "basic" )	
	
	c_fillalphaf( 1 )
	for h_ = 0, arrayOutSize - 1  do	
		local bR,bG,bB = util.unpackcolor( arrAllColors[h_ + 1] )
		--util.msg( bR, " " , bG, " ", bB )
		c_setpixelb( 0, h_, bR,bG,bB, 1 )	
	end
	if pathOut ~= nil then
		c_saveimg( s_getex( PathOut ), PathOut )		
		c_delimg( outImage )
	else		
		return outImage
	end

end


function s_poster( pathIn, pathOut, palletePath , outline , palleteSize)

	local pallete = nil
	
	local imgIdCalc = c_readimg( pathIn, s_getex(pathIn) )	

	if palletePath == nil then
		util.msg( "pallete created" )
		-- good single objects pallete = c_getcolors( 30, 0.4 )	
		-- good foto bad contrast pallete = c_getcolors( 40, 0.2 )	
		-- good discrete colors   bear_hysteria pallete = c_getcolors( 10, 0.06 )	
		--pallete = c_getcolors( 40, 0.2 )	
		pallete = c_getcolors( palleteSize, 0.2 )	
	else
		pallete = c_readimg( palletePath, s_getex(palletePath) )	
	end
	
	c_selimg( imgIdCalc )
	c_poster( pallete )
	
	c_priorcolor( 2, 0, 3  )
	
	c_clearimage( 6, 0 )
	
	c_selimg( pallete )
	
	-- outline
	if outline == true then
		local r,g,b = c_getpixelb(0,0)	
		c_selimg( imgIdCalc )
		c_priorcolor( 6, 0, 1, 0,115,0   )
	end
	c_selimg( imgIdCalc )
	

	c_saveimg( "png",pathOut )

	--c_saveimg( "png",PathOut )
	if palletePath == nil then
		c_delimg(pallete)
	end
	--c_delimg(imgId)
	c_delimg(imgIdCalc)
end


function s_priorcolor ( PathIn, PathOut, around, factor, passes )

	local picImg = c_readimg( PathIn,  s_getex( PathIn ) )
	
	c_priorcolor( around, factor, passes  )
	
	--c_priorcolor( 2, 0, 3  )
	--c_priorcolor( 6, 0, 1, 0,0,170  )
	

	--local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, c_uniqid(), "png")
	c_saveimg( s_getex( PathOut ), PathOut )		
	c_delimg( picImg )	
end


function s_priorcolor2 ( PathIn, PathOut, around, factor, passes )

	local picImg = c_readimg( PathIn,  s_getex( PathIn ) )
	
	c_priorcolor2( around, factor, passes  )
	
	--c_priorcolor( 2, 0, 3  )
	--c_priorcolor( 6, 0, 1, 0,0,170  )
	

	--local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, c_uniqid(), "png")
	c_saveimg( s_getex( PathOut ), PathOut )		
	c_delimg( picImg )	
end




--[[
	returns filled  box  with pixel coordinates alpha > 0.0	
]]--
function s_boxFromAlpha( imgPath )

	local picImg = nil
	
	if s_isstring(imgPath) == true  then
		picImg = c_readimg( imgPath ,s_getex(imgPath) )
	else
		picImg = imgPath
	end	
	
	c_selimg(picImg)
	
	local w = c_getwidth()
	local h = c_getheight()
	
	--local outImage = c_createimg( w, h , "basic" )	
	local box = c_createbox("vec3f_vector")
	
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				--c_selimg( picImg )				
				local r,g,b,a = c_getpixelb( w_, h_ )	
				
				if a > 0 then
					c_pushbackbox3f( box, math.floor(w_), math.floor(h_), 0.0 )					
				end				
			end		
	end
	
	if s_isstring(imgPath) == true then
		c_delimg( picImg )
	end
	
	
	util.msg("***s_boxFromAlpha finished")
	
	return  box	
end

function s_numsorted( PathIn, ext  )

	local filesStr = {}	
	
	local files  = util.countfiles( PathIn, ext )

	if files ~= nil and files > 2 then

		for i = 1, files - 1 do 		
		
				local fileInFullPath = util.getfile( PathIn, i ,ext , false )				
				local num =  tonumber(string.match( fileInFullPath ,  "%d+" ))
				
				filesStr[num] = fileInFullPath				
				
		end					
		
	end	
	for  i, val in ipairs(filesStr) do
		util.msg(val)
	end
	
	return filesStr
end
