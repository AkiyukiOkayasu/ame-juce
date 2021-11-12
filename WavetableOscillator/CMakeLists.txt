# JUCE
set(pluginname WavetableOscillator)
juce_add_plugin(${pluginname}
    VERSION ${PROJECT_VERSION}
    # ICON_BIG ""   # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
    # ICON_SMALL "${CMAKE_SOURCE_DIR}/assets/icon.png " 
    COMPANY_NAME "Akiyuki Okayasu"                    # Specify the name of the plugin's author
    COMPANY_COPYRIGHT "Akiyuki Okayasu"
    # COMPANY_WEBSITE ""
    COMPANY_EMAIL "akiyuki.okayasu@gmail.com"
    IS_SYNTH TRUE                       # Is this a synth or an effect?
    NEEDS_MIDI_INPUT TRUE               # Does the plugin need midi input?
    NEEDS_MIDI_OUTPUT FALSE              # Does the plugin need midi output?
    IS_MIDI_EFFECT FALSE                 # Is this plugin a MIDI effect?
    # EDITOR_WANTS_KEYBOARD_FOCUS TRUE/FALSE    # Does the editor need keyboard focus?
    COPY_PLUGIN_AFTER_BUILD TRUE        # ビルドするたびにプラグインをインストールするか？
    PLUGIN_MANUFACTURER_CODE AkiyukiOkayasu               # 最低一文字大文字でプラグインベンダーのIDを指定
    PLUGIN_CODE ${pluginname}                            # 最低一文字大文字でプラグインのユニークIDを指定
    FORMATS 
    # ここでビルドしたいプラグインの種類を指定する
            Standalone 
            AU
            # AUv3 
            VST3 
            # Unity 
            # AAX           
    VST3_CATEGORIES "Instrument" #複数(3つまで)列挙するとホスト上でフォルダにネストされて表示される
    AU_MAIN_TYPE "kAudioUnitType_MusicDevice"
    # AU_SANDBOX_SAFE TRUE
    # AAX_CATEGORY ""
    # HARDENED_RUNTIME_ENABLED # macOS app settings
    # HARDENED_RUNTIME_OPTIONS
    # APP_SANDBOX_ENABLED TRUE
    # APP_SANDBOX_INHERIT
    # DESCRIPTION "" #設定するとProtoolsのプラグイン名がこのDescriptionになってしまう（バグ？）
    MICROPHONE_PERMISSION_ENABLED FALSE
    # MICROPHONE_PERMISSION_TEXT "This applicaion requires a permission to use an audio input device of your computer. By Default, Built-In microphone will be used."
    PRODUCT_NAME ${pluginname}        # The name of the final 
)

add_subdirectory(src)

juce_generate_juce_header(${pluginname})

target_compile_options(${pluginname} PUBLIC -O2 -Wall)
target_compile_definitions(${pluginname} PUBLIC JUCE_VST3_CAN_REPLACE_VST2=0)
target_compile_features(${pluginname} PUBLIC cxx_std_17)
target_link_libraries(${pluginname} PRIVATE
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_plugin_client
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core    
    juce::juce_data_structures
    juce::juce_dsp
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
    ame
)