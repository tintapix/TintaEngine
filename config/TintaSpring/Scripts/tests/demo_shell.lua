require "Scripts/lib/l_util"
require "config"
require "config_unit1"

local file  = "3132729095_2016104135152999_28025_4.jpg"

local stringPathOut = string.format("%s\\%s",server_path, file)

local stringPathIn = string.format("%s\\%s",pic_dir_filtered_win, file)

util.msg(stringPathIn)

c_executeshell(string.format("copy %s %s ", stringPathIn, stringPathOut ))


