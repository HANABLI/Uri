# CMackeLists.txt for Uri
#
# © 2024 by Hatem Nabli


cmake_minimum_required(VERSION 3.20)
set(this Uri)

set(Headers
    include/Uri/Uri.hpp
    src/PercentEncodedCharacterDecoder.hpp
    src/CharacterSet.hpp
)

set(Sources
    src/Uri.cpp
    src/PercentEncodedCharacterDecoder.cpp
    src/CharacterSet.cpp
)

add_library(${this} STATIC ${Sources} ${Headers})
set_target_properties(${this} PROPERTIES
    FOLDER Libraries
)

target_link_libraries(${this} PUBLIC
    StringUtils
)

target_include_directories(${this} PUBLIC include)

add_subdirectory(test)