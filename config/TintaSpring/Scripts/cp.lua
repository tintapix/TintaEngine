-- file to execute for parallel execution
-- call this file from server console:  main.fl("cp.lua")

require "Scripts/lib/l_util"
require "config"

util.msg("start executing")
local outExt = "png"

-- scripts from Parallel folder
--main.fl("p_prior_color.lua")
main.fl("Scripts/Parallel/p_scale.lua")