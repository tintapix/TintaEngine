require "Scripts/lib/l_util"


--[[
local filePath  = "D:/Work/SUMK_OSHOT_vs2015/bin/Debug/2.txt"
--local filePathOut  = "D:/ReloadedItemsMap.out"
local filePathOut  = "D:/Work/SUMK_OSHOT_vs2015/bin/Debug/2_out.txt"

local textVec = s_getStringArrayId( filePath, "utf8", false )

local out = c_createbox(c_createbox"str_vector")
local size = c_boxelements(textVec)

local nameCount = 263
local nameCountOld = 264

for count = 1 , size do
	local text = c_getvalboxs( textVec, count - 1 )	
	
	local textOld = string.format("[Name%i]", nameCountOld )
	
	if text == textOld then	
		text = string.format("[Name%i]", nameCount )
		
		nameCount  = nameCount + 1		
		nameCountOld = nameCountOld + 1
		
	end
	c_pushbackboxs( out, text )	
end

c_writetofile( filePathOut, out )

c_delbox(textVec)

util.msg("finished")

]]--

--[[
local filePath  = "D:/Work/SUMK_OSHOT_vs2015/SUMKApp/data/db/DeviceError.txt"
--local filePathOut  = "D:/ReloadedItemsMap.out"
local filePathOut  = "D:/Work/SUMK_OSHOT_vs2015/SUMKApp/data/db/DeviceError_out.txt"

local textVec = s_getStringArrayId( filePath, "utf8", false )

local out = c_createbox(c_createbox"str_vector")
local size = c_boxelements(textVec)

local nameCount_ = 277
local nameCount  = 81

for count = 1 , size do

	local text = c_getvalboxs( textVec, count - 1 )		
	
	local secTest = string.format("[Name%i]", nameCount_ )
	
	if text == secTest then	
		text = string.format("[Name%i]", nameCount )
		
		nameCount  = nameCount + 1
		nameCount_	= nameCount_ + 1
	end
	
	c_pushbackboxs( out, text )	
	
end

c_writetofile( filePathOut,  out )

c_delbox(textVec)

]]--



--[[
local filePath  = "D:/Work/SUMK_OSHOT_vs2015/bin/Debug/err_out.txt"
--local filePathOut  = "D:/ReloadedItemsMap.out"
local filePathOut  = "D:/Work/SUMK_OSHOT_vs2015/bin/Debug/2_out.txt"

local textVec = s_getStringArrayId( filePath, "utf8", false )

local out = c_createbox(c_createbox"str_vector")
local size = c_boxelements(textVec)

local nameCount = 81

for count = 1 , size do
	local text = c_getvalboxs( textVec, count - 1 )	
	
		
	
	local number = string.match(text,   "%d+" )
	if number ~= nil then
		local NameX = string.format("[Name%i]", nameCount )	
		c_pushbackboxs( out, NameX )		
		
		
		
		local ID = string.format("ID = %i", number )	
		
		c_pushbackboxs( out, ID )		
		
		local description  = string.sub(text, string.len(number + 1))
		description = string.format("TextSimantics = %s", description )
		c_pushbackboxs( out, description )
		nameCount = nameCount  + 1
	end
	
end


c_writetofile( filePathOut,  out )

]]--


--[[
local filePath  = "D:/Work/SUMK_OSHOT_vs2015/bin/Debug/db/DeviceError.txt"
--local filePathOut  = "D:/ReloadedItemsMap.out"
local filePathOut  = "D:/Work/SUMK_OSHOT_vs2015/bin/Debug/err_out.txt"

local textVec = s_getStringArrayId( filePath, "utf8", false )
local out = c_createbox(c_createbox"str_vector")

local size = c_boxelements(textVec)
for count = 1 , size do
	local text = c_getvalboxs( textVec, count - 1 )	
	local number = string.match(text,   "F%d+" )
	if 	number ~= nil then
		c_pushbackboxs( out, text )
	end
end
]]--

--[[
local filePath  = "D:/Work/SUMK_OSHOT_vs2015/bin/Debug/db/DeviceError.txt"
--local filePathOut  = "D:/ReloadedItemsMap.out"
local filePathOut  = "D:/Work/SUMK_OSHOT_vs2015/bin/Debug/err_out.txt"

local textVec = s_getStringArrayId( filePath, "utf8", false )
local out = c_createbox(c_createbox"str_vector")

local size = c_boxelements(textVec)
for count = 1 , size do
	local text = c_getvalboxs( textVec, count - 1 )	
	local descrSina = string.match(text,   "^TextSinamics" )	
	local descrSew = string.match(text,   "^TextSew" )
	
	if 	descrSina ~= nil or descrSew ~= nil then		
		--util.msg(text)		
		text = string.format("%s%s", text, "\"" )
	end
	
	c_pushbackboxs( out, text )
end

c_writetofile( filePathOut,  out )
]]--




--[[
local filePath  = "D:/Work/SUMK_OSHOT_vs2015/bin/Debug/sinamics.txt"
--local filePathOut  = "D:/ReloadedItemsMap.out"
local filePathOut  = "D:/Work/SUMK_OSHOT_vs2015/bin/Debug/err_out.txt"

local textVec = s_getStringArrayId( filePath, "utf8", false )
local out = c_createbox(c_createbox"str_vector")

local size = c_boxelements(textVec)
for count = 1 , size do
	local text = c_getvalboxs( textVec, count - 1 )	
	local number = string.match(text,   "^F%d+" )
	if 	number ~= nil then	
		--util.msg(text)
		c_pushbackboxs( out, text )
	end
end
c_writetofile( filePathOut,  out )


c_delbox(textVec)
c_delbox(out)
]]--



local filePath  = "D:/mya/TintaSpringBuildWin32_GPU/in.txt"

local filePathOut  = "D:/mya/TintaSpringBuildWin32_GPU/out.txt"

local textVec = s_getStringArrayId( filePath, "utf8", false )
local inFile = c_createbox("str_vector")
local outFile = c_createbox("str_vector")

--[[
local size = c_boxelements(textVec)
for count = 1 , size do
	local text = c_getvalboxs( textVec, count - 1 )	
	local number = string.match(text,   "address=" )
	if 	number ~= nil then	
		--util.msg(text)
		c_pushbackboxs( inFile, text )
	else
		c_pushbackboxs( outFile, text )
	end
end
]]--
local q = c_boxelements(textVec)

startId = 1515

for k = 0, q - 1 do	
	val = c_getvalboxs( textVec, k )
	strFindName =  string.match( val, "icsId=\"%d+" ) 		
	if strFindName == nil then
		--util.msg("nill")
		c_pushbackboxs( outFile, val )
	else
		strFind = string.match(strFindName,  "%d+" )		
		strNew = string.format( "icsId=\"%d", startId + tonumber( strFind ) )	
		local final = string.gsub( val, strFindName, strNew )   	
		c_pushbackboxs( outFile, final )
	end		
	
end

c_writetofile( filePathOut, outFile )

c_delbox( textVec )
c_delbox( inFile )
c_delbox( outFile )

util.msg("\nfinished")