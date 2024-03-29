/**
 * @file CharacterSetTests.cpp
 * 
 * This module contains unit Tests of the Uri::CharacterSet class 
 * 
 * © 2024 by Hatem Nabli
*/

#include <gtest/gtest.h>
#include <stddef.h>
#include <src/CharacterSet.hpp>

TEST(CharacterSetTests, DefaultConstructor_Test) {
    Uri::CharacterSet charSet;
    size_t index = 0;
    for (char c = 0; c < 0x7F; ++c) {
        ASSERT_FALSE(charSet.Contains(c)) << index;
        ++index;
    }
}

TEST(CharacterSetTests, SingleCharacterConstructor_Test) {
    Uri::CharacterSet charSet('Y');
    size_t index = 0;
    for (char c = 0; c < 0x7F; ++c) {
        if (c == 'Y') {
            ASSERT_TRUE(charSet.Contains(c)) << index;
        } else {
            ASSERT_FALSE(charSet.Contains(c)) << index;
        }
        ++index;
    }
}

TEST(CharacterSetTests, RangeConstructor_Test) {
    Uri::CharacterSet charSet('A', 'D');
    size_t index = 0;
    for (char c = 0; c < 0x7F; ++c ) {
        if ((c >= 'A') && (c <= 'D')) {
            ASSERT_TRUE(charSet.Contains(c)) << index;
        } else {
            ASSERT_FALSE(charSet.Contains(c)) << index;
        }
        ++index;
    }
}

TEST(CharacterSetTests, InitializerListConstructor_Test) {
    Uri::CharacterSet charSet1{Uri::CharacterSet('a', 'c'), Uri::CharacterSet('f', 'i')};
    for (char c = 0; c < 0x7F; ++c) {
        if (((c >= 'a') && (c <= 'c')) || ((c >= 'f') && (c <= 'i'))) {
            ASSERT_TRUE(charSet1.Contains(c));
        } else {
            ASSERT_FALSE(charSet1.Contains(c));
        }
    }
    Uri::CharacterSet charSet2{Uri::CharacterSet('a', 'c'), Uri::CharacterSet('f')};
    for (char c = 0; c < 0x7F; ++c) {
        if (((c >= 'a') && (c <= 'c')) || (c == 'f') ) {
            ASSERT_TRUE(charSet2.Contains(c));
        } else {
            ASSERT_FALSE(charSet2.Contains(c));
        }
    }
        Uri::CharacterSet charSet3{Uri::CharacterSet('a', 'c'), Uri::CharacterSet('f'), Uri::CharacterSet('r', 'w')};
    for (char c = 0; c < 0x7F; ++c) {
        if (((c >= 'a') && (c <= 'c')) || ((c >= 'r') && (c <= 'w')) || (c == 'f') ) {
            ASSERT_TRUE(charSet3.Contains(c));
        } else {
            ASSERT_FALSE(charSet3.Contains(c));
        }
    }
}

TEST(CharacterSetTests, Contains) {

}