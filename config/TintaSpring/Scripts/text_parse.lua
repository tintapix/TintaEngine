require "Scripts/lib/l_util"
require "Scripts/lib/patterns"

require "paths"
util.timestamp(false)
--[[
local path = "D:/work/indicator"

--if util.fopened(path) == false then

	--file = io.open( path , "r" )

	--io.input( file )
	--utril.msg("lines")
	for line in io.lines(path) do 
    --t[#t+ 1] = line
		lines = {}
		for token in string.gmatch(line, "[^%s]+") do
			--util.msg(token)
			table.insert(lines,token)			
		end
		util.msg(string.format("FillIndicator(%s, BACKCOLOR_EMERGENCY, nsSpecialDataBaseNames::cstr%s );", lines[1], lines[2]))
		
	end
	
	
	--io.close(file)
	--io.flush() 

--end
--util.sleep(100000)
--c_start("write_data()", 500)
]]--


--[[
local path = "D:/work/indicator2"

--if util.fopened(path) == false then

	--file = io.open( path , "r" )

	--io.input( file )
	--utril.msg("lines")
	for line in io.lines(path) do 
    --t[#t+ 1] = line
		lines = {}
		for token in string.gmatch(line, "%[(.*)%]") do
			util.msg("<!-- ",token,"-->")			
		end
		
		
		lines = {}
		for token in string.gmatch(line, "ID = %d+") do
			--util.msg(token)
			table.insert(lines,token)			
		end
		--util.msg(string.format("FillIndicator(%s, BACKCOLOR_EMERGENCY, nsSpecialDataBaseNames::cstr%s );", lines[1], lines[2]))
		if lines[1]	~= nil then	
			
			for token2 in string.gmatch(lines[1], "%d+") do
				--util.msg(token2)
				util.msg(string.format("<Variable category=\"Signals\" id=\"%s\"> false </Variable>", token2 ) )
			end			
		end
		
		
	end
]]--


local path = "D:/work/hmel.txt"

--if util.fopened(path) == false then

	--file = io.open( path , "r" )

	--io.input( file )
	--utril.msg("lines")
	array = {}
	for line in io.lines(path) do 
    
		dublicated = false
		for i,k in pairs(array) do 
			--util.msg(k)
			if k == line then
					util.msg( "dublicate: ", line  )	
					dublicated = true
					break
			end
		end
		
		if dublicated == false then
			table.insert(array,line)
		end
		
	end
	
	for i,k in pairs(array) do 
			--util.msg(k)		
		util.msg( k  )			
	end
	