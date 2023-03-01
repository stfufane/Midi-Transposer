cmake_minimum_required(VERSION 3.16)

set (BaseTargetName MidiBassPedalChords)
project(${BaseTargetName} 
LANGUAGES CXX
VERSION 0.0.2)

# Macro to include a github project
include(FetchContent QUIET)
macro (update_from_git name repo branch populate)
    message("Updating ${name} from git...")
    FetchContent_Declare(${name}
            GIT_REPOSITORY ${repo}
            GIT_SHALLOW TRUE
            GIT_PROGRESS TRUE
            GIT_TAG ${branch})

    # Somehow juce does not want to work with MakeAvailable :(
    if (${populate})
        FetchContent_GetProperties(${name})

        if (NOT ${name}_POPULATED)
            FetchContent_Populate(${name})
        endif ()
    else()
        FetchContent_MakeAvailable(${name})
    endif()
    message("${name} update finished")
endmacro()

set(WORKSPACE_ROOT ${CMAKE_CURRENT_LIST_DIR})

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

#set any of these to "ON" if you want to build one of the juce examples
#or extras (Projucer/AudioPluginHost, etc):
option(JUCE_BUILD_EXTRAS "Build JUCE Extras" ON)
option(JUCE_BUILD_EXAMPLES "Build JUCE Examples" OFF)

# Include the last JUCE version from the Git repository so it's a subdirectory of the project
update_from_git(juce "https://github.com/juce-framework/juce" "7.0.5" TRUE)
update_from_git(json "https://github.com/nlohmann/json" "v3.11.2" FALSE)

#Adds all the module sources so they appear correctly in the IDE
set_property(GLOBAL PROPERTY USE_FOLDERS YES)
option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Enable Module Source Groups" ON)

if(MSVC)
   string(REGEX REPLACE "/W3" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
   string(REGEX REPLACE "-W3" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
   set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

#adding juce as a subdirectory:
add_subdirectory(${juce_SOURCE_DIR})

#adding source folder
add_subdirectory(Source)
