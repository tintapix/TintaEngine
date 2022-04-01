require "Scripts/lib/l_util"

--[[
local idBuff = s_bufffromImageAlpha("C:/foto/rez/test_bitmap.png")
s_saveBufferToFile(idBuff, false, "C:/foto/rez/out.png")
image.erasebitmap(idBuff)
]]--

--[[
local idBuff = s_bufffromImageAlpha("C:/foto/rez/test_bitmap.png")
local buffOut = c_getoutline( idBuff, 2 )

s_saveBufferToFile(buffOut, false, "C:/foto/rez/out.png")
image.erasebitmap(idBuff)
image.erasebitmap(buffOut)
]]--


local idBuff = s_bufffromImageAlpha("C:/foto/rez/test_bitmap.png")
c_scalebitmap(idBuff, 2, 3 )

s_saveBufferToFile(idBuff, false, "C:/foto/rez/out.png")
image.erasebitmap(idBuff)

