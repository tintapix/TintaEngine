require "Scripts/lib/l_util"

---------------------------------------


math.randomseed( os.time() )
function main()

	local folderIn = "process"
	local dirSrc = string.format( '%s%s%s',pic_dir_in,"/", folderIn )
	local pathDest = pic_dir_filtered	
	local extOut = "png"	
	--s_fillHoles("C:/foto/rez6/fix/1316239080.png", "C:/foto/rez6/fix/1316239080o.png", 6, 3 )
	local around = 3
	local dist = 3
	local strRequest = string.format( "c_executefunc( \"s_fillHoles\", \"%s\", \"%s\", %d, %d ) ", "#1", "#2", around, dist )
	
	if c_gettasktype() == 1 then
		util.msg(strRequest)
		--s_fileOperParallel(dirSrc, pathDest, extOut, strRequest )
	else
		local fullPathSrc = string.format( '%s%s%s',dirSrc,"/", "378782324.png" )
		local fullPathDest = string.format( '%s%s%s',pathDest,"/", "378782324.png" )
		s_fillHoles( fullPathSrc, fullPathDest,3,2 )
		--getStainRed( fullPathSrc, fullPathDest, s_getex( fullPathSrc ), s_getex(fullPathDest ) )
	end
end
-- for  explicit executing


main()


