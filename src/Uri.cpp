/**
 * @file Uri.cpp
 * 
 * This module containe the implementation of the Uri::Uri class.
 * 
 * © 2024 by Hatem Nabli
*/

#include <Uri/Uri.hpp>

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
            auto authorityEnd = next.find('/', 2);
            if (authorityEnd == std::string::npos) {
                authorityEnd = next.find('?', 2);
                if (authorityEnd == std::string::npos) {
                    authorityEnd = next.find('#', 2);
                    if (authorityEnd == std::string::npos) {
                        authorityEnd = next.length();   
                    }
                }      
            }
            const auto portDelimiter = next.find(':');
            if (portDelimiter == std::string::npos) {
                impl_->host = next.substr(2, authorityEnd - 2);
            }
            else {
                impl_->host = next.substr(2, portDelimiter - 2);
                uint32_t newPort = 0;
                for (auto c: next.substr(portDelimiter + 1, authorityEnd - portDelimiter - 1 )) {
                    if ((c < '0') || (c > '9') ) {
                            return false;
                        }
                    newPort *= 10;
                    newPort += (uint16_t)(c - '0');
                    if (
                        (newPort & ~((1 << 16) - 1)) != 0
                    ) {
                        return false;
                    } 
                }
                impl_->port = (uint16_t)newPort;
                impl_->hasPort = true;
            }
            next = next.substr(authorityEnd);
        } else {
            impl_->host.clear();
        }

        // path parse
        impl_->path.clear();
        if (next._Equal("/"))
        {
            // Special case of an empty path.
            impl_->path.push_back("");
            next.clear();
        } else if (!next.empty()) {
            for(;;) {
                auto delimiter = next.find('/');
                if (delimiter == std::string::npos) {
                    impl_->path.push_back(next);
                    break;
                } else {
                    impl_->path.emplace_back(next.begin(), next.begin() + delimiter);
                    next = next.substr(delimiter + 1);
                }           
            }    
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

}

