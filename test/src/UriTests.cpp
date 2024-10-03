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
        {"http://www.example.com", false},
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
        {"//[v7.:]/", "v7.:"},
        {"//[v7.cD]/", "v7.cD"},
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
        {"http://example.com/a/../b", {"", "b"}},
        {"http://example.com/../b", {"", "b"}},
        {"http://example.com/a/../../b", {"", "b"}},
        {"./a/b", {"a", "b"}},
        {"..", {}},
        {"a/b/..", {"a", ""}},
        {"a/b/.", {"a", "b", ""}},
        {"a/b/./c", {"a", "b", "c"}},
        {"a/b/./c/", {"a", "b", "c", ""}},
        {"/a/b/..", {"", "a", ""}},
        {"/a/b/.", {"", "a", "b", ""}},
        {"/a/b/./c", {"", "a", "b", "c"}},
        {"/a/b/./c/", {"", "a", "b", "c", ""}},
        {"./a/b/..", {"a", ""}},
        {"./a/b/.", {"a", "b", ""}},
        {"./a/b/./c", {"a", "b", "c"}},
        {"./a/b/./c/", {"a", "b", "c", ""}},
        {"../a/b/..", {"a", ""}},
        {"../a/b/.", {"a", "b", ""}},
        {"../a/b/./c", {"a", "b", "c"}},
        {"../a/b/./c/", {"a", "b", "c", ""}},
        {"../a/b/../c", {"a", "c"}},
        {"../a/b/./../c/", {"a", "c", ""}},
        {"../a/b/./../c", {"a", "c"}},
        {"../a/b/./../c/", {"a", "c", ""}},
        {"../a/b/.././c/", {"a", "c", ""}},
        {"../a/b/.././c", {"a", "c"}},
        {"../a/b/.././c/", {"a", "c", ""}},
        {"/./c/d", {"", "c", "d"}},
        {"/../c/d", {"", "c", "d"}},
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

TEST(UriTests, ParseFromUriPathsWithPercentEncodedCharachter) {
    struct TestVector {
        std::string uriString;
        std::string pathFirstSegment;
    };
    const std::vector<TestVector> testVectors {
        {"%41", "A"},
        {"%4A", "J"},
        {"%4a", "J"},
        {"%bc", "\xbc"},
        {"%Bc", "\xbc"},
        {"%bC", "\xbc"},
        {"%BC", "\xbc"},
        {"%41%42%43", "ABC"},
        {"%41%4A%43%4b", "AJCK"}
    };
    size_t index = 0;
    for (const auto& test: testVectors) {
        Uri::Uri uri;
        ASSERT_TRUE(uri.ParseFromString(test.uriString)) << index;
        ASSERT_EQ(test.pathFirstSegment, uri.GetPath()[0]) << index;
        ++index;
    }
    
}

