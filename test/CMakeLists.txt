# CMakeLists.txt for UriTests
#
# c 2024 by Hatem Nabli

cmake_minimum_required(VERSION 3.8)
set(This UriTests)

set(Sources 
    src/UriTests.cpp
)

add_executable(${this} ${Sources})
set_target_properties(${this} PROPERTIES FOLDER Tests)

target_link_libraries(${this} PUBLIC gtest_main Uri)

add_test(
    NAME ${this} 
    COMMAND ${this}
)