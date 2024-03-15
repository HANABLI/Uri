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
    };
    
    Uri::~Uri() = default;

    Uri::Uri()
        : impl_(new Impl){

        }
}