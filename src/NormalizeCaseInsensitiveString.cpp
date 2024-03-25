/**
 * @file NormalizeCaseInsesitiveString.cpp
 * 
 * This modeule containe the implementation of Uri::NormalizeCaseInsesitiveString function.
 * 
 * © 2024 by Hatem Nabli
*/
#include "NormalizeCaseInsensitiveString.hpp"

namespace Uri {
    std::string NormalizeCaseInsensitiveString(const std::string& inputString) {
         std::string outputString;
         for(char c : inputString) {
            outputString.push_back(tolower(c));
         }
         return outputString;
    }
}
