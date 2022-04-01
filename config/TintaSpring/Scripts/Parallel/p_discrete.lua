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
	local pallete = "3132729095_2018314121458300_19542_pallete.png"
	--local strRequest = string.format( " require \"Scripts/lib/l_util\" s_maskchannel(\"%s\", \"%s\", 0,0,0, 0, 0.9, true )", "#1", "#2" )
	
	local strRequest = string.format( " require \"Scripts/lib/l_util\" s_discretecolor(\"%s\", \"%s\", 10, \"%s\",false,false, 0 )", "#1", "#2", pallete )
	
	s_fileOperParallel(dirSrc, pathDest, extOut, strRequest, true )

end
-- for  explicit executing


main()


