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

FILE(GLOB CppSources *.cpp)
target_sources(${BaseTargetName} PRIVATE
        ${CppSources})
		
# add_images_from_directory(${BaseTargetName} ../Resources)

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