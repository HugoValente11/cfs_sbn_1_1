 # Body file for function Function_2
 # Generated by TASTE on 2024-04-25 16:51:36
 # You can edit this file, it will not be overwritten
 # Provided interfaces : 
 # Required interfaces : PI_1
 # User-defined properties for this function:
 #   |_ Taste::FDIR = NO
 #   |_ TASTE::Is_Component_Type = false
 #   |_ Taste::Needs_datastore = NO
 #   |_ Taste::Startup_Priority = 100
 # Timers              : 
 #
cmake_minimum_required(VERSION 2.6.4)
project(FUNCTION_2 C)

include_directories(fsw/mission_inc)
include_directories(fsw/platform_inc)

aux_source_directory(fsw/src APP_SRC_FILES)


# Create the app module
add_cfe_app(function_2_app ${APP_SRC_FILES})

add_cfe_app_dependency(function_2_app dataview_lib)


# Executive services source files
target_include_directories(function_2_app PUBLIC ../../cfe/modules/es/fsw/src/)
target_link_libraries(function_2_app PRIVATE core_private)



