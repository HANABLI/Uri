/**
 * @file Uri.cpp
 * 
 * This module containe the implementation of the Uri::Uri class.
 * 
 * © 2024 by Hatem Nabli
*/

#include "CharacterSet.hpp"
#include "NormalizeCaseInsensitiveString.hpp"
#include "PercentEncodedCharacterDecoder.hpp"

#include <Uri/Uri.hpp>

namespace {
    //Singleton
    /**
     * This is the character set containing the alphabetic characters
     * from the ASCII characters.
    */
    const auto ALPHA = Uri::CharacterSet{
            Uri::CharacterSet('a', 'z'),
            Uri::CharacterSet('A', 'Z') 
        };
    /**
     * This is the character set containing the numerical characters
     * from the ASCII set.
    */
    const auto DIGIT = Uri::CharacterSet('0', '9');

    const auto HEX = Uri::CharacterSet{
            Uri::CharacterSet('A', 'F'), 
            Uri::CharacterSet('a', 'f')
        };

    /**
     * This is the character st corresponds to the "unreserved" syntax
     * specified in RFC 3986 
    */
   const auto UNRESERVED = Uri::CharacterSet{
    ALPHA,
    DIGIT,
    '.', '-', '_', '~'
   };

    /**
     * This is the character st corresponds to the "sub-delims" syntax
     * specified in RFC 3986 
    */
   const auto SUB_DELIMS = Uri::CharacterSet{
    '!', '$', '&', '\'', '(', ')', '*', '+', ',', ';', '='
   };
    /**
     * This is the character st corresponds to the second part of the "scheme" syntax
     * specified in RFC 3986 
    */
   const auto SCHEME_NOT_FIRST = Uri::CharacterSet{
    ALPHA,
    DIGIT,
    '+', '-', '.'
   };
    /**
     * This is the character st corresponds to the "pchar" syntax
     * specified in RFC 3986 
    */
   const auto PCHAR_NOT_PCT_ENCODED = Uri::CharacterSet{
    UNRESERVED, SUB_DELIMS, 
    ':', '@'
    };

    const auto QUERY_OR_FRAGMENT_CHAR = Uri::CharacterSet{
        PCHAR_NOT_PCT_ENCODED, '/', '?'
    };

    const auto USER_INFO_CHAR = Uri::CharacterSet{
        UNRESERVED, SUB_DELIMS,':'};
    

    const auto IPV_LAST_PART_FUTURE = Uri::CharacterSet{
        UNRESERVED, SUB_DELIMS,':'
    };

    const auto HEXDIGIT = Uri::CharacterSet{
        Uri::CharacterSet('0', '9'),
        Uri::CharacterSet('A', 'F'),
        Uri::CharacterSet('a', 'f')
    };

    /**
     * This fuction parses the given string as an unsigned 16-bit
     * integer, detecting invalid characters, overflow, etc..
     * 
     * @param[in] numberString
     *      This is the string containing the number to parse.
     * 
     * @param[out] number
     *      This is where to store the number parsed.
     * 
     * @return
     *      An indication of whether or not the number was parsed
     *      successfully is returned. 
    */
    bool ParseUint16(const std::string& numberString, uint16_t& number) {
        uint32_t number32Bits = 0;
        for (auto c: numberString) {
            if ((c < '0') || (c > '9') ) {
                    return false;
                }
            number32Bits *= 10;
            number32Bits += (uint16_t)(c - '0');
            if (
                (number32Bits & ~((1 << 16) - 1)) != 0
            ) {
                return false;
            } 
        }
        number = (uint16_t)number32Bits;
        return true;
    }

    /**
     * This function takes a given "stillPassing" strategy
     * and invokes it on the sequence of characters in the given
     * string, to check if the string passes or not.
     * 
     * @param[in] candidate
     *      This is the string to test.
     * @param[in] stillOkStrategy
     *      This is the strategy to invoke in the order to test the string.
     * @return
     *      An indication of whether or not the given candidate string
     *      passes the test strategy is returned.
    */
    bool FailsMatch( const std::string& candidate, 
        std::function<bool(char, bool)> stillOkStrategy) {
            for (const auto c: candidate) {
                if(!stillOkStrategy(c, false)) {
                    return true;
                }
            }
            return !stillOkStrategy(' ', true);
    }


