require "Scripts/lib/l_util"
---------------------------------------

--c_print_msg(os.time())
math.randomseed( os.time() )

function main()




	local folderIn = "process"
	local dirSrc = string.format( '%s%s%s',pic_dir_in,"/", folderIn )
	local pathDest = pic_dir_filtered
	local extIn = "png"
	local extOut = "png"
	
	local rVal, gVal,bVal = util.unpackcolor( 0xffc900 )
	local rVal2, gVal2,bVal2 = util.unpackcolor( 0xc000000 )

	--local strRequest = string.format( " require \"Scripts/lib/l_util\" s_maskchannel(\"%s\", \"%s\", 0,0,0, 0, 0.9, true )", "#1", "#2" )
	
	local strRequest = string.format( " require \"Scripts/lib/l_util\" s_changecolor(\"%s\", \"%s\", %d,%d,%d, %d,%d,%d )", "#1", "#2", rVal, gVal,bVal, rVal2, gVal2,bVal2 )
	
	s_fileOperParallel( dirSrc, pathDest, extOut, strRequest, true )

end
-- for  explicit executing


main()


