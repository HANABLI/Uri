/**
 * @file CharacteraSet.cpp
 * 
 * This module containe the implementation of the Uri::CharacteraSet function.
 * 
 * © 2024 by Hatem Nabli
*/
#include "CharacterSet.hpp"
#include <set>
namespace Uri {

    struct CharacterSet::Impl {
        /**
         * This will be the set of ol characters
        */
        std::set<char> charactersInSet;
    };

    CharacterSet::CharacterSet(CharacterSet&&) = default;

    CharacterSet& CharacterSet::operator=(CharacterSet&&) = default;

    CharacterSet::CharacterSet(const CharacterSet& other) 
    : impl_(new Impl(*other.impl_)) {

    }
    CharacterSet& CharacterSet::operator=(const CharacterSet& other) {
        if(this != &other) {
            *impl_ = *other.impl_;
        }
        return *this;
    }

    CharacterSet::~CharacterSet() = default;

    CharacterSet::CharacterSet(): impl_(new Impl)
    {
    }

    CharacterSet::CharacterSet(char c): impl_(new Impl) {
        (void)impl_->charactersInSet.insert(c);
    }

    CharacterSet::CharacterSet(char first, char last): impl_(new Impl) {
        for(char c = first; c < last + 1; ++c) {
            (void)impl_->charactersInSet.insert(c);
        }
    }

    CharacterSet::CharacterSet(std::initializer_list<const CharacterSet> characterSets) : impl_(new Impl) {
        for (auto characterSet = characterSets.begin(); characterSet != characterSets.end(); ++characterSet) {
            (void)impl_->charactersInSet.insert(
                characterSet->impl_->charactersInSet.begin(),
                characterSet->impl_->charactersInSet.end()
            );
        }
    }
        /**
     * This function indicate whether or not a given character
     * is in the given character set.
     *
     * @param[in] c
     *      The character to look for
     * @return
     *      return an indication of whether or not the cahracter was find
     *      in the character set.
     */
    bool CharacterSet::Contains(char c) const {
        return impl_->charactersInSet.find(c) != impl_->charactersInSet.end();
    }

}
