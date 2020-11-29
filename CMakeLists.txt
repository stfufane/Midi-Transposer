cmake_minimum_required(VERSION 3.16)

set (BaseTargetName MidiBassPedalChords)
project(${BaseTargetName} VERSION 0.0.1)

# Macro to include a github project
include(FetchContent QUIET)
macro (update_from_git name repo branch)
    FetchContent_Declare(${name}
            GIT_REPOSITORY ${repo}
            GIT_SHALLOW TRUE
            GIT_PROGRESS TRUE
            GIT_TAG "origin/${branch}")

    FetchContent_GetProperties(${name})

    if (NOT ${name}_POPULATED)
        message("Updating ${name} from git...")
        FetchContent_Populate(${name})
        message("${name} update finished")
    endif ()
endmacro()

# Function to build the binary data from a folder of images
function(add_images_from_directory target imagesSubFolder)
    set (ImagesDir ${CMAKE_CURRENT_LIST_DIR}/${imagesSubFolder})
    file(GLOB_RECURSE images
            "${ImagesDir}/*.jpg"
            "${ImagesDir}/*.png"
            "${ImagesDir}/*.jpeg")

    if(NOT images STREQUAL "")
        set (ImagesTarget "${target}-Images")
        juce_add_binary_data(${ImagesTarget} SOURCES ${images})
        target_link_libraries(${target} PRIVATE ${ImagesTarget})
    endif()
endfunction()

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Include the last JUCE version from the Git repository so it's a subdirectory of the project
update_from_git(juce "https://github.com/juce-framework/juce" master)

#Adds all the module sources so they appear correctly in the IDE
set_property(GLOBAL PROPERTY USE_FOLDERS YES)
option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Enable Module Source Groups" ON)

#set any of these to "ON" if you want to build one of the juce examples
#or extras (Projucer/AudioPluginHost, etc):
option(JUCE_BUILD_EXTRAS "Build JUCE Extras" OFF)
option(JUCE_BUILD_EXAMPLES "Build JUCE Examples" OFF)

#adding juce as a subdirectory:
add_subdirectory(${juce_SOURCE_DIR})

if(MSVC)
   string(REGEX REPLACE "/W3" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
   string(REGEX REPLACE "-W3" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
endif()

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
	COPY_PLUGIN_AFTER_BUILD TRUE
	PLUGIN_MANUFACTURER_CODE Stfu
	PLUGIN_CODE Mbpc
	FORMATS VST3
	PRODUCT_NAME "MidiBassPedalChords")

FILE(GLOB CppSources Source/*.cpp)
target_sources(${BaseTargetName} PRIVATE
        ${CppSources})
		
add_images_from_directory(${BaseTargetName} Resources)

juce_generate_juce_header(${BaseTargetName})

target_compile_definitions(${BaseTargetName}
	PUBLIC
	JUCE_WEB_BROWSER=0
	JUCE_USE_CURL=0
	JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(${BaseTargetName} PRIVATE
	juce::juce_audio_utils
	juce::juce_recommended_config_flags
	juce::juce_recommended_lto_flags
	juce::juce_recommended_warning_flags)