TEST(UriTests, ReferenceResolution) {
    const std::string baseString("http://a/b/c/d;p?q");
    Uri::Uri baseUri;
    ASSERT_TRUE(baseUri.ParseFromString(baseString));
    struct TestVector {
        std::string baseUri;
        std::string relativeReferenceString;
        std::string target;
    };
    const std::vector< TestVector > testVectors {
        {"http://a/b/c/d;p?q", "g:h", "g:h"},
        {"http://a/b/c/d;p?q", "g", "http://a/b/c/g"},
        {"http://a/b/c/d;p?q", "./g", "http://a/b/c/g"},
        {"http://a/b/c/d;p?q", "g/", "http://a/b/c/g/"},
        {"http://a/b/c/d;p?q", "/g", "http://a/g"},
        {"http://a/b/c/d;p?q", "//g", "http://g"},
        {"http://a/b/c/d;p?q", "?y" ,  "http://a/b/c/d;p?y"},
        {"http://a/b/c/d;p?q", "g?y",  "http://a/b/c/g?y"},
        {"http://a/b/c/d;p?q", "#s" ,  "http://a/b/c/d;p?q#s"},
        {"http://a/b/c/d;p?q", "g#s",  "http://a/b/c/g#s"},
        {"http://a/b/c/d;p?q", "g?y#s", "http://a/b/c/g?y#s"},
        {"http://a/b/c/d;p?q", ";x" ,  "http://a/b/c/;x"},
        {"http://a/b/c/d;p?q", "g;x",  "http://a/b/c/g;x"},
        {"http://a/b/c/d;p?q", "g;x?y#s", "http://a/b/c/g;x?y#s"},
        {"http://a/b/c/d;p?q", ""   ,  "http://a/b/c/d;p?q"},
        {"http://a/b/c/d;p?q", "."  ,  "http://a/b/c/"},
        {"http://a/b/c/d;p?q", "./" ,  "http://a/b/c/"},
        {"http://a/b/c/d;p?q", ".." ,  "http://a/b/"},
        {"http://a/b/c/d;p?q", "../",  "http://a/b/"},
        {"http://a/b/c/d;p?q", "../g" , "http://a/b/g"},
        {"http://a/b/c/d;p?q", "../..", "http://a/"},
        {"http://a/b/c/d;p?q", "../../" , "http://a/"},
        {"http://a/b/c/d;p?q", "../../g", "http://a/g"},
        {"http://example.com", "foo", "http://example.com/foo"},
    };
    size_t index = 0;
    for (const auto& test: testVectors) {
        Uri::Uri relativeReferenceUri, expectedTargetUri;
        ASSERT_TRUE(baseUri.ParseFromString(test.baseUri));
        ASSERT_TRUE(relativeReferenceUri.ParseFromString(test.relativeReferenceString)) << index;
        ASSERT_TRUE(expectedTargetUri.ParseFromString(test.target)) << index;
        const auto actualTargetUri = baseUri.Resolve(relativeReferenceUri);
        ASSERT_EQ(expectedTargetUri, actualTargetUri) << index;
        ++index;
    }
}

TEST(UriTests, EmptyPathInUriWithAuthorityIsAquivalentToSlashOnlyPath) {
    Uri::Uri uri1, uri2;
    ASSERT_TRUE(uri1.ParseFromString("http://example.com"));
    ASSERT_TRUE(uri2.ParseFromString("http://example.com/"));
    ASSERT_EQ(uri1, uri2);
    ASSERT_TRUE(uri1.ParseFromString("//example.com"));
    ASSERT_TRUE(uri2.ParseFromString("//example.com/"));
    ASSERT_EQ(uri1, uri2);
}

TEST(UriTests, IPv6Address) {
    struct TestVector {
        std::string uriString;
        std::string expectedHost;
        bool isValid;
    };
    const std::vector< TestVector > testVectors {
        // vlid IPv6
        {"http://[::1]/", "::1", true},
        {"http://[::ffff:1.2.3.4]/", "::ffff:1.2.3.4", true},
        {"http://[2001:db8:85a3:8d3:1319:8a2e:370:7348]/", "2001:db8:85a3:8d3:1319:8a2e:370:7348", true},
        {"http://[fFfF:1:2:3:4:5:6:a]", "fFfF:1:2:3:4:5:6:a", true},
        {"http://[fFfF::1]", "fFfF::1", true},
        //invalid
        {"http://[::ffff:1.2.x.4]/", "", false},
        {"http://[::ffff:1.2.3.4.9]/", "", false},
        {"http://[::ffff:1.2.3]/", "", false},
        {"http://[::ffff:1.2.3.256]/", "", false},
        {"http://[::ffff:1.2.3.]/", "", false},
        {"http://[::fxff:1.2.3.4]/", "", false},
        {"http://[::ffff:1.2.3. 4]/", "", false},
        {"http://[::ffff:1.2.3.-4]/", "", false},
        {"http://[::ffff:1.2.3.4 ]/", "", false},
        {"http://[::fxff:1.2.3.4/", "", false},
        {"http://[2001:db8:85a3:8d3:1319:8a2e:370:7348:0000]/", "", false},
        {"http://[2001:db8:85a3::8a2e:0:]/", "", false},
        {"http://[2001:db8:85a3:::8a2e::]/", "", false},
        {"http://[]/", "", false}
    };
    size_t index = 0;
    for (auto& test: testVectors) {
        Uri::Uri baseUri;
        const bool parseResult = baseUri.ParseFromString(test.uriString);
        ASSERT_EQ(test.isValid, parseResult) << index;
        if (parseResult) {
            ASSERT_EQ(test.expectedHost, baseUri.GetHost()) << index;
        }
        ++index;
    }
}

