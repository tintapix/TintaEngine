require "Scripts/lib/l_util"



local fileIn = 	"D:/mya/meshes/pyramid.ASE"
local fileOut = "D:/mya/meshes/pyramid.mmesh"
			


local textBox = s_getStringArrayId( fileIn, "utf8", false )

local q = box.elements(textBox)
local vertexpart = false
local indexpart = false
local sRez = ""

for k = 0, q - 1 do


	local valLine = box.getvals(textBox,k)
	
	local regexVertNodematch = "(.*)(MESH_VERTEX_LIST)(.*)"
	local regexIndexNodematch = "(.*)(MESH_FACE_LIST)(.*)"
	local regexVertmatch = "(.*)(MESH_VERTEX)(.*)"
	local regexIndexmatch = "(.*)(MESH_FACE)(.*)"
	
		
	if main.regexmatch( regexVertNodematch, valLine,  0 ) == 1 then
		k = k + 1
		vertexpart = true
		sRez = string.format('%s%s',sRez, "vertices 	\n") 
	end
	
	if main.regexmatch( regexIndexNodematch, valLine,  0 ) == 1 then
		k = k + 1
		indexpart = true
		sRez = string.format('%s%s',sRez, "\nindices {	\n") 
		
	end
	
	if vertexpart == true then
		
		local match = main.regexmatch(regexVertmatch, valLine,  0)
		
		
		if match == 1 then
			sRez = string.format('%s%s',sRez, "{") 
			local regex = "[-+]?([0-9]*\\.[0-9]+|[0-9]+)"
			local iRez = main.regextoken( regex, valLine, "0", 0	)
			
			if iRez ~=  nil then
				local q1 = box.elements(iRez)
				
				for k1 = 1, q1 - 1 do				
					sRez = string.format('%s%s',sRez, box.getvals( iRez, k1 ))
					if k1 ~= q1-1 then
						sRez = string.format('%s%s',sRez, ",")
					end
				end
				sRez = string.format('%s%s', sRez, "},\n")
				--util.msg( sRez)
				image.erase( iRez )
			end
		else
			vertexpart = false
			sRez = string.format('%s%s',sRez, " },	") 
		end
		
	end
	
	if indexpart == true then
		
		local match = main.regexmatch(regexIndexmatch, valLine,  0)
		
		
		if match == 1 then
						
			local regex = "\\d+"
			local iRez = main.regextoken( regex, valLine, "0", 0	)
			
			if iRez ~=  nil then
				local q1 = box.elements(iRez)
				
				for k1 = 1, q1 - 1 do				
					
					if k1 == 1 or  k1 == 2 or k1 == 3 then -- taking only A B C indices 
						sRez = string.format('%s%s%s',sRez, box.getvals( iRez, k1 ) , ",\n")											
						
					end
				end
				--sRez = string.format('%s%s', sRez, ",")
				
				image.erase( iRez )
			end
		else
			indexpart = false
			sRez = string.format('%s%s',sRez, " } ") 
		end
		
	end
	
end
--util.util.msgf( sRez)
local idBoxOut = box.create("strs_vector")
box.pushbacks( idBoxOut, sRez )
--val = box.getvals( idBox, 0 )
c_writetofile(fileOut, "ascii", idBoxOut )


image.erase( textBox )
image.erase( idBoxOut )

local s = string.format('%s%s%s', "Export ", fileIn, "finished." )	
util.msg( s )