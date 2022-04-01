require "Scripts/lib/l_util"
require "Scripts/lib/patterns"
require "Scripts/filters/image_filters"
require "Scripts/lib/interp"
require "config"

-- выполнить расчет на юните и вернть результат
-- расчет идет скриптом calc_unit.lua
-- call this file from server console:  main.fl("cp.lua")

if  increment == nil then
	increment = 0
end

increment = increment + 1

local val = string.format( " util.sleep(main.randint(1000,5000) ) increment = %d  main.fl(\"cp_calc_unit.lua\")", increment )
main.ed( val )





