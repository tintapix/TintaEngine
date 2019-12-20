require "Scripts/lib/l_util"

function main()


	local folderIn = "process"
	local dirSrc = string.format( '%s%s%s',pic_dir_in,"/", folderIn )
	local pathDest = pic_dir_filtered
	local extIn = "jpg"
	local extOut = "png"
	local factor = 0.98
	local strRequest = string.format( "c_executefunc( \"/lib/l_util\", \"s_interpToColor\",\"%s\",\"%s\", c_randint(220,255), c_randint(217,217), c_randint(217,217), %f )" , "#1", "#2", factor )
	
	if c_gettasktype() == 1 then
		s_fileOperParallel(dirSrc, pathDest, extOut, strRequest )
	else
		c_assert(false , "The operation is not parallel!")
	end
end
-- for  explicit executing


main()