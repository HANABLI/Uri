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

    const auto HEX = Uri::CharacterSet('A', 'F');
}

namespace Uri{

    struct PercentEncodedCharacterDecoder::Impl
    {
                
        /**
         * This is the decoded character
         */
        char decodedCharacter ;

        /**
         * this the corrent state of the decoder's state machine.
         * 
         *
        */
        size_t decoderState = 0;
    };
    
    PercentEncodedCharacterDecoder::~PercentEncodedCharacterDecoder() = default;
    PercentEncodedCharacterDecoder::PercentEncodedCharacterDecoder(PercentEncodedCharacterDecoder&&) = default;
    PercentEncodedCharacterDecoder& PercentEncodedCharacterDecoder::operator=(PercentEncodedCharacterDecoder&&) = default;
    PercentEncodedCharacterDecoder::PercentEncodedCharacterDecoder() 
    : impl_(new Impl) {

    }

    bool PercentEncodedCharacterDecoder::NextEncodedCharacter(char c) {
        switch (impl_->decoderState)
        {             
            case 0: {
                impl_->decoderState = 1;
                impl_->decodedCharacter <<= 4;
                if (IsCharacterInSet(c, DIGIT)) {
                    impl_->decodedCharacter += (int)(c - '0');
                } else if (IsCharacterInSet(c, HEX)) {
                    impl_->decodedCharacter += (int)(c - 'A') + 10;
                } else {
                    return false;
                }
            } break;

            case 1: { // %[0-9A-F] ...
                    impl_->decodedCharacter <<= 4;
                    impl_->decoderState = 2;
                if (IsCharacterInSet(c, DIGIT)) {                         
                    impl_->decodedCharacter += (int)(c - '0');                      
                } else if (IsCharacterInSet(c, HEX)) {
                    impl_->decodedCharacter += (int)(c - 'A') + 10;
                } else {
                    return false;
                }
            } break;  

            default: break;
        }  
        return true;         
    };

    bool PercentEncodedCharacterDecoder::Done() const {return impl_->decoderState == 2;}

    char PercentEncodedCharacterDecoder::GetDecodedCharacter() const {return (char)impl_->decodedCharacter;}
}