require "Scripts/lib/l_util"

---------------------------------------


math.randomseed( os.time() )
function main()

	local folderIn = "process"
	local dirSrc = string.format( '%s%s%s',pic_dir_in,"/", folderIn )
	local pathDest = pic_dir_filtered	
	local extOut = "png"
	--s_clearPicies( "C:/foto/1316239080.png","C:/foto/1316239080o.png" )
	local strRequest = string.format( "require \"/lib/l_util\" c_executefunc(\"/lib/l_util\", \"s_clearPicies\", \"%s\", \"%s\", %d) ", "#1", "#2", 60 )
	
	if c_gettasktype() == 1 then
		util.msg(strRequest)
		s_fileOperParallel(dirSrc, pathDest, extOut, strRequest )
	else	
		local fullPathSrc = string.format( '%s%s%s',dirSrc,"/", "IMG_6341.jpg" )
		local fullPathDest = string.format( '%s%s%s',dirSrc,"/", "IMG_6341_out.jpg" )
		s_clearPicies( fullPathSrc, fullPathDest )
		--getStainRed( fullPathSrc, fullPathDest, s_getex( fullPathSrc ), s_getex(fullPathDest ) )
	end
end
-- for  explicit executing


main()


