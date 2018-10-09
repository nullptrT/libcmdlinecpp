# LibCmdLineC++
##### A simple parser for command line arguments with C++
###### v0.1.0

This library is a simple and intuitive frontend to libstdc++ adding a simple command line parser so that you don't need to write it yourself. It has a simple streaming API and some old-fashioned functions.

This library aims to be simple, but highly usable and customizable without having a bunch of other dependencies, libraries or code.

For examples, look below on this page.

* [Online api documentation](https://doc.0ptr.de/libcmdlinecpp/annotated.html)
* Packages can be found for ArchLinux in [Arch User Repository](https://aur.archlinux.org/libcmdlinecpp)
* How to use it is documented lower on this page.

If you wrote additional datastructures or functions and you think it could be useful within this library: You are welcome to create a pull request or contact me ( lauseb644 _at_ gmail _dot_ com ).

More features can be requested through issues on [GitHub](https://github.com/nullptrT/libcmdlinecpp)


#### Currently supported features

* Defining the command line arguments and options.
* Parsing the command line parameters provided when running the program.
* A list as last positional argument like `program_name <POSITIONAL1> <POSITIONAL2...>`
* Getting command line parameters as different data types into variables.
* Printing the help.

## Future features

* An action without parameter as first argument like `program_name action [OPTION...]`


## License:

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.

* Copyright (C) 2018 Sebastian Lau <lauseb644@gmail.com>

## Usage

The included files in the directory `src/excamples` can be used and compiled as an example for the library's usage. For a more detailed reference, please read the following short documentation.

In most cases you will need to use the class macro `cmdline` by including `cmdline/cmdline.hpp`. It gives you the ability to define the parameter environment for the program and define the help contents.

The basic usage of libcmdlinecpp is demonstrated in `main.cpp` (see next code example), an equivalent usage with more old-fashioned functions instead of the streaming API can be found below.

```c++
#include <cmdlinecpp/cmdline.hpp>
#include <iostream>

int main( int argc, char** argv ) {

    // Define the program
    cmdline.set_program_name( "cmdlinecpp_test" );
    cmdline.set_program_description( "A simple test for LibCmdLineC++." );
    cmdline.set_program_version( std::string(LIBCMDLINECPP_VERSION) );
    
    // Add a positional argument with help text that is required to run the program.
    cmdline << cmdlinecpp::Option("PATH", "The path to the file to operate on.");
    
    // Define another optional Option. Optional options have an additional short parameter.
    cmdlinecpp::Option option_iterations
        = cmdlinecpp::Option( "iterations", 'n'
                            , "The number of iterations the program makes."
                            , cmdlinecpp::Data::Int );
    // Add some more help text to the option.
    option_iterations << "This is needed for testing purposes.";
    // Add the new option to the command line parser.
    cmdline << option_iterations;
    
    // Specify the main function's parameters to the command line parser.
    cmdline << argc << argv;
    
    // Parse the parameters passed to main.
    try {
        if ( !cmdline.parse() ) {
            // This happens, when there are no parameters to parse
            cmdline.print_help();
            // As an alternative you can specify the program's default behavior without options here.
            return 1;
        }
    } catch ( std::out_of_range& oor ) {
        // This happens, when not all positional arguments like "PATH" are specified
        // or a string option does not provide a string as parameter
        std::cerr << "ERROR: " << oor.what() << std::endl;
        cmdline.print_help();
        return 1;
    }
    
    // If the help option is specified, this function will print the help and exit with 0
    cmdline.print_help_if_requested();
        
    // DO SOMETHING IN YOUR PROGRAM
    
    // ACCESS THE VALUES OF THE PARAMETERS WITH DIFFERENT DATA TYPES
    // Required parameters can be safely accessed as they need to be specified
    std::string path;
    cmdline >> cmdlinecpp::Data("PATH") >> path;
    std::string path2 = cmdline["PATH"];
    
    int n = 1;
    // Since the option "iterations" is optional, you should guard its access with an if statement
    if ( cmdline.is_specified("iterations") ) {
        cmdline >> cmdlinecpp::Data("iterations") >> n;
    }
    
    for ( unsigned int i = 0; i < n; i++ ) {
        // DO SOMETHING ELSE IN YOUR PROGRAM
    }
    
    
    return 0;
}
```

The same functionality can be provided by not using the streaming API of libcmdlinecpp. This is demonstrated in `main2.cpp` (see next code example). Both API definitions can be used together, it does not matter, that you use some old-fashioned functions beside the streaming API. It is recommended to use the API with streaming.

```c++
    cmdline << cmdlinecpp::Option("PATH", "The path to the file to operate on.");
    // is equivalent to
    cmdline.define_option( cmdlinecpp::Option("PATH", "The path to the file to operate on.") );

    cmdline << argc << argv;
    cmdline.parse();
    // is equivalent to
    cmdline.parse( argc, argv );
    
    std::string path;
    cmdline >> cmdlinecpp::Data("path") >> path;
    // is equivalent to
    cmdline.to_variable( "path", path );
```




#### Version checking

For checking the version of libcmdlinecpp at compile time without CMake you could have a `cmdline.cpp` file compiled like this:

```c++
#include <cmdlinecpp/cmdlinecppversion.hpp>

#if LIBCMDLINECPP_VERSION < 100000
#error LibCmdLineC++ needs to be at least at version 1.0.0
#else
#pragma message ("Found LibCmdLineC++ at required version 1.0.0 or more")
#endif
```


## Building

You can simply build this with a compiler that supports C++11:

```
 % cd /path/to/clone/in
 % git clone https://github.com/nullptrT/libcmdlinecpp.git
 % mkdir libcmdlinecpp/build # or some other build directory
 % cd libcmdlinecpp/build
 % cmake .. # You may add CMake options here (see below)
 % make
```

On windows you may use the `cmake-gui` or `qtcreator` to create build files for MSVC 2015 or later.


#### CMake options

Additionally to the default options CMake currently offers the following options (can be specified with `-DOPTION=ON`):

* `CMDLINECPP_SHARED`: Enables building a shared library `libcmdlinecpp.so`. Not using this option builds a static `libcmdlinecpp.a` by default.
* `BUILD_CMDLINECPP_TESTS`: Build simple main runtimes that demonstrate current features of libcmdlinecpp.
* `CMDLINECPP_DESTDIR`: Where are the files installed to. Defaults to `CMAKE_INSTALL_PREFIX` (`/usr` on UNIX and `c:/Program Files` on WIN32)
* `CMDLINECPP_HEADER_INSTALL_DIR`: Can be set to control, where headers are installed. Defaults to `CMDLINECPP_DESTDIR/include/libcmdlinecpp`.
* `CMDLINECPP_LIB_INSTALL_DIR`: Can be set to control where the library is installed. Defaults to `CMDLINECPP_DESTDIR/lib`.
* `CMDLINECPP_INSTALL_LIBS`: Enables targets for installation of library files. Because it is useful not to install the library (e.g. when used as submodule of a project) this defaults to off. If enabled, it installs all headers to `CMDLINECPP_HEADER_INSTALL_DIR` and the library to `CMDLINECPP_LIB_INSTALL_DIR`

#### Requirements

* cmake>=3.0
* a c++ compiler supporting c++11 (tested for gcc)
* doxygen (optional for building the [html documentation](https://doc.0ptr.de/libcmdlinecpp/))


## Use with CMake


The included `LibLogCPPConfig.cmake` module enables you to use `find_package(LibLogCPP >= 1.6)` in CMake:
* `LIBLOGCPP_INCLUDE_DIR`: The directory, where the headers are located in `liblogcpp/` (usually `/usr/include`)
* `LIBLOGCPP_LIBRARY`: The static library you can link into your program. It's only useful, if you use `stdlog`
* `LIBLOGCPP_VERSION_STRING`: The current version of liblogcpp.


