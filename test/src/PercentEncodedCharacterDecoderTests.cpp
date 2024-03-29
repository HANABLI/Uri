/**
 * @file PercentEncodedCharacterDecoderTests.cpp
 * 
 * This module contains unit Tests of the Uri::PercentEncodedCharacterDecoder class 
 * 
 * © 2024 by Hatem Nabli
*/
#include <gtest/gtest.h>
#include <src/PercentEncodedCharacterDecoder.hpp>

TEST(PercentEncodedCharacterDecoderTests, GoodSequences) {
    Uri::PercentEncodedCharacterDecoder pec;
    ASSERT_FALSE(pec.Done());
    ASSERT_TRUE(pec.NextEncodedCharacter('4'));
    ASSERT_FALSE(pec.Done());
    ASSERT_TRUE(pec.NextEncodedCharacter('1'));
    ASSERT_TRUE(pec.Done());
    ASSERT_EQ('A', pec.GetDecodedCharacter());
}