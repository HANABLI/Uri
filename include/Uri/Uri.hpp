#ifndef URI_HPP
#define URI_HPP
/**
 * @file Uri.hpp
 * 
 * This module contains the declaration of the Uri::Uri class.
 * 
 * © 2024 by Hatem Nabli
*/



#include <functional>
#include <inttypes.h>
#include <memory>
#include <stdio.h>
#include <string>
#include <vector>

namespace Uri {
    /**
     * This struct represents the user informations element
    */
    struct UserInfo
    {
        std::string name;
        std::string pass;
        void clear() {
            name.clear();
            pass.clear();
        }
        bool operator==(UserInfo& other) {
            return (
                (name == other.name) && 
                (pass == other.pass)
                );
        }
    };      

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
        Uri(Uri&&); // Move Constructor that transfers resources from an expiring object to a newly constructed object.
        Uri& operator=(const Uri&) = delete; //Copy Assignment Operation That assigns the values of one object to another object using the assignment operator (=)
        Uri& operator=(Uri&&); //Move Assignment Operator: Amove assignment operator efficiently transfers resources from one object to another.
        
        //public methods
    public:
        /**
         * This is the default constructor
        */
       Uri();

        /**
         * This is the equality comparison operator for the class
         * 
         * @param[in] otherUri
         *      This is the other URI to which to compare this URI.
         * @return
         *      An indication of whether or not this URI is equal to the other one
        */
       bool operator==(const Uri& otherUri) const;
        /**
         * This is the inequality comparison operator for the class
         * 
         * @param[in] otherUri
         *      This is the other URI to which to compare this URI.
         * @return
         *      An indication of whether or not this URI is equal to the other one
        */
       bool operator!=(const Uri& otherUri) const;
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
         *      as a sequence of segments.
        */
       std::vector<std::string> GetPath() const;

       /**
        * This method returns an indication of whether or not the URI
        * includes a port number.
        * 
        * @return
        *       true if the URI includes a port number else false.
       */
       bool HasPort() const;


        /**
         * This method returns the port number of the UrI if it exists.
         * 
         * @return
         *      The port number element of the URI 
         * 
         * @note
         *      The returned port number is only valid if the
         *      HasPort method returns true.
        */
       uint16_t GetPort() const;

        /**
         * This methode returns an indication of whether or not 
         * the URI is a relative reference.
         * 
         * @return
         *      An indication of wheter or not the URI is a relative reference.
         *      return true if is a relative reference else false.
        */
       bool IsRelativeReference() const;


        /**
         * This methode returns an indication of whether or not 
         * the URI is a relative path.
         * 
         * @return
         *      An indiction of whether or not the URI is a relative path.
         *      return true if is a relative path else false
         * 
        */
       bool HasRelativePath() const;


        /**
         * This methode returns the query element of the Uri string if it exists.
         * 
         * @return
         *      return the query item if it exist
        */
       std::string GetQuery() const;

        /**
         * This method returns the fragment of the Uri string if it exists.
         * 
         * @return
         *      return the fragment if it exist
        */
       std::string GetFragment() const;


        /**
         * This method returns the User information element.
         * 
         * @return
         *      return the user name and password structure
        */
       UserInfo GetUserInfo() const;

        /**
         * This method applies the "remove_dot_segment" routine according to 
         * the RFC 3986 to the path segments of the URI, in order to normalize it.
         * (apply and remove "." and ".." segments).
        */
       void NormalizePath(); 

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

#endif /*URI_HPP*/
