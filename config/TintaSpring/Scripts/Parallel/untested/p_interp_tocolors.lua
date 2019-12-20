require "/lib/l_util"

---------------------------------------


math.randomseed( os.time() )
function main()

	local folderIn = "process"
	local dirSrc = string.format( '%s%s%s',pic_dir_in,"/", folderIn )
	local pathDest = pic_dir_filtered	
	local extOut = "png"
		
	local rInterp, gInterp, bInterp =  util.c_unpackcolor(0xdbf6d3)
	local rInterp2, gInterp2, bInterp2 =  util.c_unpackcolor(0xdbf6d3)
	
	local factor = 0.97
	local interpType = 0
	
	local strRequest = string.format( "c_executefunc( \"/lib/l_util\" , \"s_interpToColors\", \"%s\", \"%s\", %d, %d, %d, %d, %d, %d, %f,  %d, %f ) ", "#1", "#2", rInterp, gInterp, bInterp, rInterp2, gInterp2, bInterp2, factor, interpType , 0 )
	
	if c_gettasktype() == 1 then
		util.msg(strRequest)
		s_fileOperParallel(dirSrc, pathDest, extOut, strRequest )	
	end
end
-- for  explicit executing


main()


