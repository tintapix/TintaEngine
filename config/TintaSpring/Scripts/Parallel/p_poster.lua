require "Scripts/lib/l_util"
---------------------------------------

--c_print_msg(os.time())
math.randomseed( os.time() )

function main()




	local folderIn = "process"
	local dirSrc = string.format( '%s%s%s',pic_dir_in,"/", folderIn )
	local pathDest = pic_dir_filtered
	local extOut = "png"
	
	
	local strRequest = string.format( " require \"Scripts/lib/l_util\" s_poster(\"%s\", \"%s\" , \"c:/foto/rez/pallete.png\" )", "#1", "#2" )
	
	s_fileOperParallel(dirSrc, pathDest, extOut, strRequest, true )

end
-- for  explicit executing


main()


