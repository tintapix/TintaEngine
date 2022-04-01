require "Scripts/lib/l_util"


function main()

	local folderIn = "process"
	local dirSrc = string.format( '%s%s%s',pic_dir_in,"/", folderIn )
	local pathDest = pic_dir_filtered
	local extIn = "png"
	local extOut = "png"
	local pallete = "3132729095_2018314121458300_19542_pallete.png"
	--local strRequest = string.format( " require \"Scripts/lib/l_util\" s_maskchannel(\"%s\", \"%s\", 0,0,0, 0, 0.9, true )", "#1", "#2" )
	
	local strRequest = string.format( " require \"Scripts/scale\" s_scalepixels(\"%s\", \"%s\",2.0, 2 )", "#1", "#2" )
	
	s_fileOperParallel(dirSrc, pathDest, extOut, strRequest, true )

end
-- for  explicit executing


main()