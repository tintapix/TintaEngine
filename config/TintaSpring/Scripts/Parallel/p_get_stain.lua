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
	
	--[[
		channelIn - 0 - r
		channelIn - 1 - g
		channelIn - 2 - b
		channelIn - 3 - a	
		channelIn - 4 - light
	]]--
	
	local channel = 1
	local factor = 0.5
	
	--local strRequest = string.format( " require \"Scripts/lib/l_util\" s_getStain(\"%s\", \"%s\", %d, %d )", "#1", "#2", factor)--, ch )
	local strRequest = string.format( " require \"Scripts/filters/image_filters\" s_getStain(\"%s\", \"%s\" , %f , %d )", "#1", "#2", factor, channel )
	--util.msg(strRequest)
	s_fileOperParallel(dirSrc, pathDest, extOut, strRequest )
	
end

main()