    /**
     * This function returns a strategy function that 
     * my be used with the FailsMatch function to test a scheme
     * and make sure it is legal according to the standard.
     * 
     * @return
     *      returns a strategy function that 
     *      my be used with the FailsMatch function to test a scheme
     *      and make sure it is legal according to the standard.
    */
    std::function<bool(char, bool)> LegalSchemeStartegy()  {
        auto isFirstCharacter = std::make_shared<bool>(true);
        return [isFirstCharacter](char c, bool end) {
                if (end) {
                    return !*isFirstCharacter;
                } else {
                    bool check;
                    if(*isFirstCharacter) {
                        check = ALPHA.Contains(c);
                    } else {
                        check = SCHEME_NOT_FIRST.Contains(c);

                    }
                    *isFirstCharacter = false;
                    return check;
                }            
            };
    }

    
}

namespace Uri {
    /**
     * This contains the private properties of Uri instance.
    */

    struct Uri::Impl
    {
        /**
         * This is the scheme elemen of the Uri.  
        */
        std::string scheme;
        /**
         * This is the host element of the Uri.
        */
        std::string host;
        /**
         * This is the path elements vector of the Uri.
        */
        std::vector<std::string> path;
        /**
         * This is the port element of the Uri.
        */
        uint16_t port = 0;
        /**
         * This is an indication of whether or not the Uri has a port number.
        */
        bool hasPort = false;
        /**
         * This is the query element of the Uri.
        */
        std::string query;
        /**
         * This is the fragment element of the Uri.
        */
        std::string fragment;
        /**
         * This is the User info structure.
        */
        UserInfo userInfo;

        // Methods
        /**
         * This method checks and decodes the given URI element.
         * 
         * @param[in, out] element
         *      On input, this is the uri element to check and decode.
         *      On output, this is the decoded element.
         * @return
         *      return an indication of whether or not the element
         *      passed all checks and was decoded successfully.
        */
        bool DecodeElement(std::string& element, const CharacterSet& charachterSetAllowed) {
                const auto entryElement = std::move(element);
                element.clear();
                bool decodingPec = false;
                PercentEncodedCharacterDecoder pDecoder;
                for (const auto c: entryElement) {
                    if(decodingPec) {
                        if (!pDecoder.NextEncodedCharacter(c)) {
                            return false;
                        }
                        if (pDecoder.Done()) {
                            decodingPec = false;
                            element.push_back((char)pDecoder.GetDecodedCharacter());
                        }
                    } else {
                        if (c == '%') {
                            decodingPec = true;
                            pDecoder = PercentEncodedCharacterDecoder();
                        }
                        else {
                            if (charachterSetAllowed.Contains(c)) {
                                element.push_back(c);
                            }
                            else {
                                return false;
                            }
                        }
                    } 
                }
                return true;        
            }

        /**
         * This method checks and decode query or fragment elements of 
         * a String Uri.
         * 
         * @param[in, out] queryOrFragment
         *      On input, we have the Query or Fragment element to check and decode.
         *      On output, we have the decoded Query or Fragment element
         * @return
         *      return an indication of whether or not the element 
         *      passed all checks and was decoded
        */
        bool DecodeQueryOrFragment(std::string& queryOrFragment) {
           if (!DecodeElement(queryOrFragment, QUERY_OR_FRAGMENT_CHAR)) {
                return false;
           }
           return true;
        }

        
        /**.
         * This method checks and decode the path segment.
         * 
         * @param[in, out] segment
         *      On input, this is the path segment to check and decode.
         *      On output, this is the decoded path segment.
         * @return
         *      return an indication of whether or not the path segment
         *      passed all checks and was decoded successfully.
        */
        bool DecodePathSegment(std::string& segment) {
            if (!DecodeElement(segment, PCHAR_NOT_PCT_ENCODED)) {
                return false;
            }
            return true;
        }

