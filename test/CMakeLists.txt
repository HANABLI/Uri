# CMakeLists.txt for UriTests
#
# © 2024 by Hatem Nabli

cmake_minimum_required(VERSION 3.8)
set(this UriTests)

set(Sources 
    src/UriTests.cpp
    src/CharacterSetTests.cpp
    src/NormalizeCaseInsensitiveStringTests.cpp
    src/PercentEncodedCharacterDecoderTests.cpp
)

add_executable(${this} ${Sources})
set_target_properties(${this} PROPERTIES 
    FOLDER Tests
)

target_include_directories(${this} PRIVATE ..)

target_link_libraries(${this} PUBLIC 
    gtest_main 
    Uri
)

add_test(
    NAME ${this} 
    COMMAND ${this}
)