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
        impl_->host = next.substr(2, authorityEnd - 2);
        next = next.substr(authorityEnd);
    } else {
        impl_->host.clear();
    }

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
