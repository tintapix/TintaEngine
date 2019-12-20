require "Scripts/lib/l_util"



local fileIn = 	"D:/mya/meshes/pyramid.ASE"
local fileOut = "D:/mya/meshes/pyramid.mmesh"
			


local textBox = s_getStringArrayId( fileIn, "utf8", false )

local q = c_boxelements(textBox)
local vertexpart = false
local indexpart = false
local sRez = ""

for k = 0, q - 1 do


	local valLine = c_getvalboxs(textBox,k)
	
	local regexVertNodematch = "(.*)(MESH_VERTEX_LIST)(.*)"
	local regexIndexNodematch = "(.*)(MESH_FACE_LIST)(.*)"
	local regexVertmatch = "(.*)(MESH_VERTEX)(.*)"
	local regexIndexmatch = "(.*)(MESH_FACE)(.*)"
	
		
	if c_regexmatch( regexVertNodematch, valLine,  0 ) == 1 then
		k = k + 1
		vertexpart = true
		sRez = string.format('%s%s',sRez, "vertices 	\n") 
	end
	
	if c_regexmatch( regexIndexNodematch, valLine,  0 ) == 1 then
		k = k + 1
		indexpart = true
		sRez = string.format('%s%s',sRez, "\nindices {	\n") 
		
	end
	
	if vertexpart == true then
		
		local match = c_regexmatch(regexVertmatch, valLine,  0)
		
		
		if match == 1 then
			sRez = string.format('%s%s',sRez, "{") 
			local regex = "[-+]?([0-9]*\\.[0-9]+|[0-9]+)"
			local iRez = c_regextoken( regex, valLine, "0", 0	)
			
			if iRez ~=  nil then
				local q1 = c_boxelements(iRez)
				
				for k1 = 1, q1 - 1 do				
					sRez = string.format('%s%s',sRez, c_getvalboxs( iRez, k1 ))
					if k1 ~= q1-1 then
						sRez = string.format('%s%s',sRez, ",")
					end
				end
				sRez = string.format('%s%s', sRez, "},\n")
				--util.msg( sRez)
				c_delbox( iRez )
			end
		else
			vertexpart = false
			sRez = string.format('%s%s',sRez, " },	") 
		end
		
	end
	
	if indexpart == true then
		
		local match = c_regexmatch(regexIndexmatch, valLine,  0)
		
		
		if match == 1 then
						
			local regex = "\\d+"
			local iRez = c_regextoken( regex, valLine, "0", 0	)
			
			if iRez ~=  nil then
				local q1 = c_boxelements(iRez)
				
				for k1 = 1, q1 - 1 do				
					
					if k1 == 1 or  k1 == 2 or k1 == 3 then -- taking only A B C indices 
						sRez = string.format('%s%s%s',sRez, c_getvalboxs( iRez, k1 ) , ",\n")											
						
					end
				end
				--sRez = string.format('%s%s', sRez, ",")
				
				c_delbox( iRez )
			end
		else
			indexpart = false
			sRez = string.format('%s%s',sRez, " } ") 
		end
		
	end
	
end
--util.util.msgf( sRez)
local idBoxOut = c_createbox("strs_vector")
c_pushbackboxs( idBoxOut, sRez )
--val = c_getvalboxs( idBox, 0 )
c_writetofile(fileOut, "ascii", idBoxOut )


c_delbox( textBox )
c_delbox( idBoxOut )

local s = string.format('%s%s%s', "Export ", fileIn, "finished." )	
util.msg( s )