require "Scripts/lib/l_util"
require "filters/image_filters"
---------------------------------------

--c_print_msg(os.time())
math.randomseed( os.time() )

--[[
function main()

	local folderIn = "process"
	local dirSrc = string.format( '%s%s%s',pic_dir_in,"/", folderIn )
	
	pathDist = pic_dir_filtered

	extIn = "jpg"
	extOut = "png"
	
	local parallel = c_gettasktype() == 1	
	util.msg( parallel )
	
	local files  = util.c_countfiles(dirSrc, extIn)
	for i = 0, files - 1 do 
			local filFullPath = util.c_getfile(dirSrc, i ,extIn , false)
			--filename =     util.c_getfile(dirSrc, i ,extIn , true )		
			local fileout = string.format('%s%s%s%s%s', pathDist, "/", main.uniqid(), ".", extOut )		
			if parallel == true then
				strRequest = string.format( "c_executefunc( \"/filters/image_filters\", \"getStainBlack\",\"%s\", \"%s\", \"%s\", \"%s\") ", filFullPath, fileout, extIn, extOut )							
				c_executeparallel( strRequest, "" )				
				util.msg(strRequest)
			else
				getStainBlack(filFullPath, fileout, extIn, extOut)
			end
			
	end


end --main
]]--

function main()


	local folderIn = "process"
	local dirSrc = string.format( '%s%s%s',pic_dir_in,"/", folderIn )
	local pathDest = pic_dir_filtered
	local extIn = "png"
	local extOut = "png"
	
	local strRequest = string.format( "c_executefunc( \"/filters/image_filters\", \"getStainBlue\",\"%s\", \"%s\", \"%s\", \"%s\") ", "#1", "#2", extIn, extOut )
	if c_gettasktype() == 1 then
		s_fileOperParallel(dirSrc, pathDest, extOut, strRequest )
	else
		local fullPathSrc = string.format( '%s%s%s',dirSrc,"/", "s1.jpg" )
		local fullPathDest = string.format( '%s%s%s',dirSrc,"/", "IMG_6341_out.jpg" )
		getStainRed( fullPathSrc, fullPathDest, s_getex( fullPathSrc ), s_getex(fullPathDest ) )
	end
end
-- for  explicit executing


main()


