require "Scripts/lib/l_util"
require "Scripts/lib/patterns"
require "paths"

local begTime = os.clock()
s_printTimePoint(begTime, 0)


	
	
function makeRecord( secondsRec , rate )

	local frameTime24 = 41.666
	local frameTime32 = 31.25
	
	local frameTime = nil
	
	if rate == 24 then
		frameTime = frameTime24
	elseif rate == 32 then
		frameTime = frameTime32
	else
		c_assert( false , "Wrong frame rate" )	
	end

	local 	timeRecord = secondsRec * 1000
	
	util.msg("rate ", frameTime)
	local frames = math.floor(timeRecord / frameTime )
	
	util.msg("frames ", frames)
	
	
	local PathIn = "C:/foto/rez/3132729095_2017122615937608_1110.png"
	local PathInMap = "C:/foto/indicator2.png"
	local ext = "jpg"
	
	--local picImg = c_readimg( PathIn,  s_getex( PathIn ) )	
	--local mapImg = c_readimg( PathInMap,  s_getex( PathInMap ) )
	
	local rVal, gVal,bVal = util.unpackcolor( 0xff0000 )
	local channel = 4
	
	for f = 0, frames - 1  do	
	
		local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, f, ext )				
		
		--s_interpToColorChannelMap( PathInMap, PathIn, PathOut, rVal, gVal,bVal, channel, f/( frames - 1 ) )		
		local script = string.format( " require \"Scripts/tests/test_util\" s_interpToColorChannelMap( \"%s\", \"%s\", \"%s\", %d,%d,%d, %d, %f ) ", PathInMap, PathIn, PathOut, rVal, gVal,bVal,channel, f/( frames - 1 ))		
		c_ed( script )
		--s_interpToColorChannel( picImg, PathOut, rVal,gVal,bVal, channel, f/( frames - 1 ) )	
		
	end
	
	
	--c_delimg( picImg )
	
	--c_delimg( mapImg )
end

makeRecord( 60, 32 )

s_printTimePoint(begTime, 1)