require "Scripts/lib/l_util"
require "config"


local filePath  = "D:/test.txt"
local filePathOut  = "D:/ReloadedItemsMap.out"

--local token = "\\[\\w{0,50}\\]"
local token = "<div>(.+?)</div>"

local text = s_getStringArrayId( filePath, "utf8", true )

--local valLine = c_getvalboxs( text, 0 )
--local match = regexmatch(token, valLine,  0)

local iRez = c_regextokenex( token, text, 0, "1", 0 )
if iRez ~=  nil then
	local r = c_boxelements(iRez)
	util.msg("matches:", r)
	for i = 0, r - 1 do				
		util.msg( c_getvalboxs( iRez, i ) )		
	end	
	
	--c_writetofile( filePathOut, "utf8", iRez )
	c_delbox( iRez )
end



c_delbox( text )
