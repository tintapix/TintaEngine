require "Scripts/lib/l_util"
require "Scripts/lib/patterns"


require "paths"
util.timestamp(false)




function createDoc( PathIn, PathOut)


	local fileUtilOut = io.open( PathOut , "w" )

	fileUtilOut:write("<!DOCTYPE html>")
	fileUtilOut:write("\n")
	fileUtilOut:write("<body bgcolor=\"#FFFFFF\">")
	fileUtilOut:write("\n")

	local nameTag = nil
	for line in io.lines(PathIn) do 


			nameTag = string.find(line, "@name")	
			descripTag =  string.find(line, "@description")
			paramTag =  string.find(line, "@param")
			returnTag =  string.find(line, "@return")
			
				
			
			if nameTag ~= nil then
				fileUtilOut:write("&ensp;")					
				
				fileUtilOut:write("<p>")
				fileUtilOut:write("<b>")
				fileUtilOut:write("\n")
				fileUtilOut:write( "Name: " )				
				fileUtilOut:write("\n")	
				fileUtilOut:write("</b>")							
				
				
				fileUtilOut:write("\n")
				fileUtilOut:write( string.sub( line, nameTag + string.len("@name"), -1 ) )				
				fileUtilOut:write("\n")	
				fileUtilOut:write("</p>")			
				fileUtilOut:write("\n")			
			end
			
			if descripTag ~= nil then
				fileUtilOut:write("<p>")
				fileUtilOut:write("<b>")
				fileUtilOut:write("\n")
				fileUtilOut:write("Description: " )				
				fileUtilOut:write("\n")	
				fileUtilOut:write("</b>")							
				
				
				fileUtilOut:write("\n")
				fileUtilOut:write( string.sub( line, descripTag + string.len("@description"), -1 ) )				
				fileUtilOut:write("\n")	
				fileUtilOut:write("</p>")			
				fileUtilOut:write("\n")
			end
			
			if paramTag ~= nil then
				fileUtilOut:write("<p>")
				fileUtilOut:write("<b>")
				fileUtilOut:write("\n")
				fileUtilOut:write( "Parameter: " )				
				fileUtilOut:write("\n")	
				fileUtilOut:write("</b>")							
				
				
				fileUtilOut:write("\n")
				fileUtilOut:write( string.sub( line, paramTag + string.len("@param"), -1 ) )				
				fileUtilOut:write("\n")	
				fileUtilOut:write("</p>")			
				fileUtilOut:write("\n")	
			end
						
			if returnTag ~= nil then	
				fileUtilOut:write("<p>")
				fileUtilOut:write("<b>")
				fileUtilOut:write("\n")
				fileUtilOut:write( "Return: " )				
				fileUtilOut:write("\n")	
				fileUtilOut:write("</b>")							
				
				
				fileUtilOut:write("\n")
				fileUtilOut:write( string.sub( line, returnTag + string.len("@return"), -1 ) )				
				fileUtilOut:write("\n")	
				fileUtilOut:write("</p>")			
				fileUtilOut:write("\n")	
			end
			
				
	end

	if nameTag ~= nil then
		fileUtilOut:write("/p>")				
		fileUtilOut:write("\n")	
	end

	fileUtilOut:write("</body>")
	fileUtilOut:write("\n")
	fileUtilOut:write("</html>")
	fileUtilOut:close()

end

local inFolder = "D:/mya/TintaPix/Tinta"
local outFolder = "D:/mya/TintaPix/Tinta/doc"

local pathUtil = string.format("%s%s", inFolder, "/tintaCommon/src/tintaUtilLocalEn.h")
local pathUtilOut = string.format("%s%s", outFolder, "/util.html" )

createDoc( pathUtil, pathUtilOut)

local pathUtil = string.format("%s%s", inFolder, "/tintaPix/src/tintaFunctions.h")
local pathUtilOut = string.format("%s%s", outFolder, "/tintaspring.html")

createDoc( pathUtil, pathUtilOut)

local pathUtil = string.format("%s%s", inFolder, "/tintaShape/src/tintaShapeLocalRu.h")
local pathUtilOut = string.format("%s%s", outFolder, "/tintashape.html")

createDoc( pathUtil, pathUtilOut)

util.msg("docs created in: ", outFolder )