TEST(UriTests, GenerateStringFromUriElements_Test) {
    struct TestVector {
        std::string stringScheme;
        struct UserInfo
        {
            std::string name;
            std::string pass;
        } userInfo; 
        std::string stringhost;
        bool hasPort;
        uint16_t port;
        std::vector<std::string> path;
        bool hasQuery;
        std::string query;
        bool hasFragment;
        std::string fragment;
        std::string expectedUriString;
    };
    const std::vector< TestVector > testVectors {
        {"http",     {"", ""}, "www.example.com",   true,   8080,   {"", "path1", "path2"}, true, "library",    true, "book", "http://www.example.com:8080/path1/path2?library#book"},
        {"",         {"", ""}, "example.com",       false,  0,      {},                     true, "book",       false, "",  "//example.com?book"},
        {"",         {"", ""}, "example.com",       true,   8080,   {""},                   false, "",           false, "", "//example.com:8080/"},
        {"",         {"", ""}, "",                  false,   0,     {},                     true, "bar",        false, "", "?bar"},
        {"http",     {"", ""}, "",                  false,   0,     {},                     true, "bar",        false, "", "http:?bar"},
        {"http",     {"", ""}, "",                  false,   0,     {},                     false, "",           false, "", "http:"},
        {"http",     {"", ""}, "::1",               false,   0,     {},                     false, "",           false, "", "http://[::1]"},
        {"http",     {"", ""}, "::1.2.3.4",         false,   0,     {},                     false, "",           false, "", "http://[::1.2.3.4]"},
        {"http",     {"", ""}, "1.2.3.4",           false,   0,     {},                     false, "",           false, "", "http://1.2.3.4"},
        {"",         {"", ""}, "",                  false,   0,     {},                     false, "",           false, "", ""},
        {"",         {"", ""}, "",                  false,   0,     {"", "abc", ""},        false, "",           false, "", "/abc/"},

        {"http", {"b b", ""}, "www.example.com", true,  8080, {"", "abc", "def"}, true,  "foobar", true,  "ch2", "http://b%20b@www.example.com:8080/abc/def?foobar#ch2"},
        {"http", {"bob", ""}, "www.e ample.com", true,  8080, {"", "abc", "def"}, true,  "foobar", true,  "ch2", "http://bob@www.e%20ample.com:8080/abc/def?foobar#ch2"},
        {"http", {"bob", ""}, "www.example.com", true,  8080, {"", "a c", "def"}, true,  "foobar", true,  "ch2", "http://bob@www.example.com:8080/a%20c/def?foobar#ch2"},
        {"http", {"bob", ""}, "www.example.com", true,  8080, {"", "abc", "def"}, true,  "foo ar", true,  "ch2", "http://bob@www.example.com:8080/abc/def?foo%20ar#ch2"},
        {"http", {"bob", ""}, "www.example.com", true,  8080, {"", "abc", "def"}, true,  "foobar", true,  "c 2", "http://bob@www.example.com:8080/abc/def?foobar#c%202"},
        //{"http", {"bob", ""}, "ሴ.example.com",   true,  8080, {"", "abc", "def"}, true,  "foobar", false, "",    "http://bob@%E1%88%B4.example.com:8080/abc/def?foobar"},

        {"http", {"bob", ""}, "fFfF::1", true,  8080, {"", "abc", "def"}, true,  "foobar", true,  "c 2", "http://bob@[ffff::1]:8080/abc/def?foobar#c%202"}
    };
    size_t index = 0;
    for (const auto& test: testVectors) {
        Uri::Uri uri;
        uri.SetScheme(test.stringScheme);
        uri.SetUserName(test.userInfo.name);
        uri.SetUserPass(test.userInfo.pass);
        uri.SetHost(test.stringhost);
        if(test.hasPort) {
            uri.SetPort(test.port);
        }
        uri.SetPath(test.path);
        if (test.hasQuery) {
            uri.SetQuery(test.query);
        }
        if (test.hasFragment) {
            uri.SetFragment(test.fragment);
        }
        const auto actualUriString = uri.GenerateString();
        ASSERT_EQ(test.expectedUriString, actualUriString) << index;
        ASSERT_EQ(test.hasPort, uri.HasPort()) << index;
        ++index;
    }

}

