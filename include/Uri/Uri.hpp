/**
 * @file Uri.hpp
 * 
 * This module contains the declaration of the Uri::Uri class.
 * 
 * © 2024 by Hatem Nabli
*/

#include <memory>
#include <string>
#include <vector>

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

       /**
        * This method build the URI from the elements parsed
        * from the given string rendering of a URI
        * 
        * @param[in] uriString
        *       This is the string rendering of the URI to parce.
        * @return
        *       whether or not the URI was parsed sucssefully is returnd.
       */
       bool ParseFromString(const std::string& uriString);

        /**
         * This method returns the "scheme" element of the URI.
         * 
         * @return
         *      The "scheme" element of the URI.
         * @retval ""
         *      An empty string is returned if there is no "scheme" element in the URI.
        */
       std::string GetScheme() const;

        /**
         * This method returns the "host name" element of the URI.
         * 
         * @return
         *      The "host name" element of the URI.
         * 
         * @retval ""
         *      An empty string is returned if there is no "host name" element in the URI.
        */
       std::string GetHost() const;

        /**
         * This method returns the "path" element of the URI as a sequence of steps.
         * 
         * @note    
         *      If the first sep of the path is an empty string, then 
         *      the URI has an absolute path.
         * 
         * @return
         *      The "path" element of the URI is returned 
         *      as a sequence of segments
        */
       std::vector<std::string> GetPath() const;

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