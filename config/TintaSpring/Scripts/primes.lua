require "Scripts/lib/l_util"
require "Scripts/lib/patterns"

require "paths"

local begTime = os.clock()
s_printTimePoint(begTime, 0)

-- что бы не забивать в генератор случайных чисел опять os.time() делаем это только один раз
if seedInit == nil then
	-- инициализируем генератор случайных чисел
	math.randomseed( os.time() )
	util.msg( os.time() )
	seedInit = true
end

-- Малая  теорема ферма
--[[
	Если p — простое число и a — целое число, не делящееся на p, то  a^{p-1}-1}a^{p-1}-1 делится на p.
]]--
function isPrime( val )

	if val % 2 == 0 then
		return false
	end
	
	i = 3
	while  i * i < val or i * i == val  do
				
		if val % i == 0 then	
			return false	
		end
		
		i = i + 2
	end

	return true;

end
--util.msg(math.log(0.00000000000000000000001))
for i = 0, 99999999999 do
	if isPrime( i ) then
		util.msg( i )
	end
end

util.msg("finished")