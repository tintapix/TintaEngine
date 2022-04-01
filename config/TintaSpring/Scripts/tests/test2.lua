require "Scripts/lib/l_util"
require "Scripts/lib/patterns"

require "paths"



file = io.open( "app.txt", "w" )

io.output( file )

function write_data()
	util.msg("writing...")
	io.write( main.getname(), "\n" )
	--io.close(file)
	io.flush() 
end 



c_start("write_data()", 500)

