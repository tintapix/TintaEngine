require "Scripts/lib/l_util"
require "Scripts/lib/patterns"

require "paths"

if seedInit == nil then
	-- инициализируем генератор случайных чисел
	math.randomseed( os.time() )
	util.msg( os.time() )
	seedInit = true
end

image.eraseall()


pathIn = "C:/foto/3132729095_2019829162913698_15333.png"
local picImg2 = image.read( pathIn, ext )

w = image.width()
h = image.height()

window.redraw()

function key() 
	util.msg("key")
end

function timer() 
	util.msg("timer")
	
	for w_ = 0, w - 1 do	
			for h_ = 0, h - 1 do				
				
				image.setpixelb( w_, h_, math.random(0,255),math.random(0,255),math.random(0,255), 1 )												
				
			end		
	end	
	
	window.redraw()
	
end

main.start(2000, "timer()")
window.setkey(0x1D, "key()")

