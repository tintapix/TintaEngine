require "paths"

-- configuration file may be executed on lua for correct
-- Available folders with scripts(including CL scripts and binaries) absolute paths may exist "c:\\scripts" or simple path - !! \\ must be removed by /
-- only double slesh using avalible otherwise slesh will be removed
Scripts_Folders = {
	"Scripts",
	"Scripts/work",
	"Scripts/torender",
	"Scripts/Euler",
	"Scripts/Parallel"	
	
}
GPUPrograms_Folders = {
	"OCL"
}

GPUPrograms = {
	path1 = { "user_filter1.cl" , "user_kernel_1_1" }
	--[[,
	path2 = { "user_filter1.cl" , "user_kernel_1_2" },
	path3 = { "user_filter2.cl" , "user_kernel_2_1" },
	path4 = { "user_filter2.cl" , "user_kernel_2_2" }]]--
	
}


Start_script = "util.timestamp(false)"

-- for server
--Ip_Adress = "172.21.3.215"
Ip_Adress = "127.0.0.1"
-- for client and server
Port = 5001

Local_Enabled = true

Unit_Reconnect_Time = 5000

App_Name = "ServerApp"
Gpu_Enabled = true
Gpu_Platform = 0
Gpu_Device = 0

Unit_Priority = 1

TrySend_Time =	5000




