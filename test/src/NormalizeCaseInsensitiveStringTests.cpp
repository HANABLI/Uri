/**
 * @file NormalizeCaseInsensitiveStringTests.cpp
 * 
 * This module contains unit Tests of the Uri::NormalizeCaseInsensitiveString class 
 * 
 * © 2024 by Hatem Nabli
*/
#include <gtest/gtest.h>
#include <src/NormalizeCaseInsensitiveString.hpp>

TEST(NormalizeCaseInsensitiveStringTests, SomeCases) {
    ASSERT_EQ("example", Uri::NormalizeCaseInsensitiveString("example"));
    ASSERT_EQ("example", Uri::NormalizeCaseInsensitiveString("eXAMple"));
    ASSERT_EQ("example", Uri::NormalizeCaseInsensitiveString("EXAMPLE"));
    ASSERT_EQ("example", Uri::NormalizeCaseInsensitiveString("exAMPLE"));
    ASSERT_EQ("example", Uri::NormalizeCaseInsensitiveString("EXAmple"));
    ASSERT_EQ("example", Uri::NormalizeCaseInsensitiveString("eXampLe"));
    ASSERT_EQ("example", Uri::NormalizeCaseInsensitiveString("ExamplE"));
}