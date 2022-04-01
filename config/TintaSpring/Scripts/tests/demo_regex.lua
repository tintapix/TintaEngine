require "Scripts/lib/l_util"
require "config"


local filePath  = "D:/test.txt"
local filePathOut  = "D:/ReloadedItemsMap.out"

--local token = "\\[\\w{0,50}\\]"
local token = "<div>(.+?)</div>"

local text = s_getStringArrayId( filePath, "utf8", true )

--local valLine = box.getvals( text, 0 )
--local match = regexmatch(token, valLine,  0)

local iRez = main.regextokenex( token, text, 0, "1", 0 )
if iRez ~=  nil then
	local r = box.elements(iRez)
	util.msg("matches:", r)
	for i = 0, r - 1 do				
		util.msg( box.getvals( iRez, i ) )		
	end	
	
	--c_writetofile( filePathOut, "utf8", iRez )
	image.erase( iRez )
end



image.erase( text )
