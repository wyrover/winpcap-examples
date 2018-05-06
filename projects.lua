BOOK_CODE_PATH = "E:/book-code"
THIRD_PARTY = "E:/book-code/3rdparty"
WORK_PATH = os.getcwd()
includeexternal (WORK_PATH .. "/premake-vs-include.lua")


workspace "winpcap-examples"
    language "C++"
    location "build/%{_ACTION}/%{wks.name}"    
    if _ACTION == "vs2015" then
        toolset "v140_xp"
    elseif _ACTION == "vs2013" then
        toolset "v120_xp"
    end

    
--    group "google"
--
--
--        project "gtest"            
--            removeconfigurations "TRACE*"   
--            kind "StaticLib"     
--            defines { "GTEST_HAS_PTHREAD=0", "_HAS_EXCEPTIONS=1" }            
--            files
--            {
--            
--                "%{THIRD_PARTY}/googletest/googletest/src/gtest-all.cc"
--                     
--                               
--            }             
--            includedirs
--            {
--               
--                "%{THIRD_PARTY}/googletest/googletest/include",
--                "%{THIRD_PARTY}/googletest/googletest"
--            }
--            
--        
--    
--
--        project "gtest_main"     
--            removeconfigurations "TRACE*"   
--            kind "StaticLib"     
--            defines { "GTEST_HAS_PTHREAD=0", "_HAS_EXCEPTIONS=1" }            
--            files
--            {
--            
--                "%{THIRD_PARTY}/googletest/googletest/src/gtest-all.cc",
--                "%{THIRD_PARTY}/googletest/googletest/src/gtest_main.cc"
--                  
--                               
--            }             
--            includedirs
--            {
--               
--                "%{THIRD_PARTY}/googletest/googletest/include",
--                "%{THIRD_PARTY}/googletest/googletest"
--            }
--            
--        
--            
--    
--   
--
--        project "gmock"         
--            removeconfigurations "TRACE*"   
--            kind "StaticLib"     
--            defines { "GTEST_HAS_PTHREAD=0", "_HAS_EXCEPTIONS=1" }            
--            files
--            {
--            
--                "%{THIRD_PARTY}/googletest/googletest/src/gtest-all.cc",
--                "%{THIRD_PARTY}/googletest/googlemock/src/gmock-all.cc"      
--                               
--            }             
--            includedirs
--            {
--               
--                "%{THIRD_PARTY}/googletest/googletest/include",
--                "%{THIRD_PARTY}/googletest/googletest",
--                "%{THIRD_PARTY}/googletest/googlemock/include",
--                "%{THIRD_PARTY}/googletest/googlemock"
--
--            }
--            
--        
--    
--    
--
--        project "gmock_main"   
--            removeconfigurations "TRACE*"   
--            kind "StaticLib"     
--            defines { "GTEST_HAS_PTHREAD=0", "_HAS_EXCEPTIONS=1" }            
--            files
--            {
--            
--                
--                "%{THIRD_PARTY}/googletest/googletest/src/gtest-all.cc",
--                "%{THIRD_PARTY}/googletest/googlemock/src/gmock-all.cc",
--                "%{THIRD_PARTY}/googletest/googlemock/src/gmock_main.cc"
--                               
--            }             
--            includedirs
--            {       
--               
--                "%{THIRD_PARTY}/googletest/googletest/include",
--                "%{THIRD_PARTY}/googletest/googletest",
--                "%{THIRD_PARTY}/googletest/googlemock/include",
--                "%{THIRD_PARTY}/googletest/googlemock"
--
--            }
--
--    group "glog"
--        project "glog"            
--            removeconfigurations "TRACE*"   
--            kind "StaticLib"
--            defines { "GOOGLE_GLOG_DLL_DECL=", "HAVE_SNPRINTF" }
--            files
--            {
--                "%{THIRD_PARTY}/glog-master/src/logging.cc",
--                "%{THIRD_PARTY}/glog-master/src/windows/port.cc",
--                "%{THIRD_PARTY}/glog-master/src/raw_logging.cc",
--                "%{THIRD_PARTY}/glog-master/src/utilities.cc",
--                "%{THIRD_PARTY}/glog-master/src/vlog_is_on.cc",
--                ------------------------------------------------------------------------------------------------------------
--                "%{THIRD_PARTY}/glog-master/src/base/commandlineflags.h",
--                "%{THIRD_PARTY}/glog-master/src/windows/config.h",
--                "%{THIRD_PARTY}/glog-master/src/base/googleinit.h",
--                "%{THIRD_PARTY}/glog-master/src/windows/glog/log_severity.h",
--                "%{THIRD_PARTY}/glog-master/src/windows/glog/logging.h",
--                "%{THIRD_PARTY}/glog-master/src/base/mutex.h",
--                "%{THIRD_PARTY}/glog-master/src/windows/port.h",
--                "%{THIRD_PARTY}/glog-master/src/windows/glog/raw_logging.h",
--                "%{THIRD_PARTY}/glog-master/src/windows/glog/stl_logging.h",
--                "%{THIRD_PARTY}/glog-master/src/utilities.h",
--                "%{THIRD_PARTY}/glog-master/src/windows/glog/vlog_is_on.h"
--            }
--            vpaths 
--            { 
--                ["Header Files"] = {"**.h", "**.hpp"},
--                ["Source Files"] = {"**.c", "**.cpp"}
--            }
--            includedirs
--            {
--                "%{THIRD_PARTY}/glog-master/src/windows",
--                            
--            }
--
--    group "gflags"
--        project "gflags"      
--            removeconfigurations "TRACE*"   
--            kind "StaticLib"
--            characterset "MBCS"
--            defines { "GFLAGS_IS_A_DLL=0" }
--            files
--            {
--                "%{THIRD_PARTY}/gflags-master/src/**.cc",
--                "%{THIRD_PARTY}/gflags-master/src/**.h",
--                "%{THIRD_PARTY}/gflags-master/bin/include/gflags/**.h",
--            }
--            vpaths 
--            { 
--                ["Header Files"] = {"**.h", "**.hpp"},
--                ["Source Files"] = {"**.c", "**.cpp", "**.cc"}
--            }
--            includedirs
--            {
--                "%{THIRD_PARTY}/gflags-master/bin/include/gflags",
--                "%{THIRD_PARTY}/gflags-master/bin/include",
--                            
--            }
--
--        project "gflags_nothreads"        
--            removeconfigurations "TRACE*"   
--            kind "StaticLib"
--            characterset "MBCS"
--            defines { "GFLAGS_IS_A_DLL=0", "NO_THREADS" }
--            files
--            {
--                "%{THIRD_PARTY}/gflags-master/src/**.cc",
--                "%{THIRD_PARTY}/gflags-master/src/**.h",
--                "%{THIRD_PARTY}/gflags-master/bin/include/gflags/**.h",
--            }
--            vpaths 
--            { 
--                ["Header Files"] = {"**.h", "**.hpp"},
--                ["Source Files"] = {"**.c", "**.cpp", "**.cc"}
--            }
--            includedirs
--            {
--                "%{THIRD_PARTY}/gflags-master/bin/include/gflags",
--                "%{THIRD_PARTY}/gflags-master/bin/include",
--                            
--            }

  
    
    function create_winpacp_example_project(name, dir)        
        project(name)          
        kind "ConsoleApp"                                             
        files
        {                                  
            dir .. "/%{prj.name}/**.h",
            dir .. "/%{prj.name}/**.cpp", 
            dir .. "/%{prj.name}/**.c", 
            dir .. "/%{prj.name}/**.rc" 
        }
        removefiles
        {               
        }
        includedirs
        {               
            "3rdparty/winpcap/Include",          
        }        
        
        filter { "platforms:Win32" }             
            libdirs
            {
                "3rdparty/winpcap/Lib",
            }
            links
            {
                "Packet.lib",
                "wpcap.lib",
                "ws2_32"
            }
            has_stdafx(name, dir)   
        filter "platforms:x64"
            libdirs
            {
                "3rdparty/winpcap/Lib/x64",
            }
            links
            {
                "Packet.lib",
                "wpcap.lib",
                "ws2_32"
            }
            targetsuffix "x64"
            has_stdafx(name, dir)               
    end

    group "test"       

        
        

        create_winpacp_example_project("example-001", "src")
        create_winpacp_example_project("example-002", "src")
        create_winpacp_example_project("example-003", "src")
        create_winpacp_example_project("example-004", "src")
        create_winpacp_example_project("example-005", "src")
        create_winpacp_example_project("example-006", "src")
        create_winpacp_example_project("example-007", "src")
        create_winpacp_example_project("example-008", "src")
        create_winpacp_example_project("example-009", "src")
        create_winpacp_example_project("example-010", "src")
        create_winpacp_example_project("example-011", "src")
        create_winpacp_example_project("example-012", "src")
        create_winpacp_example_project("example-013", "src")
        create_winpacp_example_project("example-014", "src")
        create_winpacp_example_project("example-015", "src")
        create_winpacp_example_project("example-016", "src")
        create_winpacp_example_project("example-017", "src")
        create_winpacp_example_project("example-018", "src")
        create_winpacp_example_project("example-019", "src")
            

--        create_console_project("CreateSignature", "src")
--            includedirs
--            {
--                "src/CryptoApi"
--            }
--            links
--            {
--                
--                "crypt32.lib",
--                
--            }
--           
--        create_console_project("CryptoApiTest", "src")
--            includedirs
--            {
--                "src/CryptoApi",
--                "%{THIRD_PARTY}/googletest/googletest/include",
--                "%{THIRD_PARTY}/googletest/googletest",
--                "%{THIRD_PARTY}/googletest/googlemock/include",
--                "%{THIRD_PARTY}/googletest/googlemock"
--            }
--            links
--            {
--                "gtest",
--                "crypt32.lib",                
--            }
--
--
--        create_console_project("test-base64", "src")
--        create_console_charset_project("rsa_tool", "src", "mbcs")
--        create_console_charset_project("base64", "src", "mbcs")
--        create_console_charset_project("rsa_key_encode", "src", "mbcs")
--        create_console_charset_project("rsa_key_decode", "src", "mbcs")
--
--        create_console_charset_project("dsa_gen", "src", "mbcs")
--        create_console_charset_project("rsa_gen", "src", "mbcs")
--        
--        create_console_project("rsacert", "src")
--            links
--            {
--                
--                "crypt32.lib",                
--            }
       
            