/**
 * @file PercentEncodedCharacterDecoder.cpp
 * 
 * This modeule containe the implementation of Uri::PercentEncodedCharacterDecoder class.
 * 
 * © 2024 by Hatem Nabli
*/

#include "PercentEncodedCharacterDecoder.hpp"

namespace {
    /**
     * This is the character set containing the numerical characters
     * from the ASCII set.
    */
    const auto DIGIT = Uri::CharacterSet('0', '9');

    const auto HEX = Uri::CharacterSet{
        Uri::CharacterSet('A', 'F'),
        Uri::CharacterSet('a', 'f')
    };
}

namespace Uri{

    struct PercentEncodedCharacterDecoder::Impl
    {
        //Properties        
        /**
         * This is the decoded character
         * 
         */
        char decodedCharacter ;

        /**
         * This is the needed number of digits to shift in.
         *
        */
        size_t digitsLeft = 2;

        //Methods
        /**
         * This method shifts in the hex character as part of building
         * the decoded character.
         * 
         * @param[c] 
         *      The hex digit param to shift into the decoded one
         * @return
         *      return an indication of whether or not the hex digit character is 
         *      shifted. 
        */
        bool ShiftInHexDigit(char c) {
            decodedCharacter <<= 4;
            if (DIGIT.Contains(c)) {
                decodedCharacter += (int)(c - '0');
            } else if (HEX.Contains(c)) {
                c = (char)towupper(c);
                decodedCharacter += (int)(c - 'A') + 10;
            } else {
                return false;
            }
            return true;
        }
    };
    
    PercentEncodedCharacterDecoder::~PercentEncodedCharacterDecoder() = default;
    PercentEncodedCharacterDecoder::PercentEncodedCharacterDecoder(PercentEncodedCharacterDecoder&&) = default;
    PercentEncodedCharacterDecoder& PercentEncodedCharacterDecoder::operator=(PercentEncodedCharacterDecoder&&) = default;
    PercentEncodedCharacterDecoder::PercentEncodedCharacterDecoder() 
    : impl_(new Impl) {

    }

    bool PercentEncodedCharacterDecoder::NextEncodedCharacter(char c) {
        if(!impl_->ShiftInHexDigit(c)) {
            return false;
        }
        --impl_->digitsLeft;
        return true;         
    }

    bool PercentEncodedCharacterDecoder::Done() const {return impl_->digitsLeft == 0;}

    char PercentEncodedCharacterDecoder::GetDecodedCharacter() const {return (char)impl_->decodedCharacter;}
}