        /**
         * This method builds the internal path element sequence
         * by parsing it from the given path string.
         * 
         * @param[in] pathString
         *      This is the whole path of the Uri.
         * 
         * @return
         *      An indication if the path was parsed correctly or not
         *      is returned.
        */
        bool ParsePath(std::string& pathString) {
            path.clear();
            if (pathString._Equal("/"))
            {
                // Special case of an empty path.
                path.push_back("");
                pathString.clear();
            } else if (!pathString.empty()) {
                for(;;) {
                    auto delimiter = pathString.find('/');
                    if (delimiter == std::string::npos) {
                        path.push_back(pathString);
                        pathString.clear();
                        break;
                    } else {
                        path.emplace_back(pathString.begin(), pathString.begin() + delimiter);
                        pathString = pathString.substr(delimiter + 1);
                    }           
                }
                if (!path.empty()) {
                    for (auto& segment: path) {
                        if(!DecodePathSegment(segment)) {
                            return false;
                        }
                    }
                }    
            }

            return true;
        }

        /**
         * This method parses the elemnets that  make up the authority
         * composite section of the Uri.
         * 
         * @param[in] authority
         *      This is the string containing the whole authority part of the Uri.
         * 
         * @return
         *      An indication if the 
         * 
        */
        bool ParseAuthority(std::string& authorityString) {
            // user info parse
            auto userInfoEnd = authorityString.find('@');
            if (userInfoEnd == std::string::npos) {
                userInfo.name.clear();
                userInfo.pass.clear();
            } else {
                std::string uriUserInfo = authorityString.substr(0, userInfoEnd );
                size_t decoderState = 0;
                std::string userInfodecoded;
                int decodedCharacter = 0;
                PercentEncodedCharacterDecoder pDecoder;
                //State Machine pattern
                for (const auto c: uriUserInfo) {
                    switch (decoderState)
                    {
                        case 0: {
                            if (c == '%') {
                                decoderState = 1;
                                pDecoder = PercentEncodedCharacterDecoder();
                            } else {
                                if (USER_INFO_CHAR.Contains(c)) {
                                    userInfodecoded.push_back(c);
                                }
                                else {
                                    return false;
                                }
                            }
                        } break;
                        
                        case 1: {
                            if (!pDecoder.NextEncodedCharacter(c)) {
                                return false;
                            }
                            if (pDecoder.Done()) {
                                decoderState = 0;
                                userInfodecoded.push_back((char)pDecoder.GetDecodedCharacter());
                            }
                        } break;
                    }
                }
                const auto userInfoDelimiter = userInfodecoded.find(':');
                if ( userInfoDelimiter == std::string::npos) {
                    userInfo.name = userInfodecoded;
                } else {
                    userInfo.name = userInfodecoded.substr(0, userInfoDelimiter);
                    userInfo.pass = userInfodecoded.substr(userInfoDelimiter + 1);
                }
                authorityString = authorityString.substr(userInfoEnd + 1);
            }

            auto authorityEnd = authorityString.find('/');
            if (authorityEnd == std::string::npos) {
                authorityEnd = authorityString.find('?');
                if (authorityEnd == std::string::npos) {
                    authorityEnd = authorityString.find('#');
                    if (authorityEnd == std::string::npos) {
                        authorityEnd = authorityString.length();
                    }
                }
            }
            // host parse
            //const auto portDelimiter = authorityString.find(':');
            std::string hostName, portString;
            hostName = authorityString.substr(0, authorityEnd );
            std::string encodedHostName;
            size_t decoderState = 0;
            int decodedCharacter = 0;
            bool isRegChar = false;
            for (const auto c: hostName) {
                switch (decoderState)
                {
                    case 0: { // first character
                        if (c == '[') {
                            decoderState = 4;
                            encodedHostName.push_back(c);
                            break;
                        } else {
                            decoderState = 1;
                            isRegChar = true;
                        }             
                    } 

                    case 1: { //reg-name or IPv4Address
                        if (c == '%') {
                            decoderState = 2;
                        } else if (c == ':') {
                            decoderState = 9;
                        } else {
                            if (USER_INFO_CHAR.Contains(c)) {
                                encodedHostName.push_back(c);
                            }
                            else {
                                return false;
                            }
                        }
                    } break;
                    
                    case 2: {
                        decoderState = 3;
                        decodedCharacter <<= 4;
                        if (DIGIT.Contains(c)) {
                            decodedCharacter += (int)(c - '0');
                        } else if (HEX.Contains(c)) {
                            decodedCharacter += (int)(c - 'A') + 10;
                        } else {
                            return false;
                        }
                    } break;

                    case 3: { // %[0-9A-F] ...
                        decodedCharacter <<= 4;
                        decoderState = 1;
                        if (DIGIT.Contains(c)) {                         
                            decodedCharacter += (int)(c - '0');                      
                        } else if (HEX.Contains(c)) {
                            decodedCharacter += (int)(c - 'A') + 10;
                        } else {
                            return false;
                        }
                        encodedHostName.push_back((char)decodedCharacter);
                    } break;

                    case 4: { // IP-literal
                        if (c == 'v') {
                            decoderState = 5;
                            encodedHostName.push_back(c);           
                            break;
                        } else {
                            decoderState = 6;
                        }
                    } 

                    case 5: { // IPvFuture: v 
                        if (c == '.') {
                            decoderState = 7;
                        } else if (!HEXDIGIT.Contains(c)) {
                            return false;
                        }
                        encodedHostName.push_back(c);
                    } break;

                    case 6: { // IPv6Address
                        // TODO: Later
                        encodedHostName.push_back(c);
                        if (c == ']') {
                            decoderState = 8;
                        }
                    } break;

                    case 7: { // IPvFuture v 1*HEXDIG
                            encodedHostName.push_back(c);
                            if(c == ']') {
                                decoderState = 8;
                            } else if (!IPV_LAST_PART_FUTURE.Contains(c)) {
                                return false;
                            }
                    } break;

                    case 8: { // we can't have anithing else, unless it's a colon,
                                // in which case it's a port delimiter
                        if (c == ':') {
                            decoderState = 9;
                        } else {
                            return false;
                        }                            
                    } break;

                    case 9: { // port
                        portString.push_back(c);
                    } break;
                }
            }
            if (portString.empty()) {
                hasPort = false;
            } else {
                if (!ParseUint16(portString, port)) {
                    return false;
                }
                hasPort = true;
            }
            if (isRegChar) {
                host = NormalizeCaseInsensitiveString(encodedHostName);
            } else {
                host = encodedHostName;
            }
            
            authorityString = authorityString.substr(authorityEnd);
            return true;
        }
        /**
         * This method parse fragment from uri string part 
         * 
         * @param[in, out] elements
         *      The part of uri string containing fragment element
         * @return
         *      return an indication of whether or not the fragment was extracted
         *      from the uri string part.
        */
        bool ParseFragment(std::string& elements) {
            const auto fragmentDelimiter = elements.find('#');
            if (fragmentDelimiter == std::string::npos) {
                fragment.clear();
            } else {
                fragment = elements.substr(fragmentDelimiter + 1);
            }
            if(!DecodeQueryOrFragment(fragment)) {
                return false;
            }
            return true;
        }


