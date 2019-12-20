require "Scripts/lib/l_util"
require "shared"

local mainImage = c_createimg( 100, 100 , "basic" )	

c_saveimg("png","\\\\developerserver\\Shared\\evdokimov\\test.png")

c_delimg(mainImage)
