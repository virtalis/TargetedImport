# CMakeLists.txt
cmake_minimum_required(VERSION 2.8.11)

set(VRTREE_SDK_DIR "C:/Virtalis/Visionary Render 2019.5.0/data/vrtreesdk/")

# Change this to whatever you want the plugin to be called
set(PluginName "TargetedImport")

project(${PluginName})

# Use unicode
ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)

include_directories(${VRTREE_SDK_DIR}/include)
link_directories(${VRTREE_SDK_DIR}/lib/x64)

add_library(${PluginName} SHARED dllmain.cpp pch.cpp pch.h framework.h)
target_link_libraries(${PluginName} vrtree-linker)

# Don't make ZERO CHECK
set(CMAKE_SUPPRESS_REGENERATION true)

# Builds the plugin directory
add_custom_command(TARGET ${PluginName} POST_BUILD
    COMMAND if not exist "$(SolutionDir)plugins" mkdir "$(SolutionDir)plugins"
    COMMAND if not exist "$(SolutionDir)plugins\\${PluginName}" mkdir "$(SolutionDir)plugins\\${PluginName}"
    COMMAND xcopy "$(TargetPath)" "$(SolutionDir)plugins\\${PluginName}" /y
    COMMAND @echo $(TargetFileName) > "$(SolutionDir)plugins\\${PluginName}\\plugin.txt"
    COMMENT "plugins folder created."
)