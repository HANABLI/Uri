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
        /* data */
        std::string scheme;
        std::string host;
        std::vector<std::string> path;
        uint16_t port = 0;
        bool hasPort = false;
    };
    
    Uri::~Uri() = default;

    Uri::Uri()
        : impl_(new Impl){

        }
}



bool Uri::Uri::ParseFromString(const std::string &uriString)
{
    // scheme parse
    const auto schemeEnd = uriString.find(':');
    impl_->scheme = uriString.substr(0, schemeEnd);
    auto next = uriString.substr(schemeEnd + 1);
    // host parse
    if (next.substr(0, 2) == "//") {
        const auto authorityEnd = next.find('/', 2);
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
    return true;
}

std::string Uri::Uri::GetScheme() const
{
    return impl_->scheme;
}

std::string Uri::Uri::GetHost() const
{
    return impl_->host;
}

std::vector<std::string> Uri::Uri::GetPath() const
{
    return impl_->path;
}

bool Uri::Uri::HasPort() const
{
    return impl_->hasPort;
}

uint16_t Uri::Uri::GetPort() const
{
    return impl_->port;
}