TEST(UriTests, FragmentEmptyButPresent_Test) {
    Uri::Uri uri;
    ASSERT_TRUE(uri.ParseFromString("http://example.com#"));
    ASSERT_TRUE(uri.HasFragment());
    ASSERT_EQ("", uri.GetFragment());
    ASSERT_EQ("http://example.com/#", uri.GenerateString()) << 1;
    uri.ClearFragment();
    ASSERT_EQ("http://example.com/", uri.GenerateString()) << 2;
    ASSERT_FALSE(uri.HasFragment());
    ASSERT_TRUE(uri.ParseFromString("http://example.com"));
    ASSERT_FALSE(uri.HasFragment());
    uri.SetFragment("");
    ASSERT_TRUE(uri.HasFragment());
    ASSERT_EQ("", uri.GetFragment());
    ASSERT_EQ("http://example.com/#", uri.GenerateString()) << 3;
}

TEST(UriTests, QueryEmptyButPresent) {
    Uri::Uri uri;
    ASSERT_TRUE(uri.ParseFromString("http://example.com?"));
    ASSERT_TRUE(uri.HasQuery());
    ASSERT_EQ("", uri.GetQuery());
    ASSERT_EQ("http://example.com/?", uri.GenerateString()) << 1;
    uri.ClearQuery();
    ASSERT_EQ("http://example.com/", uri.GenerateString()) << 2;
    ASSERT_FALSE(uri.HasQuery());
    ASSERT_TRUE(uri.ParseFromString("http://example.com"));
    ASSERT_FALSE(uri.HasQuery());
    uri.SetQuery("");
    ASSERT_TRUE(uri.HasQuery());
    ASSERT_EQ("", uri.GetQuery());
    ASSERT_EQ("http://example.com/?", uri.GenerateString()) << 3;
}

TEST(UriTests, UriTests_MakeCopy_Test) {
    Uri::Uri uri1;
    (void)uri1.ParseFromString("http://www.example.com/foo.txt");
    Uri::Uri uri2(uri1);
    uri1.SetQuery("bar");
    uri2.SetFragment("page2");
    uri2.SetHost("example.com");
    EXPECT_EQ("http://www.example.com/foo.txt?bar", uri1.GenerateString());
    EXPECT_EQ("http://example.com/foo.txt#page2", uri2.GenerateString());
}

TEST(UriTests, UriTests_AssignACopy__Test) {
    Uri::Uri uri1;
    (void)uri1.ParseFromString("http://www.example.com/foo.txt");
    Uri::Uri uri2 ;
    uri2 = uri1;
    uri1.SetQuery("bar");
    uri2.SetFragment("page2");
    uri2.SetHost("example.com");
    EXPECT_EQ("http://www.example.com/foo.txt?bar", uri1.GenerateString());
    EXPECT_EQ("http://example.com/foo.txt#page2", uri2.GenerateString());
}