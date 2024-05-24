cmake_minimum_required(VERSION 3.27)

set(PROJECT_VERSION 0.6)

project(MidiTransposer
    LANGUAGES CXX C
    VERSION ${PROJECT_VERSION})

if(APPLE)
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()

if(MSVC)
    string(REGEX REPLACE "/W3" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    string(REGEX REPLACE "-W3" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Set any of these to "ON" if you want to build one of the juce examples
# or extras (Projucer/AudioPluginHost, etc):
option(JUCE_BUILD_EXTRAS "Build JUCE Extras" ON)
option(JUCE_BUILD_EXAMPLES "Build JUCE Examples" OFF)

# Adds all the module sources so they appear correctly in the IDE
set(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Enable Module Source Groups" ON)
set_property(GLOBAL PROPERTY USE_FOLDERS YES)

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)

include(get_cpm.cmake)

# Include JUCE from the Git repository
CPMAddPackage(
    NAME JUCE
    GIT_TAG 7.0.12
    GITHUB_REPOSITORY juce-framework/JUCE
    SOURCE_DIR ${LIB_DIR}/juce
)

# Add external libraries
CPMAddPackage(
    NAME json
    GIT_TAG v3.11.3
    GITHUB_REPOSITORY nlohmann/json
    SOURCE_DIR ${LIB_DIR}/json
)

CPMAddPackage(NAME Gin
    GITHUB_REPOSITORY FigBug/Gin
    DOWNLOAD_ONLY TRUE
    SOURCE_DIR ${LIB_DIR}/gin
    GIT_TAG master)

# Manually add the gin module (the others are not required)
juce_add_module(${LIB_DIR}/gin/modules/gin)

# Generate the ProjectInfo struct.
set(PROJECT_COMPANY "Stfufane")
set(PROJECT_VERSION_STRING "${PROJECT_VERSION}")
set(PROJECT_VERSION_NUMBER 0x00060000)
configure_file(ProjectInfo.h.in data/ProjectInfo.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/data)

juce_add_plugin("${PROJECT_NAME}"
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
    PRODUCT_NAME "Midi Transposer")

# List the sources files to compile
target_sources(${PROJECT_NAME} PRIVATE
    src/processor/MidiProcessor.h
    src/processor/MidiProcessor.cpp
    src/processor/PluginProcessor.h
    src/processor/PluginProcessor.cpp
    src/params/Params.h
    src/params/ArpeggiatorParams.cpp
    src/params/IntervalParam.cpp
    src/params/MidiParams.cpp
    src/params/NoteParam.cpp
    src/params/NoteParams.cpp
    src/presets/PresetManager.h
    src/presets/PresetManager.cpp
    src/gui/PluginEditor.h
    src/gui/PluginEditor.cpp
    src/gui/CompColors.h
    src/gui/CompCoordinates.h
    src/gui/Configuration.hpp
    src/gui/UISettings.h
    src/gui/lookandfeel/BaseLookAndFeel.h
    src/gui/lookandfeel/BaseLookAndFeel.cpp
    src/gui/widgets/CustomSlider.h
    src/gui/widgets/CustomSlider.cpp
    src/gui/widgets/CustomToggleButton.h
    src/gui/widgets/CustomToggleButton.cpp
    src/gui/widgets/NoteKey.h
    src/gui/widgets/NoteKey.cpp
    src/gui/widgets/TextSwitch.h
    src/gui/widgets/TextSwitch.cpp
    src/gui/widgets/Helpers.h
    src/gui/panels/MainPanel.h
    src/gui/panels/MainPanel.cpp
    src/gui/panels/MidiPanel.h
    src/gui/panels/MidiPanel.cpp
    src/gui/panels/ArpPanel.h
    src/gui/panels/ArpPanel.cpp
    src/gui/panels/IntervalsPanel.h
    src/gui/panels/IntervalsPanel.cpp
    src/gui/panels/KeysPanel.h
    src/gui/panels/KeysPanel.cpp
    src/gui/panels/PresetsPanel.h
    src/gui/panels/PresetsPanel.cpp)

# Build the binary data
set(DataTarget "${PROJECT_NAME}-Data")
juce_add_binary_data(${DataTarget} SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/data/fonts/iosevka-regular.ttf
    ${CMAKE_CURRENT_LIST_DIR}/data/img/new.svg
    ${CMAKE_CURRENT_LIST_DIR}/data/img/save.svg
    ${CMAKE_CURRENT_LIST_DIR}/data/img/copy.svg
    ${CMAKE_CURRENT_LIST_DIR}/data/img/delete.svg
    ${CMAKE_CURRENT_LIST_DIR}/data/img/previous.svg
    ${CMAKE_CURRENT_LIST_DIR}/data/img/next.svg)

target_link_libraries(${PROJECT_NAME} PRIVATE ${DataTarget})

# Set the base for includes to avoid having to use relative paths
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_definitions(${PROJECT_NAME}
    PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    DONT_SET_USING_JUCE_NAMESPACE=1
    JUCE_VST3_CAN_REPLACE_VST2=0
    CONFIG_FOLDER="${CMAKE_CURRENT_LIST_DIR}/data/config")

target_link_libraries(${PROJECT_NAME} PRIVATE
    juce::juce_audio_utils
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
    gin
    nlohmann_json::nlohmann_json)

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /Wall /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

