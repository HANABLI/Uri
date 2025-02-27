#ifndef URI_PERCENT_ENCODED_CHARACTER_DECODER_HPP
#define URI_PERCENT_ENCODED_CHARACTER_DECODER_HPP

/**
 * @file PercentEncodedCharacterDecoder.hpp
 *
 * This modeule declare the Uri::PercentEncodedCharacterDecoder class.
 *
 * © 2024 by Hatem Nabli
 */
#include "CharacterSet.hpp"
#include <stddef.h>
#include <memory>

namespace Uri
{
    /**
     * This class take in a percent-encoded character,
     * decode it, and detect encoding essue.
     */
    class PercentEncodedCharacterDecoder
    {
        // Lifecycle management
    public:
        ~PercentEncodedCharacterDecoder();
        PercentEncodedCharacterDecoder(const PercentEncodedCharacterDecoder&) =
            delete;  // Copy Constructor that creates a new object by making a copy of an existing
                     // object.
        // It ensures that a deep copy is performed if the object contains dynamically allocated
        // resources
        PercentEncodedCharacterDecoder(
            PercentEncodedCharacterDecoder&&);  // Move Constructor that transfers resources from an
                                                // expiring object to a newly constructed object.
        PercentEncodedCharacterDecoder& operator=(const PercentEncodedCharacterDecoder&) =
            delete;  // Copy Assignment Operation That assigns the values of one object to another
                     // object using the assignment operator (=)
        PercentEncodedCharacterDecoder& operator=(
            PercentEncodedCharacterDecoder&&);  // Move Assignment Operator: Amove assignment
                                                // operator efficiently transfers resources from one
                                                // object to another. Methods
    public:
        PercentEncodedCharacterDecoder();
        /**
         * This methode inputs the next encoded character.
         *
         * @param[in] c
         *      This is the next encoded character to give to the decoded.
         * @return
         *      returns an indication of whether or not the encoded character
         *      was accepted.
         */
        bool NextEncodedCharacter(char c);
        /**
         * This method checks if the decoder is done
         * and has decoded the encoded character.
         *
         * @return
         *      returns an indication of whether or not the decoder is done
         *      and has decoded the encoded character.
         */
        bool Done() const;
        /**
         * This method returns the decoded charachter, one is done.
         *
         * @return
         *      returns the decoded charecter.
         */
        char GetDecodedCharacter() const;
        //

        // Properties
    private:
        /**
         * This is the type of structure that contains the private
         * properties of the instance. It is defined in the implementation
         * and declared here to ensure that iwt is scoped inside the class.
         */
        struct Impl;
        /**
         * This contains the private properties of the instance.
         */
        std::unique_ptr<struct Impl> impl_;
    };

}  // namespace Uri

#endif /*URI_PERCENT_ENCODED_CHARACTER_DECODER_HPP*/