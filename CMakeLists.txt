cmake_minimum_required(VERSION 3.15)
project(ame_juce VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(AKIYUKI_PLUGIN_MANIFACTURE_CODE AKYK)

# include(CTest)
include(FetchContent)
FetchContent_Declare(ame GIT_REPOSITORY https://github.com/AkiyukiOkayasu/ame GIT_TAG v1.0.3-rc3)
FetchContent_MakeAvailable(ame)

# JUCE
add_subdirectory(JUCE)
add_subdirectory(WhiteNoise)
add_subdirectory(WavetableOscillator)