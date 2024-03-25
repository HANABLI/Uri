#ifndef URI_NORMALIZE_CASE_INSESITIVE_STRING_HPP
#define URI_NORMALIZE_CASE_INSESITIVE_STRING_HPP

/**
 * @file NormalizeCaseInsesitiveString.hpp
 * 
 * This modeule declare the Uri::NormalizeCaseInsesitiveString function.
 * 
 * © 2024 by Hatem Nabli
*/
#include <string> 
#include <ctype.h>

namespace Uri {
    /**
     * This fucntion takes a string chaine and convert all upper-case characters
     * with lower-case equivalents.
     * @param[in] inputString
     *      this is the string chaine to be normalized.
     * @return
     *      returns the normalized string with only lower-case characters.
    */
   std::string NormalizeCaseInsensitiveString(const std::string& inputString);
}
#endif /*NORMALIZE_CASE_INSESITIVE_STRING_HPP*/