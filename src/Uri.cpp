/**
 * @file Uri.cpp
 * 
 * This module containe the implementation of the Uri::Uri class.
 * 
 * © 2024 by Hatem Nabli
*/

#include <Uri/Uri.hpp>

namespace {
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
        bool ParsePath(std::string pathString) {
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
            }
            return true;
        }
    };
    
    Uri::~Uri() = default;

    Uri::Uri()
        : impl_(new Impl){

        }

    bool Uri::ParseFromString(const std::string &uriString)
    {
        // scheme parse
        const auto schemeEnd = uriString.find(':');
        std::string next;
        if (schemeEnd == std::string::npos) {
            impl_->scheme.clear();
            next = uriString;
        } else {
            impl_->scheme = uriString.substr(0, schemeEnd);
            next = uriString.substr(schemeEnd + 1);
        }
        
        // host parse
        if (next.substr(0, 2) == "//") {
            
            next = next.substr(2);
            
            auto userInfoEnd = next.find('@');
            if (userInfoEnd == std::string::npos) {
                impl_->userInfo.name.clear();
                impl_->userInfo.pass.clear();
            } else {
                std::string userInfo = next.substr(0, userInfoEnd );
                const auto userInfoDelimiter = userInfo.find(':');
                if ( userInfoDelimiter == std::string::npos) {
                    impl_->userInfo.name = userInfo;
                } else {
                    impl_->userInfo.name = userInfo.substr(0, userInfoDelimiter);
                    impl_->userInfo.pass = userInfo.substr(userInfoDelimiter + 1);
                }
                next = next.substr(userInfoEnd + 1);
            }

            auto authorityEnd = next.find('/');
            if (authorityEnd == std::string::npos) {
                authorityEnd = next.find('?');
                if (authorityEnd == std::string::npos) {
                    authorityEnd = next.find('#');
                    if (authorityEnd == std::string::npos) {
                        authorityEnd = next.length();
                    }
                }
            }

            const auto portDelimiter = next.find(':');
            if (portDelimiter == std::string::npos) {
                impl_->host = next.substr(0, authorityEnd );
                impl_->hasPort = false;
            }
            else {
                impl_->host = next.substr(0, portDelimiter );
                const auto portString = next.substr(portDelimiter + 1, authorityEnd - portDelimiter - 1 );
                if (
                    !ParseUint16(portString, impl_->port)
                ) {
                    return false;
                }
                impl_->hasPort = true;
            }
            next = next.substr(authorityEnd);
        } else {
            //no host so neither userInfo 
            impl_->userInfo.clear();
            impl_->host.clear();
        }

        // path parse
        if(!impl_->ParsePath(next)) {
            return false;
        }
        

        const auto fragmentDelimiter = next.find('#');
        if (fragmentDelimiter == std::string::npos) {
            impl_->fragment.clear();
        } else {
            impl_->fragment = next.substr(fragmentDelimiter + 1);
        }
        const auto queryDelimiter = next.find('?');
        if (queryDelimiter == std::string::npos) {
            impl_->query.clear();
        } else {
            if (fragmentDelimiter != std::string::npos) {
                impl_->query = next.substr(queryDelimiter + 1, fragmentDelimiter - queryDelimiter - 1 );
            } else {
                impl_->query = next.substr(queryDelimiter + 1);
            }
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
}

