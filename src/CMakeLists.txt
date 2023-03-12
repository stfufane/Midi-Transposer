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
		gui/widgets/IndexedToggleButton.cpp
		gui/widgets/NoteKey.cpp
		gui/widgets/NoteMappingToggle.cpp
		gui/panels/MainPanel.cpp
		gui/panels/MidiPanel.cpp
		gui/panels/ArpPanel.cpp
		gui/panels/IntervalsPanel.cpp
		gui/panels/KeysPanel.cpp
		gui/panels/PresetsPanel.cpp)

# Build the binary data from the images and fonts folder
add_binary_data_from_directory(${BaseTargetName} ../data/img ../data/fonts)

# Set the base for includes to avoid having to use relative paths
target_include_directories(${BaseTargetName} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})