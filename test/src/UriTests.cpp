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

TEST(UriTests, Placeholder_Test) {
    Uri::Uri uri;
    ASSERT_TRUE(true);
}

TEST(UriTests, ParseFromStrigUriNoScheme_Test) {
    Uri::Uri uri;
    ASSERT_TRUE(uri.ParseFromString("library/book"));
    ASSERT_EQ("", uri.GetScheme());
    ASSERT_EQ(
        (std::vector<std::string>{
            "library",
            "book",
        }), uri.GetPath()
    );
}

TEST(UriTests, ParseFromStringUrl_Test) {
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

TEST(UriTests, ParseFromStringPathAllCases_Test) {
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

TEST(UriTests, ParseFromUriStringHasPortNumber_Test) {
    Uri::Uri uri;
    ASSERT_TRUE(uri.ParseFromString("http://www.example.com:8080/library/book"));
    ASSERT_TRUE(uri.HasPort());
    ASSERT_EQ(8080, uri.GetPort());
}

TEST(UriTests, ParseFromUriStringHasNoPortNumber_Test) {
    Uri::Uri uri;
    ASSERT_TRUE(uri.ParseFromString("http://www.example.com/library/book"));
    ASSERT_FALSE(uri.HasPort());
}

TEST(UriTest, ParseFromStringUriWithBadAlphabeticPortNumber_Test) {
    Uri::Uri uri;
    ASSERT_FALSE(uri.ParseFromString("http://www.example.com:blabla/library/book"));
}

TEST(UriTest, ParseFromStringUriWithBadProtNumberStartsNumericEndsAlphabetic_Test) {
    Uri::Uri uri;
    ASSERT_FALSE(uri.ParseFromString("http://www.example.com:8080blabla/library/book"));
}

TEST(UriTest, ParseFromStringUriLargestGoodPortNumber_Test) {
    Uri::Uri uri;
    ASSERT_TRUE(uri.ParseFromString("http://www.example.com:65535/library/book"));
    ASSERT_TRUE(uri.HasPort());
    ASSERT_EQ(65535, uri.GetPort());
}

TEST(URiTest, ParseFromStringUriTooBigPortNumber_Test) {
    Uri::Uri uri;
    ASSERT_FALSE(uri.ParseFromString("http://wwww.example.com:65536/library/book"));
}

TEST(UriTests, ParseFromStringIsRelativeReference_Test) {
    struct TestVector
    {
        std::string uriString;
        bool isRelativeReference;
    };
    const std::vector<TestVector> testVectors {
        {"http://www.example.com/", false},
        {"http://www.example.com", false},
        {"/", true},
        {"book", true},
    };
    size_t index = 0;
    for (const auto& test: testVectors) {
        Uri::Uri uri;
        ASSERT_TRUE(uri.ParseFromString(test.uriString)) << index;
        ASSERT_EQ(test.isRelativeReference, uri.IsRelativeReference()) << index;
        ++index;
    } 
    
}

TEST(UriTests, ParseFromStringIsRelativePath_Test) {
    struct TestVector
    {
        std::string uriString;
        bool hasRelativePath;
    };
    const std::vector<TestVector> testVectors {
        {"http://www.example.com/", false},
        {"http://www.example.com", true},
        {"/", false},
        {"book", true},
        /*
         * This is only a vlid test vector if we confirm that 
         * an empty string is a valid
         * "relative reference" URI with an empty path
         */
        {"", true},
    };
    size_t index = 0;
    for (const auto& test: testVectors) {
        Uri::Uri uri;
        ASSERT_TRUE(uri.ParseFromString(test.uriString)) << index;
        ASSERT_EQ(test.hasRelativePath, uri.HasRelativePath()) << index;
        ++index;
    }   
}

TEST(UriTests, ParseFromStringUriFragments_Test) {
    struct TestVector
    {
        std::string uriString;
        std::string host;
        std::string fragment;
    };
    const std::vector<TestVector> testVectors {
        {"http://www.example.com/", "www.example.com", ""},
        {"http://www.example.com?library", "www.example.com", ""},
        {"http://www.example.com#book", "www.example.com", "book"},
        {"http://www.example.com?library#book", "www.example.com", "book"},
        {"http://www.example.com/University?library#book", "www.example.com", "book"},
    };

    size_t index = 0;
    for (const auto& test: testVectors) {
        Uri::Uri uri;
        ASSERT_TRUE(uri.ParseFromString(test.uriString)) << index;
        ASSERT_EQ(test.host, uri.GetHost()) << index;
        ASSERT_EQ(test.fragment, uri.GetFragment()) << index;
        ++index;
    }        
} 

TEST(UriTests, ParseFromStringUriQuery_Test) {
       struct TestVector
    {
        std::string uriString;
        std::string host;
        std::string query;
        std::string fragment;
    };
    const std::vector<TestVector> testVectors {
        {"http://www.example.com/", "www.example.com", "", ""},
        {"http://www.example.com/?", "www.example.com", "", ""},
        {"http://www.example.com?library", "www.example.com", "library", ""},
        {"http://www.example.com#book", "www.example.com", "", "book"},
        {"http://www.example.com?library#book", "www.example.com", "library", "book"},
        {"http://www.example.com/University?library#book", "www.example.com", "library", "book"},
    };

    size_t index = 0;
    for (const auto& test: testVectors) {
        Uri::Uri uri;
        ASSERT_TRUE(uri.ParseFromString(test.uriString)) << index;
        ASSERT_EQ(test.host, uri.GetHost()) << index;
        ASSERT_EQ(test.query, uri.GetQuery()) <<index;
        ASSERT_EQ(test.fragment, uri.GetFragment()) << index;
        ++index;
    }         
}

TEST(UriTests, ParseFromStringUriUserInfo_Test) {
    struct TestVector {
        std::string uriString;
        struct UserInfo
        {
            std::string name;
            std::string pass;
        } userInfo;       
    };

    const std::vector<TestVector> testVectors {
        {"http://www.example.com/", {"", ""}},
        {"http://hnab@www.example.com", {"hnab", ""}},
        {"http://hnab:password@www.example.com", {"hnab", "password"}},
        {"//www.example.com", {"", ""}},
        {"//hnab@www.example.com/", {"hnab", ""}},
        {"/", {"", ""}},
        {"book", {"", ""}},
    };
    size_t index = 0;
    for (const auto& test: testVectors ) {
        Uri::Uri uri ;
        ASSERT_TRUE(uri.ParseFromString(test.uriString)) << index;
        ASSERT_EQ(test.userInfo.name, uri.GetUserInfo().name) << index;
        ASSERT_EQ(test.userInfo.pass, uri.GetUserInfo().pass) << index;
        ++index;
    }
}

TEST(UriTests, UriTests_ParseStringUriTwiceFirstUserInfoThenWithout_Test) {
    Uri::Uri uri;

    ASSERT_TRUE(uri.ParseFromString("http://joe@www.example.com/foo/bar"));
    ASSERT_TRUE(uri.ParseFromString("/foo/bar"));
    ASSERT_TRUE(uri.GetUserInfo().name.empty());
    
}

TEST(UriTests, UriTests_ParseFromStringSchemeIllegalCharacters_Test) {
    struct TestVector {
        std::string uriString;
    };
    const std::vector< TestVector > testVectors {
        {"://www.example.com/"},
        {"0://www.example.com/"},
        {"+://www.example.com/"},
        {"@://www.example.com/"},
        {".://www.example.com/"},
        {"h@://www.example.com/"},
    };
    size_t index = 0;
    for(const auto& test: testVectors) {
        Uri::Uri uri;
        ASSERT_FALSE(uri.ParseFromString(test.uriString)) <<index;
        ++index;
    }    
}

TEST(UriTests, UriTests_ParseFromStringSchemeBarelyLegal_Test) {
    struct TestVector {
        std::string uriString;
        std::string scheme;
    };
    const std::vector<TestVector> testVectors {
        
        {"h://www.example.com", "h"},
        {"a+://www.example.com", "a+"},
        {"b-://www.example.com", "b-"},
        {"c.://www.example.com", "c."},
        {"aa://www.example.com", "aa"},
        {"h0://www.example.com", "h0"},
    };
    size_t index = 0;
    for(const auto& test: testVectors) {
        Uri::Uri uri;
        ASSERT_TRUE(uri.ParseFromString(test.uriString)) <<index;
        ASSERT_EQ(test.scheme, uri.GetScheme()) <<index;
        ++index;
    }    
}

TEST(UriTests, ParseFromStringUriUserInfoIllegalCharacters) {
        struct TestVector {
        std::string uriString;
    };
    const std::vector< TestVector > testVectors{
        {"http://%X@www.example.com/"},
        {"http://{@www.example.com/"},
    }; 
    size_t index = 0;
    for(const auto& test: testVectors) {
        Uri::Uri uri;
        ASSERT_FALSE(uri.ParseFromString(test.uriString)) << index;
        ++index;
    }
}

TEST(UriTests, UriTests_ParseFromStringUserInfoBarelyLegal_Test) {
    struct TestVector {
        std::string uriString;
        std::string user;
        std::string pass;
    };
    const std::vector<TestVector> testVectors {
        
        {"//%41@www.example.com", "A",""},
        {"//@www.example.com", "",""},
        {"//!@www.example.com", "!",""},
        {"//'@www.example.com", "'",""},
        {"//(@www.example.com", "(",""},
        {"//;@www.example.com", ";",""},
        {"http://:@www.example.com", "",""},
    };
    size_t index = 0;
    for(const auto& test: testVectors) {
        Uri::Uri uri;
        ASSERT_TRUE(uri.ParseFromString(test.uriString)) <<index;
        ASSERT_EQ(test.user, uri.GetUserInfo().name) <<index;
        ASSERT_EQ(test.pass, uri.GetUserInfo().pass) <<index;
        ++index;
    }    
}

TEST(UriTests, ParseFromStringUriHostNameIllegalCharacters) {
        struct TestVector {
        std::string uriString;
    };
    const std::vector< TestVector > testVectors{
        {"http://%X@www.example.com/"},
        {"http://{@www.example.com/"},
        {"//[vX.:]/"},
    }; 
    size_t index = 0;
    for(const auto& test: testVectors) {
        Uri::Uri uri;
        ASSERT_FALSE(uri.ParseFromString(test.uriString)) << index;
        ++index;
    }
}

TEST(UriTests, UriTests_ParseFromStringUriHostNameBarelyLegal_Test) {
    struct TestVector {
        std::string uriString;
        std::string hostName;
    };
    const std::vector<TestVector> testVectors {
        
        {"//%41/", "a"},
        {"///", ""},
        {"//!/", "!"},
        {"//'/", "'"},
        {"//(/", "("},
        {"//;/", ";"},
        {"//1.2.3.4/","1.2.3.4"},
        {"//[v7.:]/", "[v7.:]"},
        {"//[v7.cD]/", "[v7.cD]"},
    };
    size_t index = 0;
    for(const auto& test: testVectors) {
        Uri::Uri uri;
        ASSERT_TRUE(uri.ParseFromString(test.uriString)) <<index;
        ASSERT_EQ(test.hostName, uri.GetHost()) << index;
        ++index;
    }    
}

TEST(UriTests, UriTests_ParseFromStringUriWithInterpretColonOnAuthorirtyAsShemeDel_Test) {
struct TestVector {
        std::string uriString;
    };
    const std::vector<TestVector> testVectors {
        {"http://foo:balbla@www.example.com/"},
        {"//[v7.:]/"},
        {"/:/book"},
    };
    size_t index = 0;
    for(const auto& test: testVectors) {
        Uri::Uri uri;
        ASSERT_TRUE(uri.ParseFromString(test.uriString)) <<index;
        ++index;
    }    
}

TEST(UriTests, ParseFromStringUriPathIllegalCharacters) {
    const std::vector< std::string > testVectors{
        {"http://www.example.com/library[book"},
        {"http://www.example.com/]book"},
        {"http://www.example.com/library]"},
        {"http://www.example.com/["},
        {"http://www.example.com/library/book]"},
        {"http://www.example.com/library/["},
        {"http://www.example.com/library]/book"},
        {"http://www.example.com/[/book]"},
        {"http://www.example.com/library]/"},
        {"http://www.example.com/[/"},
        {"/library[book"},
        {"/]book"},
        {"/library]"},
        {"/["},
        {"/library/book]"},
        {"/library/["},
        {"/library]/book"},
        {"/[/book]"},
        {"/library]/"},
        {"/[/"},
    };
    size_t index = 0;
    for(const auto& test: testVectors) {
        Uri::Uri uri;
        ASSERT_FALSE(uri.ParseFromString(test)) <<index;
        ++index;
    }
}

TEST(UriTests, ParseFromStringUriPathBarelyLegalCharacters) {
    struct TestVector {
        std::string uriString;
        std::vector<std::string> path;
    };

    std::vector< TestVector > testVectors{
        {"/:/book", {"", ":", "book"}},
        {"hnab@/book", {"hnab@", "book"}},
        {"hello!", {"hello!"}},
        {"urn:hello,%20w%6Frld", {"hello, world"}},
        {"//example.com/library/(book)/", {"", "library", "(book)", ""}},
    };
    size_t index = 0;
    for (const auto& test: testVectors) {
        Uri::Uri uri;
        ASSERT_TRUE(uri.ParseFromString(test.uriString)) <<index;
        ASSERT_EQ(test.path, uri.GetPath()) <<index;
        ++index;
    } 
}

TEST(UriTests, ParseFromStringUriQueryIllegalCharacters) {
    const std::vector< std::string > testVectors{
        {"http://www.example.com/?library[book"},
        {"http://www.example.com/?]book"},
        {"http://www.example.com/?library]"},
        {"http://www.example.com/?["},
        {"http://www.example.com/?library/book]"},
        {"http://www.example.com/?library/["},
        {"http://www.example.com/?library]/book"},
        {"http://www.example.com/?[/book]"},
        {"http://www.example.com/?library]/"},
        {"http://www.example.com/?[/"},
        {"?library[book"},
        {"?]book"},
        {"?library]"},
        {"?["},
        {"?library/book]"},
        {"?library/["},
        {"?library]/book"},
        {"?[/book]"},
        {"?library]/"},
        {"?[/"},
    };
    size_t index = 0;
    for(const auto& test: testVectors) {
        Uri::Uri uri;
        ASSERT_FALSE(uri.ParseFromString(test)) <<index;
        ++index;
    }
}

TEST(UriTests, ParseFromStringUriQueryBarelyLegalCharacters) {
    struct TestVector {
        std::string uriString;
        std::string query;
    };

    std::vector< TestVector > testVectors{
        {"/?:/book", ":/book"},
        {"?hnab@/book", "hnab@/book"},
        {"?hello!", "hello!"},
        {"urn:?hello,%20w%6Frld", "hello, world"},
        {"//example.com/library?(book)/", "(book)/"},
        {"http://www.example.com/?library?book", "library?book"},
    };
    size_t index = 0;
    for (const auto& test: testVectors) {
        Uri::Uri uri;
        ASSERT_TRUE(uri.ParseFromString(test.uriString)) <<index;
        ASSERT_EQ(test.query, uri.GetQuery()) <<index;
        ++index;
    } 
}

TEST(UriTests, ParseFromStringUriFragmentIllegalCharacters) {
    const std::vector< std::string > testVectors{
        {"http://www.example.com/#library[book"},
        {"http://www.example.com/#]book"},
        {"http://www.example.com/#library]"},
        {"http://www.example.com/#["},
        {"http://www.example.com/#library/book]"},
        {"http://www.example.com/#library/["},
        {"http://www.example.com/#library]/book"},
        {"http://www.example.com/#[/book]"},
        {"http://www.example.com/#library]/"},
        {"http://www.example.com/#[/"},
        {"#library[book"},
        {"#]book"},
        {"#library]"},
        {"#["},
        {"#library/book]"},
        {"#library/["},
        {"#library]/book"},
        {"#[/book]"},
        {"#library]/"},
        {"#[/"},
    };
    size_t index = 0;
    for(const auto& test: testVectors) {
        Uri::Uri uri;
        ASSERT_FALSE(uri.ParseFromString(test)) <<index;
        ++index;
    }
}

TEST(UriTests, ParseFromStringUriFragmentBarelyLegalCharacters) {
    struct TestVector {
        std::string uriString;
        std::string fragment;
    };

    std::vector< TestVector > testVectors{
        {"/#:/book", ":/book"},
        {"#hnab@/book", "hnab@/book"},
        {"#hello!", "hello!"},
        {"urn:#hello,%20w%6Frld", "hello, world"},
        {"//example.com/library#(book)/", "(book)/"},
        {"http://www.example.com/#library?book", "library?book"},
    };
    size_t index = 0;
    for (const auto& test: testVectors) { 
        Uri::Uri uri;
        ASSERT_TRUE(uri.ParseFromString(test.uriString)) <<index;
        ASSERT_EQ(test.fragment, uri.GetFragment()) <<index;
        ++index;
    } 
}

TEST(UriTests, UriTests_ParseFromStringUriSchemeWhithMixedcases_Test) {
     struct TestVector {
        std::string uriString;
    };
    const std::vector<TestVector> testVectors {
        
        {"http://www.example.com/"},
        {"hTtp://www.example.com/"},
        {"Http://www.example.com/"},
        {"HTTP://www.example.com/"},
        {"HttP://www.example.com/"},
        {"httP://www.example.com/"},
    };
    size_t index = 0;
    for(const auto& test: testVectors) {
        Uri::Uri uri;
        ASSERT_TRUE(uri.ParseFromString(test.uriString)) <<index;
        ASSERT_EQ("http", uri.GetScheme()) <<"-!---- Failed with test case : "<<index<<" ----!-";
        ++index;
    }    
}

TEST(UriTests, UriTests_ParseFromStringUriHostWhithMixedcases_Test) {
     struct TestVector {
        std::string uriString;
    };
    const std::vector<TestVector> testVectors {
        
        {"http://www.example.com/"},
        {"hTtp://www.EXAMPLE.com/"},
        {"Http://WWW.example.com/"},
        {"HTTP://www.eXAmple.COM/"},
        {"HttP://WWW.example.COM/"},
        {"httP://wWw.eXample.Com/"},
    };
    size_t index = 0;
    for(const auto& test: testVectors) {
        Uri::Uri uri;
        ASSERT_TRUE(uri.ParseFromString(test.uriString)) <<index;
        ASSERT_EQ("www.example.com", uri.GetHost()) <<"-!---- Failed with test case : "<<index<<" ----!-";
        ++index;
    }    
}

TEST(UriTests, NormalizePath_Test) {
    struct TestVector {
        std::string uriString;
        std::vector< std::string > normalizedPathSegments;
    };
    const std::vector<TestVector> TestVectors {
        {"/a/b/c/./../../g", {"", "a", "g"}},
        {"mid/content=5/../6", {"mid", "6"}},
    };
    size_t index = 0;
    for (const auto& test: TestVectors) {
        Uri::Uri uri;
        ASSERT_TRUE(uri.ParseFromString(test.uriString)) << index;
        uri.NormalizePath();
        ASSERT_EQ(test.normalizedPathSegments, uri.GetPath()) << index;
        ++index;
    }
}

TEST(UriTests, NormalizeAndCompareEquivalentUris_Test) {
    Uri::Uri uri1, uri2;
    ASSERT_TRUE(uri1.ParseFromString("example://a/b/c/%7Bfoo%7D"));
    ASSERT_TRUE(uri2.ParseFromString("eXAMPLE://a/./b/../b/%63/%7bfoo%7d"));
    ASSERT_NE(uri1, uri2);
    uri2.NormalizePath();
    ASSERT_EQ(uri1, uri2);
}