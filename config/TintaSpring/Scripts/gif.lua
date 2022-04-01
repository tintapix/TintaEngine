require "Scripts/lib/l_util"
require "Scripts/lib/patterns"

require "paths"

local begTime = os.clock()


-- что бы не забивать в генератор случайных чисел опять os.time() делаем это только один раз
if seedInit == nil then
	-- инициализируем генератор случайных чисел
	math.randomseed( os.time() )
	util.msg( os.time() )
	seedInit = true
end


clearAnimation( "c:/foto/gif", "png", "c:/foto/out" )



	