        /**
         * This method parse query element from uri string part 
         * 
         * @param[in, out] elements
         *      The part of uri string containing query element
         * @return
         *      return an indication of whether or not the query was extracted
         *      from the uri string part.
        */
        bool ParseQuery(std::string& elements) {
            const auto queryDelimiter = elements.find('?');
            const auto fragmentDelimiter = elements.find('#');
            if (queryDelimiter == std::string::npos) {
                query.clear();
            } else {
                if (fragmentDelimiter != std::string::npos) {
                    query = elements.substr(queryDelimiter + 1, fragmentDelimiter - queryDelimiter - 1 );
                } else {
                    query = elements.substr(queryDelimiter + 1);
                }
            }
            if(!DecodeQueryOrFragment(query)) {
                return false;
            }
            return true;
        }

        
    /**
     * This method determines whether or not it makes sense to
     * navigate one level up from the current path
     * (in other words, does appending ".." to the path
     * actually change the path?)
     *
     * @return
     *     An indication of whether or not it makes sense to
     *     navigate one level up from the current path is returned.
     */
    bool CanNavigatePathUpOneLevel() const {
        return (
            !IsPathAbsolute()
            || (path.size() > 1)
        );
    }
    /**
     * This method returns an indication of whether or not the
     * path of the URI is an absolute path, meaning it begins
     * with a forward slash ('/') character.
     *
     * @return
     *     An indication of whether or not the path of the URI
     *     is an absolute path, meaning it begins
     *     with a forward slash ('/') character is returned.
     */
    bool IsPathAbsolute() const {
        return (
            !path.empty()
            && (path[0] == "")
        );
    }
    };
    
