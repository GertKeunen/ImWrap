cmake_minimum_required(VERSION 3.22)

project(ImWrap VERSION 0.1.0)

set(IMWRAP_JUCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/JUCE" CACHE PATH "Path to JUCE")

add_subdirectory(${IMWRAP_JUCE_PATH} JUCE)

juce_add_plugin(ImWrap
    COMPANY_NAME "KeunenAudio"
    PLUGIN_MANUFACTURER_CODE Knen
    PLUGIN_CODE Imwr
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    COPY_PLUGIN_AFTER_BUILD FALSE
    FORMATS VST3
    PRODUCT_NAME "ImWrap"
)

target_sources(ImWrap PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
)

target_link_libraries(ImWrap PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_processors
)

target_compile_definitions(ImWrap PRIVATE
    JUCE_IGNORE_VST3_MISMATCHED_PARAMETER_ID_WARNING=1
    JUCE_VST3_EMULATE_MIDI_CC_WITH_PARAMETERS=0
)