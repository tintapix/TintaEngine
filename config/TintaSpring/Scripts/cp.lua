-- file to execute for parallel execution
-- call this file from server console:  c_fl("cp.lua")

require "Scripts/lib/l_util"
require "config"

util.msg("start executing")
local outExt = "png"

-- scripts from Parallel folder
--c_fl("p_prior_color.lua")
c_fl("p_poster.lua")