    Uri::~Uri() = default;

    Uri::Uri(Uri&&) = default;
    Uri& Uri::operator=(Uri&&) = default;
    Uri::Uri()
        : impl_(new Impl){

        }

    bool Uri::operator==(const Uri& other) const {
        return (
            (impl_->scheme == other.GetScheme()) &&
            (impl_->userInfo == other.GetUserInfo()) &&
            (impl_->host == other.GetHost()) &&
            ((!impl_->hasPort && !other.HasPort()) || 
            (impl_->hasPort && other.HasPort()) && 
            (impl_->port == other.GetPort())) &&
            (impl_->query == other.GetQuery()) &&
            (impl_->fragment == other.GetFragment()) &&
            (impl_->path == other.GetPath())
        );
    }

    bool Uri::operator!=(const Uri& other) const {
        return !(*this == other);
    }

    bool Uri::ParseFromString(const std::string &uriString)
    {

        auto authorityOrPathDelimiter = uriString.find('/');
        if (authorityOrPathDelimiter == std::string::npos) {
            authorityOrPathDelimiter = uriString.length();
        }

        // scheme parse
        const auto schemeEnd = uriString.substr(0, authorityOrPathDelimiter).find(':');
        std::string next;
        if (schemeEnd == std::string::npos) {
            impl_->scheme.clear();
            next = uriString;
        } else {
            impl_->scheme = uriString.substr(0, schemeEnd);
            if(FailsMatch(impl_->scheme, LegalSchemeStartegy())) {
                return false;
            }
            impl_->scheme = NormalizeCaseInsensitiveString(impl_->scheme);
            next = uriString.substr(schemeEnd + 1);
        }
        
        if (next.substr(0, 2) == "//") {
            
            next = next.substr(2);
            // authority parse
            if(!impl_->ParseAuthority(next)) {
                return false;
            }
            
        } else {
            //no host so neither userInfo 
            impl_->userInfo.clear();
            impl_->host.clear();
            impl_->hasPort = false;
        }
        auto pathString = next.substr(0, next.find_first_of("?#"));
        // path parse
        if(!impl_->ParsePath(pathString)) {
            return false;
        }

        //Handle special case of absolute URI with empty
        //path -- treat the same as "/" path.
        if (!impl_->host.empty() && impl_->path.empty()) {
            impl_->path.push_back("");
        }
        // fragment
        if (!impl_->ParseFragment(next)) {
            return false;
        }

        // query
        if(!impl_->ParseQuery(next)) {
            return false;
        }
        return true;
    }

    std::string Uri::GetScheme() const
    {
        return impl_->scheme;
    }

    std::string Uri::GetHost() const
    {
        return impl_->host;
    }

    std::vector<std::string> Uri::GetPath() const
    {
        return impl_->path;
    }

    bool Uri::HasPort() const
    {
        return impl_->hasPort;
    }

    uint16_t Uri::GetPort() const
    {
        return impl_->port;
    }

    bool Uri::IsRelativeReference() const
    {
        return impl_->scheme.empty();
    }

    bool Uri::HasRelativePath() const
    {
        if (impl_->path.empty()) {
            return true;
        } else {
            return !impl_->path[0].empty();
        }
    }
    
