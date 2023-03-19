# List the sources files to compile
target_sources(${BaseTargetName} PRIVATE
		processor/MidiProcessor.cpp
		processor/PluginProcessor.cpp
		params/ArpeggiatorParams.cpp
		params/IntervalParam.cpp
		params/MidiParams.cpp
		params/NoteParam.cpp
		params/NoteParams.cpp
		presets/PresetManager.cpp
		gui/PluginEditor.cpp
        gui/Configuration.hpp
		gui/lookandfeel/BaseLookAndFeel.cpp
		gui/widgets/CustomSlider.cpp
		gui/widgets/CustomToggleButton.cpp
		gui/widgets/NoteKey.cpp
		gui/widgets/TextSwitch.cpp
		gui/panels/MainPanel.cpp
		gui/panels/MidiPanel.cpp
		gui/panels/ArpPanel.cpp
		gui/panels/IntervalsPanel.cpp
		gui/panels/KeysPanel.cpp
		gui/panels/PresetsPanel.cpp)

# Build the binary data
set (DataTarget "${BaseTargetName}-Data")
juce_add_binary_data(${DataTarget} SOURCES
		${CMAKE_CURRENT_LIST_DIR}/../data/fonts/iosevka-regular.ttf
		${CMAKE_CURRENT_LIST_DIR}/../data/img/save.png)

target_link_libraries(${BaseTargetName} PRIVATE ${DataTarget})

# Set the base for includes to avoid having to use relative paths
target_include_directories(${BaseTargetName} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})