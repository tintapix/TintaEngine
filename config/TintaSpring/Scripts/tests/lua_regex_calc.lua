require "Scripts/lib/l_util"


--[[
local filePath  = "D:/Work/SUMK_OSHOT_vs2015/bin/Debug/2.txt"
--local filePathOut  = "D:/ReloadedItemsMap.out"
local filePathOut  = "D:/Work/SUMK_OSHOT_vs2015/bin/Debug/2_out.txt"

local textVec = s_getStringArrayId( filePath, "utf8", false )

local out = box.create(box.create"str_vector")
local size = box.elements(textVec)

local nameCount = 263
local nameCountOld = 264

for count = 1 , size do
	local text = box.getvals( textVec, count - 1 )	
	
	local textOld = string.format("[Name%i]", nameCountOld )
	
	if text == textOld then	
		text = string.format("[Name%i]", nameCount )
		
		nameCount  = nameCount + 1		
		nameCountOld = nameCountOld + 1
		
	end
	box.pushbacks( out, text )	
end

c_writetofile( filePathOut, out )

image.erase(textVec)

util.msg("finished")

]]--

--[[
local filePath  = "D:/Work/SUMK_OSHOT_vs2015/SUMKApp/data/db/DeviceError.txt"
--local filePathOut  = "D:/ReloadedItemsMap.out"
local filePathOut  = "D:/Work/SUMK_OSHOT_vs2015/SUMKApp/data/db/DeviceError_out.txt"

local textVec = s_getStringArrayId( filePath, "utf8", false )

local out = box.create(box.create"str_vector")
local size = box.elements(textVec)

local nameCount_ = 277
local nameCount  = 81

for count = 1 , size do

	local text = box.getvals( textVec, count - 1 )		
	
	local secTest = string.format("[Name%i]", nameCount_ )
	
	if text == secTest then	
		text = string.format("[Name%i]", nameCount )
		
		nameCount  = nameCount + 1
		nameCount_	= nameCount_ + 1
	end
	
	box.pushbacks( out, text )	
	
end

c_writetofile( filePathOut,  out )

image.erase(textVec)

]]--



--[[
local filePath  = "D:/Work/SUMK_OSHOT_vs2015/bin/Debug/err_out.txt"
--local filePathOut  = "D:/ReloadedItemsMap.out"
local filePathOut  = "D:/Work/SUMK_OSHOT_vs2015/bin/Debug/2_out.txt"

local textVec = s_getStringArrayId( filePath, "utf8", false )

local out = box.create(box.create"str_vector")
local size = box.elements(textVec)

local nameCount = 81

for count = 1 , size do
	local text = box.getvals( textVec, count - 1 )	
	
		
	
	local number = string.match(text,   "%d+" )
	if number ~= nil then
		local NameX = string.format("[Name%i]", nameCount )	
		box.pushbacks( out, NameX )		
		
		
		
		local ID = string.format("ID = %i", number )	
		
		box.pushbacks( out, ID )		
		
		local description  = string.sub(text, string.len(number + 1))
		description = string.format("TextSimantics = %s", description )
		box.pushbacks( out, description )
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
local out = box.create(box.create"str_vector")

local size = box.elements(textVec)
for count = 1 , size do
	local text = box.getvals( textVec, count - 1 )	
	local number = string.match(text,   "F%d+" )
	if 	number ~= nil then
		box.pushbacks( out, text )
	end
end
]]--

--[[
local filePath  = "D:/Work/SUMK_OSHOT_vs2015/bin/Debug/db/DeviceError.txt"
--local filePathOut  = "D:/ReloadedItemsMap.out"
local filePathOut  = "D:/Work/SUMK_OSHOT_vs2015/bin/Debug/err_out.txt"

local textVec = s_getStringArrayId( filePath, "utf8", false )
local out = box.create(box.create"str_vector")

local size = box.elements(textVec)
for count = 1 , size do
	local text = box.getvals( textVec, count - 1 )	
	local descrSina = string.match(text,   "^TextSinamics" )	
	local descrSew = string.match(text,   "^TextSew" )
	
	if 	descrSina ~= nil or descrSew ~= nil then		
		--util.msg(text)		
		text = string.format("%s%s", text, "\"" )
	end
	
	box.pushbacks( out, text )
end

c_writetofile( filePathOut,  out )
]]--




--[[
local filePath  = "D:/Work/SUMK_OSHOT_vs2015/bin/Debug/sinamics.txt"
--local filePathOut  = "D:/ReloadedItemsMap.out"
local filePathOut  = "D:/Work/SUMK_OSHOT_vs2015/bin/Debug/err_out.txt"

local textVec = s_getStringArrayId( filePath, "utf8", false )
local out = box.create(box.create"str_vector")

local size = box.elements(textVec)
for count = 1 , size do
	local text = box.getvals( textVec, count - 1 )	
	local number = string.match(text,   "^F%d+" )
	if 	number ~= nil then	
		--util.msg(text)
		box.pushbacks( out, text )
	end
end
c_writetofile( filePathOut,  out )


image.erase(textVec)
image.erase(out)
]]--



local filePath  = "D:/mya/TintaSpringBuildWin32_GPU/in.txt"

local filePathOut  = "D:/mya/TintaSpringBuildWin32_GPU/out.txt"

local textVec = s_getStringArrayId( filePath, "utf8", false )
local inFile = box.create("str_vector")
local outFile = box.create("str_vector")

--[[
local size = box.elements(textVec)
for count = 1 , size do
	local text = box.getvals( textVec, count - 1 )	
	local number = string.match(text,   "address=" )
	if 	number ~= nil then	
		--util.msg(text)
		box.pushbacks( inFile, text )
	else
		box.pushbacks( outFile, text )
	end
end
]]--
local q = box.elements(textVec)

startId = 1515

for k = 0, q - 1 do	
	val = box.getvals( textVec, k )
	strFindName =  string.match( val, "icsId=\"%d+" ) 		
	if strFindName == nil then
		--util.msg("nill")
		box.pushbacks( outFile, val )
	else
		strFind = string.match(strFindName,  "%d+" )		
		strNew = string.format( "icsId=\"%d", startId + tonumber( strFind ) )	
		local final = string.gsub( val, strFindName, strNew )   	
		box.pushbacks( outFile, final )
	end		
	
end

c_writetofile( filePathOut, outFile )

image.erase( textVec )
image.erase( inFile )
image.erase( outFile )

util.msg("\nfinished")