    std::string Uri::GetQuery() const {
        return impl_->query;
    }

    std::string Uri::GetFragment() const
    {
        return impl_->fragment;
    }

    UserInfo Uri::GetUserInfo() const
    {
        return impl_->userInfo;
    }

    void Uri::NormalizePath() {
        // Rebuild the path one segment
        // at a time, removing and applying special
        // navigation segments ("." and "..") as we go.
        auto oldPath = std::move(impl_->path);
        impl_->path.clear();
        bool atDirectoryLevel = false;
        for (const auto segment: oldPath) {
            if (segment == ".") {
                atDirectoryLevel = true;
            } else if (segment == "..") {
                // Remove last path element
                // if we can navigate up a level.
                if (!impl_->path.empty()) {
                    if (impl_->CanNavigatePathUpOneLevel()) {
                        impl_->path.pop_back();
                    }
                }
                atDirectoryLevel = true;
            } else {
                // Non-relative elements can just
                // transfer over fine.  An empty
                // segment marks a transition to
                // a directory level context.  If we're
                // already in that context, we
                // want to ignore the transition.
                if (
                    !atDirectoryLevel
                    || !segment.empty()
                ) {
                    impl_->path.push_back(segment);
                }
                atDirectoryLevel = segment.empty();
            }
        }

        // If at the end of rebuilding the path,
        // we're in a directory level context,
        // add an empty segment to mark the fact.
        if (
            atDirectoryLevel
            && (
                !impl_->path.empty()
                && !impl_->path.back().empty()
            )
        ) {
            impl_->path.push_back("");
        }
    }
    Uri Uri::Resolve(const Uri& relativeReference) const {
        //  Resole the reference according to the algorithm
        //  from section 5.2.2 in
        //  RFC 3986
        Uri target;
        if (!relativeReference.impl_->scheme.empty()) {
            target.impl_->scheme = relativeReference.impl_->scheme;
            target.impl_->host = relativeReference.impl_->host;
            target.impl_->path = relativeReference.impl_->path;
            target.impl_->hasPort = relativeReference.impl_->hasPort;
            target.impl_->port = relativeReference.impl_->port;
            target.impl_->userInfo = relativeReference.impl_->userInfo;
            target.NormalizePath();
            target.impl_->query = relativeReference.impl_->query;
        } else {
            if(!relativeReference.impl_->host.empty()) {
                target.impl_->host = relativeReference.impl_->host;
                target.impl_->path = relativeReference.impl_->path;             
                target.impl_->hasPort = relativeReference.impl_->hasPort;
                target.impl_->port = relativeReference.impl_->port;
                target.impl_->userInfo = relativeReference.impl_->userInfo;
                target.NormalizePath();
                target.impl_->query = relativeReference.impl_->query;
            } else {
                if(relativeReference.impl_->path.empty()) {
                    target.impl_->path = relativeReference.impl_->path;
                    if(!relativeReference.impl_->query.empty()) {
                        target.impl_->query = relativeReference.impl_->query;
                    } else {
                        target.impl_->query = impl_->query;
                    }
                } else {
                    if (
                        !relativeReference.impl_->path.empty() &&
                        (relativeReference.impl_->path[0] == "")
                    ) {
                        target.impl_->path = relativeReference.impl_->path;
                        target.NormalizePath();
                    } else {
                        target.impl_->path = impl_->path;
                        if(target.impl_->path.size() > 1) {
                            target.impl_->path.pop_back();
                        }
                        std::copy(relativeReference.impl_->path.begin(), 
                            relativeReference.impl_->path.end(), 
                            std::back_inserter(target.impl_->path));
                            target.NormalizePath();
                    }
                    target.impl_->query = relativeReference.impl_->query;
                }
                target.impl_->host = impl_->host;
                target.impl_->userInfo = impl_->userInfo;
                target.impl_->hasPort = impl_->hasPort;
                target.impl_->port = impl_->port;
            }
            target.impl_->scheme = impl_->scheme;
        }
        target.impl_->fragment = impl_->fragment;
        return target;
    }
}

