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

# Set any of these to "ON" if you want to build one of the juce examples
# or extras (Projucer/AudioPluginHost, etc):
option(JUCE_BUILD_EXTRAS "Build JUCE Extras" ON)
option(JUCE_BUILD_EXAMPLES "Build JUCE Examples" OFF)

# Include JUCE from the Git repository so it's a subdirectory of the project
update_from_git(juce "https://github.com/juce-framework/juce" "7.0.5" TRUE)

# Add external libraries
update_from_git(json "https://github.com/nlohmann/json" "v3.11.2" FALSE)
update_from_git(gin "https://github.com/FigBug/Gin" "8f73311" FALSE)

#Adds all the module sources so they appear correctly in the IDE
set_property(GLOBAL PROPERTY USE_FOLDERS YES)
option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Enable Module Source Groups" ON)

if(MSVC)
   string(REGEX REPLACE "/W3" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
   string(REGEX REPLACE "-W3" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
   set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# Add juce as a subdirectory:
add_subdirectory(${juce_SOURCE_DIR})

# Load gin module
juce_add_module(${gin_SOURCE_DIR}/modules/gin)

juce_add_plugin("${BaseTargetName}"
        VERSION 0.2                                 # Set this if the plugin version is different to the project version
        # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
        # ICON_SMALL ...
        COMPANY_NAME "Stfufane"
        IS_SYNTH FALSE
        NEEDS_MIDI_INPUT TRUE
        NEEDS_MIDI_OUTPUT TRUE
        IS_MIDI_EFFECT TRUE
        EDITOR_WANTS_KEYBOARD_FOCUS FALSE
        COPY_PLUGIN_AFTER_BUILD FALSE
        PLUGIN_MANUFACTURER_CODE Stfu
        PLUGIN_CODE Mbpc
        FORMATS VST3
        PRODUCT_NAME "MidiBassPedalChords")

# Add source folder
add_subdirectory(src)

juce_generate_juce_header(${BaseTargetName})

target_compile_definitions(${BaseTargetName}
        PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        DONT_SET_USING_JUCE_NAMESPACE=1
        JUCE_VST3_CAN_REPLACE_VST2=0
        CONFIG_FOLDER="${WORKSPACE_ROOT}/data/config")

target_link_libraries(${BaseTargetName} PRIVATE
        juce::juce_audio_utils
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
        gin
        nlohmann_json::nlohmann_json)
