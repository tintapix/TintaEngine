require "Scripts/lib/l_util"
require "paths"

	local  pathIn = "c:/foto/holes_test.png"
	local imgIdCalc = image.read( pathIn, s_getex(pathIn) )	

	
	
	image.fillholes(255,255,0,  200, 0.5 )
		
	
	image.save( "png","c:/foto/rez/holes_test_out.png" )
	
	image.erase(imgIdCalc)
	
	util.msg("finished")