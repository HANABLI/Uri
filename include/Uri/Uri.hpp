/**
 * @file Uri.hpp
 * 
 * This module contains the declaration of the Uri::Uri class.
 * 
 * © 2024 by Hatem Nabli
*/

#include <memory>

namespace Uri {

    /**
     * This class represents a Uniform Resource Identifier (URI),
     * as defined in RFC 3986 (https://tools/ieft.org/html/rfc3986).
    */

    class Uri {
        // Lifecycle management
    public:
        ~Uri();
        Uri(const Uri&) = delete; // Copy Constructor that creates a new object by making a copy of an existing object. 
        //It ensures that a deep copy is performed if the object contains dynamically allocated resources 
        Uri(Uri&&) = delete; // Move Constructor that transfers resources from an expiring object to a newly constructed object.
        Uri& operator=(const Uri&) = delete; //Copy Assignment Operation That assigns the values of one object to another object using the assignment operator (=)
        Uri& operator=(Uri&&) = delete; //Move Assignment Operator: Amove assignment operator efficiently transfers resources from one object to another.
        
        //public methods
    public:
        /**
         * This is the default constructor
        */
       Uri();

       //private properties
    private:
        /**
         * This is the type of structure that contains the private
         * properties of the instance. It is defined in the implementation
         * and declared here to ensure that iwt is scoped inside the class.
        */
       struct Impl;

       /**
        * This contains the private properties of the instance.
        */       
       std::unique_ptr<struct Impl> impl_;
   };
}