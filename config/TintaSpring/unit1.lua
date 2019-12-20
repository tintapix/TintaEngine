require "paths"

-- configuration file may be executed on lua for correct
-- Available folders with scripts(including CL scripts and binaries) absolute paths may exist "c:\\scripts" or simple path - !! \\ must be removed by /
-- only double slesh using avalible otherwise slesh will be removed
Scripts_Folders = {
	"Scripts"	
}
GPUPrograms_Folders = {
	"OCL"
}

GPUPrograms = {		
}

Start_script = str_null --test2.lua"

-- for server
--Ip_Adress = "172.21.3.215"
Ip_Adress = "127.0.0.1"
-- for client and server
Port = 5001

Local_Enabled = true
Unit_Reconnect_Time = 5000

App_Name = "Unit1"
Gpu_Enabled = false
Unit_Priority = 1

TrySend_Time =	5000


