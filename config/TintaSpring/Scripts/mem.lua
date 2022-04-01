require "Scripts/lib/l_util"
require "Scripts/lib/patterns"
require "paths"

local begTime = os.clock()
--s_printTimePoint(begTime, 0)

-- что бы не забивать в генератор случайных чисел опять os.time() делаем это только один раз
if seedInit == nil then
	-- инициализируем генератор случайных чисел
	math.randomseed( os.time() )
	util.msg( os.time() )
	seedInit = true
end

if calc == nil then
	calc = true
end

v1 = math.random(10,999)
v2 = math.random(10,999)


v3 = math.random(0,1)

text = ""

if calc == true then
	rez = 0	



	if v3 == 0 then
		rez = v1 + v2
		text = string.format(" %s + %s", v1,v2)
	elseif v3 == 1 then
		rez = v1 - v2
		text = string.format(" %s - %s", v1,v2)
	elseif v3 == 2 then
		v2 = math.random(4,9)
		rez = v1 * v2
		text = string.format(" %s * %s", v1,v2)
	end

end


if calc == true then
	util.msg(text)
	calc = false
else
	util.msg(rez)
	calc = true
end
