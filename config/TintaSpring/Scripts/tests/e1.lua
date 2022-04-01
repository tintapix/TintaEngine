require "Scripts/lib/l_util"
require "shared"

local mainImage = image.create( 100, 100 , "basic" )	

image.save("png","\\\\developerserver\\Shared\\evdokimov\\test.png")

image.erase(mainImage)
