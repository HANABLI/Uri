# CMackeLists.txt for Uri
#
# © 2024 by Hatem Nabli


cmake_minimum_required(VERSION 3.8)
set(this Uri)

set(Headers 
    include/Uri/Uri.hpp
    src/PercentEncodedCharacterDecoder.hpp
    src/CharacterSet.hpp
    src/NormalizeCaseInsensitiveString.hpp
)

set(Sources 
    src/Uri.cpp
    src/PercentEncodedCharacterDecoder.cpp
    src/CharacterSet.cpp
    src/NormalizeCaseInsensitiveString.cpp
)

add_library(${this} STATIC ${Sources} ${Headers})
set_target_properties(${this} PROPERTIES 
    FOLDER Libraries
)

target_include_directories(${this} PUBLIC include)

add_subdirectory(test)