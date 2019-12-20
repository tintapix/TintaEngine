require "Scripts/lib/l_util"
---------------------------------------

--c_print_msg(os.time())
math.randomseed( os.time() )

function main()




	local folderIn = "process"
	local dirSrc = string.format( '%s%s%s',pic_dir_in,"/", folderIn )
	local PathInterp = "C:/foto/3132729095_2018125121813805_6487.png"
	local pathDest = pic_dir_filtered
	local extIn = "png"
	local extOut = "png"
	local channel = 0
	--local strRequest = string.format( " require \"Scripts/lib/l_util\" s_maskchannel(\"%s\", \"%s\", 0,0,0, 0, 0.9, true )", "#1", "#2" )
	
	local strRequest = string.format( " require \"Scripts/lib/l_util\" s_interpToImageChannel(\"%s\", \"%s\",\"%s\", %d, 1, true )", "#1", "#2", PathInterp, channel )
	
	s_fileOperParallel(dirSrc, pathDest, extOut, strRequest, true )

end
-- for  explicit executing


main()


