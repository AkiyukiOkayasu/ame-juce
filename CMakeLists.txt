cmake_minimum_required(VERSION 3.15)
project(ame_juce VERSION 0.0.1)

# include(CTest)
include(FetchContent)
FetchContent_Declare(ame GIT_REPOSITORY https://github.com/AkiyukiOkayasu/ame GIT_TAG v0.3.1)
FetchContent_MakeAvailable(ame)

# JUCE
add_subdirectory(JUCE)
add_subdirectory(WhiteNoise)
add_subdirectory(WavetableOscillator)