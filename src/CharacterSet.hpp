#ifndef URI_IS_CHARACTER_IN_SET
#define URI_IS_CHARACTER_IN_SET

/**
 * @file CharacterSet.hpp
 * 
 * This module declare the Uri::CharacteraSet class.
 * 
 * © 2024 by Hatem Nabli
*/

#include <initializer_list>
#include <memory>
namespace Uri {
    /**
     * This represent à set of characters which can be quired 
     * to look if a character is in the set or not.
     */

    class CharacterSet {
        //Lifecycle management for the class
    public:
        ~CharacterSet();
        CharacterSet(const CharacterSet&);
        CharacterSet(CharacterSet&&);
        CharacterSet& operator=(const CharacterSet&);
        CharacterSet& operator=(CharacterSet&&);
    
    public:
        /**
         * This is the default constructor.
        */
        CharacterSet();

        /**
         * This is the constructer of a character set 
         * including a simple character
         * 
         * @param[in] c
         *      The character of the set
        */
        CharacterSet(char c);
        /**
         * This is th constracter of all the characters between 
         * the given "first" and "last" characters, inclusive.
         * 
         * @param[in] first
         *      The first character of the set.
         * 
         * @param[in] last
         *      The last character of the set.
        */
        CharacterSet(char first, char last);

        /**
         * This is th constracter of all the characters in several
         * charachter sets.
         * 
         * @param[in] characterSets
         *      The list of all character sets.
         * 
         * 
        */
        CharacterSet(std::initializer_list<const CharacterSet > characterSets);

        /**
         * This method check if a given character is in the set
         * 
         * 
         * @param[in] c
         *      This is the character to check
         * 
         * @return
         *      return an indication of whether or not the character is on the set.
        */

        bool Contains(char c) const;
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
        std::unique_ptr< struct Impl > impl_;
        
    };

}




#endif /*URI_IS_CHARACTER_IN_SET*/
