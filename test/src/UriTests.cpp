/**
 * @file UriTests.cpp
 * 
 * This module contains unit Tests of the Uri::Uri class 
 * 
 * © 2024 by Hatem Nabli
*/
#include <stdio.h>
#include <gtest/gtest.h>
#include <Uri/Uri.hpp>

TEST(UriTests, Placeholder) {
    Uri::Uri uri;
    ASSERT_TRUE(true);
}

TEST(UriTests, ParseFromStringUrl) {
    Uri::Uri uri;
    ASSERT_TRUE(uri.ParseFromString("http://www.example.com/library/book"));
    ASSERT_EQ("http", uri.GetScheme());
    ASSERT_EQ("www.example.com", uri.GetHost());
    ASSERT_EQ(
        (std::vector<std::string>{
        "",
        "library",
        "book",
    }), uri.GetPath());
    
}

TEST(UriTests, ParseFromStringPathAllCases) {
    struct TestCasesVector
    {
        std::string pathIn;
        std::vector<std::string> pathOut;
    };
    const std::vector<TestCasesVector> testsVector {
        {"", {}},
        {"/", {""}},
        {"/do", {"", "do"}},
        {"do/", {"do", ""}},
    };
    size_t index = 0;
    for (const auto& test : testsVector) {
        Uri::Uri uri;
        ASSERT_TRUE(uri.ParseFromString(test.pathIn)) << index;
        ASSERT_EQ(test.pathOut, uri.GetPath()) << index;
        index++;
    }
}

TEST(UriTests, ParseFromUriStringHasPortNumber) {
    Uri::Uri uri;
    ASSERT_TRUE(uri.ParseFromString("http://www.example.com:8080/library/book"));
    ASSERT_TRUE(uri.HasPort());
    ASSERT_EQ(8080, uri.GetPort());
}

TEST(UriTests, ParseFromUriStringHasNoPortNumber) {
    Uri::Uri uri;
    ASSERT_TRUE(uri.ParseFromString("http://www.example.com/library/book"));
    ASSERT_FALSE(uri.HasPort());
}

TEST(UriTest, ParseFromStringUriWithBadAlphabeticPortNumber) {
    Uri::Uri uri;
    ASSERT_FALSE(uri.ParseFromString("http://www.example.com:blabla/library/book"));
}

TEST(UriTest, ParseFromStringUriWithBadProtNumberStartsNumericEndsAlphabetic) {
    Uri::Uri uri;
    ASSERT_FALSE(uri.ParseFromString("http://www.example.com:8080blabla/library/book"));
}

TEST(UriTest, ParseFromStringUriLargestGoodPortNumber) {
    Uri::Uri uri;
    ASSERT_TRUE(uri.ParseFromString("http://www.example.com:65535/library/book"));
    ASSERT_TRUE(uri.HasPort());
    ASSERT_EQ(65535, uri.GetPort());
}

TEST(URiTest, ParseFromStringUriTooBigPortNumber) {
    Uri::Uri uri;
    ASSERT_FALSE(uri.ParseFromString("http://wwww.example.com:65536/library/book"));
}

