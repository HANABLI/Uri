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
    ASSERT_TRUE(uri.ParseFromString("http://www.example.com/foo/bar"));
    ASSERT_EQ("http", uri.GetScheme());
    ASSERT_EQ("www.example.com", uri.GetHost());
    ASSERT_EQ(
        (std::vector<std::string>{
        "",
        "foo",
        "bar",
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
