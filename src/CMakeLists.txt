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

target_sources(${BaseTargetName} PRIVATE
		PluginProcessor.cpp
		PluginEditor.cpp
		MidiProcessor.cpp
		PresetBrowser/PresetManager.cpp
		Params/ArpeggiatorParams.cpp
		Params/IntervalParam.cpp
		Params/MidiParams.cpp
		Params/NoteParam.cpp
		Params/NoteParams.cpp
		GUI/BaseLookAndFeel.h
		GUI/BaseLookAndFeel.cpp
		GUI/BarSliderLookAndFeel.h
		GUI/Helpers.h
		GUI/MainPanel.cpp
        GUI/SettingsPanel.cpp
		GUI/IntervalsPanel.cpp
		GUI/KeysPanel.cpp
		GUI/NoteKey.cpp
		GUI/NoteMappingToggle.cpp
		GUI/PresetsPanel.cpp)
		
add_images_from_directory(${BaseTargetName} ../data/img)
