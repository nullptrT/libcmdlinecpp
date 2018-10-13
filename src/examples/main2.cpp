/*
    LibCmdLineC++: A simple parser for parsing command line arguments with C++
    Copyright (C) 2018 Sebastian Lau <lauseb644@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 3 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
    USA
*/


#include "config.hpp"

#include "cmdline.hpp"

#include <iostream>


int main( int argc, char** argv ) {

    // Define the program
    cmdline.set_program_name( "cmdlinecpp_test2" );
    cmdline.set_program_description( "A simple test with old-styled functions for LibCmdLineC++." );
    cmdline.set_program_version( std::string(LIBCMDLINECPP_VERSION) );
    
    // Add a positional argument with help text that is required to run the program.
    cmdline.define_option( cmdlinecpp::Option("PATH", "The path to the file to operate on.") );
    
    // Define another optional Option. Optional options have an additional short parameter.
    cmdlinecpp::Option option_iterations
        = cmdlinecpp::Option( "iterations", "n"
                            , "The number of iterations the program makes."
                            , cmdlinecpp::Data::Int);
    // Add some more help text to the option.
    option_iterations << "This is needed for testing purposes.";
    // Add the new option to the command line parser.
    cmdline.define_option( option_iterations );
        
    // Parse the parameters passed to main.
    try {
        if ( !cmdline.parse( argc, argv ) ) { // Alternative definition of CmdLineParser::parse
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
    cmdline.to_variable( "PATH", path );
    std::string path2 = cmdline["path"];
    
    int n = 1;
    // Since the option "iterations" is optional, you should guard its access with an if statement
    if ( cmdline.is_specified("iterations") ) {
        cmdline.to_variable( "iterations", n );
    }
    

    for ( unsigned int i = 0; i < n; i++ ) {
        // DO SOMETHING ELSE IN YOUR PROGRAM
    }
    
    
    return 0;
}
