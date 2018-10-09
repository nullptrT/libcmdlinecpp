/**
 * @file parameters.hpp
 * @brief A file that defines a map with the parameter name and value
 * @author Sebastian Lau <lauseb644 [at] gmail [dot] com>
 **/
/*
    LibCmdLineC++: A simple parser for command line options with C++
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

#pragma once

#include "arguments.hpp"

#include <map>
#include <string>


namespace cmdlinecpp {


/**
 * @brief A class that holds the parameters specified for options specified in a CmdLineOptions class
 */
class CmdLineParameters {
protected:
    /**
     * @brief The parameters that where parsed. The first is the option name that can be looked up and the second is the parameter specified for this option
     */
    std::map< std::string, std::string > m_parameters;
    /**
     * @brief The last positional parameters as list
     */
    std::vector< std::string > m_last_positionals_list;
    
public:
    /**
     * @brief Constructor. Constructs a class with no parameters specified
     */
    CmdLineParameters();
    /**
     * @brief Constructor. Constructs a class with option keys specified by an existing CmdLineArguments environment
     */
    CmdLineParameters( const CmdLineArguments* cl_args );
    
    /**
     * @brief Add a new key to this class. Its parameter element will be an empty string until specified.
     * @param option_name Name of the option
     */
    void add_option_key( const std::string option_name );
    
    /**
     * @brief Set the parameter element of a given key to a value
     * @param option_name The name of the option to set the parameter for
     * @param parameter The value of the option
     * @throws std::out_of_range If the option key was not found in this class
     */
    void set( const std::string option_name
            , const std::string parameter );
    
    /**
     * @brief Get the value for an option
     * @param option_name Parameter to look up
     * @returns A string containing the value of the option key. Returns an empty string, if the option for option_name is not found in this class or no value is set for this key
     */
    const std::string get( const std::string option_name ) const;
    
    /**
     * @brief Set last positional strings from command line parameters
     * @param last_positionals The strings that are defined for the last postitional argument
     */
    void set_last_positionals( std::vector< std::string > last_positionals_list );
    /**
     * @brief Get the list of the last positional parameters
     * @returns A vector containing strings with the last positional parameters
     */
    std::vector< std::string > get_last_positionals() const;
    
    /**
     * @brief Test, if an option has an value specified
     * @param option_name The name of the option to look up
     * @returns True, if a value is specified for option_name
     */
    bool has_value( const std::string option_name ) const;
    /**
     * @brief Test, if a key for option_name was specified in this class
     * @param option_name The name of the option to look up
     * @returns True, if a option_name is specified as key in this class
     */
    bool is_specified( const std::string option_name ) const;
    
    /**
     * @brief Get the number of options specified
     * @returns The number of options specified
     */
    const size_t size() const;
    /**
     * @brief Reset this class to contain no option keys
     */
    void clear();
};


} // namespace cmdlinecpp
