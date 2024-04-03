# Uri
[RFC 3986](https://datatracker.ietf.org/doc/html/rfc3986) "Uniform Resource Identifier (URI): Generic Syntax".


* A Uniform Resource Identifier is a unique sequence of characters that identifies an abstract or physical resource, such as resources on a webpage, mail address, phone number, books, real-world objects such as people and places, concepts.
The strings of characters incorporated in a URI serve as identifiers, such as a scheme name and a file path.

### __Example__

        http://www.example.com:8080/over/there?name=ferret#nose
        \_/   \___________________/\_________/ \_________/ \__/
        |           |                  |            |       |
    scheme     authority              path         query   fragment
        |_   __________________________|__
        / \ /                             \
        urn:example:animal:ferret:nose

### Scheme
Within the URI, the first element is the scheme name. Schemes are case-insensitive and separated from the rest of the object by a colon. The scheme establishes the concrete syntax and associated protocols for the URI.

### Authority
The URI's authority component is made up of multiple parts: a host consisting of either a registered name or an IP address, an optional authentication section and an optional port number.

### Query (optional)
The query contains a string of non-hierarchical data. It is often a sequence of attribute-value pairs separated by a delimiter, such as an ampersand (&) or semicolon. A question mark separates the query from the part that comes before it.

### Fragment (optional)
The fragment contains an identifier that provides direction to a secondary resource. It is separated from the preceding part of the URI by a hash (#).

## Building the C++ Implementation

A portable library is built which depends only on the C++11 compiler and
standard library, so it should be supported on almost any platform.  The
following are recommended toolchains for popular platforms.

* Windows -- [Visual Studio](https://www.visualstudio.com/) (Microsoft Visual
  C++)
* Linux -- clang or gcc
* MacOS -- Xcode (clang)

This library is not intended to stand alone.  It is intended to be included in
a larger solution which uses [CMake](https://cmake.org/) to generate the build
system and build applications which will link with the library.

There are two distinct steps in the build process:

1. Generation of the build system, using CMake
2. Compiling, linking, etc., using CMake-compatible toolchain

### Prerequisites

* [CMake](https://cmake.org/) version 3.8 or newer
* C++11 toolchain compatible with CMake for your development platform (e.g.
  [Visual Studio](https://www.visualstudio.com/) on Windows)

### Build system generation

Generate the build system using [CMake](https://cmake.org/) from the solution
root.  For example:

```bash
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A "x64" ..
```

### Compiling, linking, etc...

Either use [CMake](https://cmake.org/) or your toolchain's IDE to build.
For [CMake](https://cmake.org/):

```bash
cd build
cmake --build . --config Release
```

## License

Licensed under the [MIT license](LICENSE.txt).