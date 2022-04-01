require "Scripts/lib/l_util"
require "Scripts/filters/image_filters"
---------------------------------------

--c_print_msg(os.time())
math.randomseed( os.time() )


function main()


	local folderIn = "process"
	local dirSrc = string.format( '%s%s%s',pic_dir_in,"/", folderIn )
	local pathDest = pic_dir_filtered
	local extIn = "jpg"
	local extOut = "png"
	
	local strRequest = string.format( "  require \"Scripts/lib/l_util\" s_maskchannel(\"%s\", \"%s\", 0,0,0, 4, 0.9, true )", "#1", "#2" )
	
	s_fileOperParallel(dirSrc, pathDest, extOut, strRequest )
end
-- for  explicit executing


main()


