cmake_minimum_required(VERSION 3.27)

set(BaseTargetName MidiTransposer)
set(PROJECT_VERSION 0.6)

project(${BaseTargetName}
        LANGUAGES CXX C
        VERSION ${PROJECT_VERSION})

if (APPLE)
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif ()

include(Utils.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Set any of these to "ON" if you want to build one of the juce examples
# or extras (Projucer/AudioPluginHost, etc):
option(JUCE_BUILD_EXTRAS "Build JUCE Extras" ON)
option(JUCE_BUILD_EXAMPLES "Build JUCE Examples" OFF)

# Include JUCE from the Git repository so it's a subdirectory of the project
update_from_git(juce "https://github.com/juce-framework/juce" "7.0.10" TRUE)

# Add external libraries
update_from_git(json "https://github.com/nlohmann/json" "v3.11.3" FALSE)

# Adds all the module sources so they appear correctly in the IDE
set(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Enable Module Source Groups" ON)
set_property(GLOBAL PROPERTY USE_FOLDERS YES)

if (MSVC)
    string(REGEX REPLACE "/W3" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    string(REGEX REPLACE "-W3" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif ()

# Add juce as a subdirectory:
add_subdirectory(${juce_SOURCE_DIR})

# Load gin module
FetchContent_Declare(gin
        GIT_REPOSITORY https://github.com/FigBug/Gin
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        GIT_TAG master
        SOURCE_SUBDIR modules)
FetchContent_MakeAvailable(gin)

# Generate the ProjectInfo struct.
set(PROJECT_COMPANY "Stfufane")
set(PROJECT_NAME "MidiTransposer")
set(PROJECT_VERSION_STRING "${PROJECT_VERSION}")
set(PROJECT_VERSION_NUMBER 0x00060000)
configure_file(ProjectInfo.h.in data/ProjectInfo.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/data)

juce_add_plugin("${BaseTargetName}"
        COMPANY_NAME "${PROJECT_COMPANY}"
        IS_SYNTH FALSE
        NEEDS_MIDI_INPUT TRUE
        NEEDS_MIDI_OUTPUT TRUE
        IS_MIDI_EFFECT TRUE
        EDITOR_WANTS_KEYBOARD_FOCUS FALSE
        COPY_PLUGIN_AFTER_BUILD FALSE
        PLUGIN_MANUFACTURER_CODE Stfu
        PLUGIN_CODE Mdtr
        FORMATS VST3
        PRODUCT_NAME "${PROJECT_NAME}")

# Add source folder
add_subdirectory(src)

target_compile_definitions(${BaseTargetName}
        PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        DONT_SET_USING_JUCE_NAMESPACE=1
        JUCE_VST3_CAN_REPLACE_VST2=0
        CONFIG_FOLDER="${CMAKE_CURRENT_LIST_DIR}/data/config")

target_link_libraries(${BaseTargetName} PRIVATE
        juce::juce_audio_utils
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
        gin
        nlohmann_json::nlohmann_json)
