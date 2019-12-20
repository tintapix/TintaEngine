require "/lib/l_util"

---------------------------------------


math.randomseed( os.time() )
function main()

	local folderIn = "process"
	local dirSrc = string.format( '%s%s%s',pic_dir_in,"/", folderIn )
	local pathDest = pic_dir_filtered	
	local extOut = "png"
		
	local rInterp, gInterp, bInterp =  util.c_unpackcolor(0xff9500)
	
	local factor = 0.99
	
	local strRequest = string.format( "c_executefunc( \"/lib/l_util\" , \"s_interpToColor\", \"%s\", nil, \"%s\", %d, %d, %d, %f, true ) ", "#1", "#2", rInterp, gInterp, bInterp, factor )
		
	s_fileOperParallel(dirSrc, pathDest, extOut, strRequest )	
	
	
end
-- for  explicit executing


main()


