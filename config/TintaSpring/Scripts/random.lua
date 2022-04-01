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

--util.msg(isPrime( 17 ))

local A = 48271
local M = 2147483647
local Q = math.floor(M / A)
local R = math.floor(M % A)

if val == nil then
	-- seed
	seed = 3294823095724555
end

function randomVal( maxVal )

	--[[
	int tmpState = A * ( state % Q ) - R * ( state / Q ); 
	11 if ( tmpState >= 0 ) 
	12 state = tmpState; 
	13 else 
	14 state = tmpstate + M; 
	15 
	16 return state; 	
	]]--
	
	val = (A * math.floor(seed % Q )) - ( R * math.floor( seed / Q ) ) 
	--util.msgf(val)
	if val < 0 then
		val = val + M
	end
	
	seed = val
	--util.msg(seed)
	val = math.floor(val % maxVal)
	--util.msg(val)
	
	return val
end

local outImage = image.create( 150, 150 , "basic" )

c_fillalphaf( 1 )
local w = image.width()
local h = image.height()				

for h_ = 0, h-1 do				
	for w_ = 0, w -1 do				
		r = randomVal( 255 )
		
		--util.msgf(r)--, " ", g ," ", b)
		image.setpixelb( w_, h_, r,255,255, 1 )		
	end		
end	
 

image.save( "png", "c:/foto/rez/random.png" )		
image.erase( outImage )

