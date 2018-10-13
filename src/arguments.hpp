/**
 * @file arguments.hpp
 * @brief A file that defines types of command line arguments
 * @author Sebastian Lau <lauseb644 [at] gmail [dot] com>
 **/
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

#pragma once

#include "actions.hpp"

#include <stdexcept>
#include <string>
#include <utility>
#include <vector>


namespace cmdlinecpp {


/**
 * @def TRUE
 * @brief Macro that represents the string value "True"
 */
#define TRUE "True"
/**
 * @def FALSE
 * @brief Macro that represents the string value "False"
 */
#define FALSE "False"


/**
 * @brief Class to request data of an option.
 */
class Data {
    std::string m_key;
    unsigned int m_appearence;
    
public:
    /**
     * @brief Default constructor.
     */
    Data();
    /**
     * @brief Constructor.
     * @param key The name of the option to request
     */
    Data( const std::string key );
    /**
     * @brief Constructor.
     * @param key The name of the option to request
     * @param appearence The n-th appearence of this option is requested
     * @warning Not needed at the moment
     */
    Data( const std::string key
        , const unsigned int appearence );
    
    /**
     * @brief This class is valid, if it is not default constructed
     * @returns True, if at least the key is set
     */
    bool is_valid() const;
    /**
     * @brief Get the name of the requested option
     * @returns The name of the requested option
     */
    const std::string key() const;
    /**
     * @brief Get the number of the requested occurrence.
     * @returns A number, not an index
     */
    const unsigned int appearence() const;
       
    /**
     * @brief An enum with all data types supported by this library.
     * @note Type::Void is equivalend to no data type
     */
    enum Type {
        Bool,
        Double,
        Long,
        Int,
        Short,
        String,
        UnsignedLong,
        UnsignedInt,
        UnsignedShort,
        Void
    };
};


/**
 * @brief Defines a simple argument for command line
 */
class Option {
protected:
    /**
     * @brief The pair that defines the option. The first element is the name, the second can be an empty string or a string with one element
     */
    std::pair< const std::string, const std::string > m_option;
    /**
     * @brief The help text for this Option
     */
    std::string m_help_text;
    /**
     * @brief The data type this option has as parameter
     */
    Data::Type m_data_type;
    
public:
    /**
     * @brief Constructor for positional argument options.
     * @param option_long The name of this positional argument option
     * @param help_text The help text of this positional argument option. Defaults to an empty string.
     * @param data_type The Data::Type of this positional argument. Defaults to Data::Type::String.
     */
    Option( const std::string option_long
          , const std::string help_text
          , Data::Type data_type = Data::Type::String );
    /**
     * @brief Constructor for regular options.
     * @param option_long The name and long option specifier of this regular option
     * @param option_short The short option specifier of this regular option (1-3 character)
     * @param help_text The help text of this regular option. Defaults to an empty string.
     * @param data_type The Data::Type of this regular argument. Defaults to Data::Type::String.
     * @throws logic_error If option_short is an empty character
     */
    Option( const std::string option_long
          , const std::string option_short
          , const std::string help_text
          , Data::Type data_type = Data::Type::String );
    
    /**
     * @brief Test, if this option is a regular or a positional option
     * @returns True, if no short option specifier was passed to the constructor
     */
    bool is_positional() const;
    /**
     * @brief Test, if this option is a positional or a regular option
     * @returns True, if no short option specifier was passed to the constructor
     */
    bool is_regular() const;
    /**
     * @brief Test, if a help text was specified
     * @returns True, if the help text contains at least one character
     */
    bool has_help_text() const;
    
    /**
     * @brief Get the data type this option has as a parameter
     * @returns The data type this option has as a parameter
     */
    const Data::Type dataType() const;
    /**
     * @brief Get the name of this option and the long specifier for regular options
     * @returns The string passed as parameter option_long to the constructor
     */
    const std::string option() const;
    /**
     * @brief Get the short specifier of this regular option
     * @returns The 1-3 characters passed as parameter option_short to the constructor
     * @warning Returns an empty string on positional argument options
     */
    const std::string option_short() const;
    /**
     * @brief Get the help text
     * @returns The help text
     */
    const std::string help_text() const;
    
    /**
     * @brief Add a string to the help text.
     * @param help_text The text to add to the current help text
     * @note Linebreaks will be removed
     */
    Option& operator<<( const std::string help_text );
};


/**
 * @brief A class that specifies and holds the required and optional arguments of a command line call to a program
 */
class CmdLineArguments {
protected:
    /**
     * @brief All possible actions
     */
    std::vector< Action > m_actions;
    /**
     * @brief All positional arguments
     */
    std::vector< Option > m_positionals;
    /**
     * @brief All optional argument options
     */
    std::vector< Option > m_options;
    
    /**
     * @brief Add a positional option
     * @param option The option to add
     */
    void add_option_positional( Option option );
    /**
     * @brief Add a regular option
     * @param option The option to add
     */
    void add_option_regular( Option option );
public:
    /**
     * @brief Default constructor. Constructs an command line specifier with no arguments
     */
    CmdLineArguments();
        
    /**
     * @brief Add a positional option
     * @param option The option to add
     */
    void add_option( Option option );
    /**
     * @brief Add a possible action
     * @param action The action to add
     */
    void add_action( Action action );
    
    /**
     * @brief Look up, if the requested option is regular
     * @param parameter Parameter that can be the long or short option specifier or some string that is not known
     * @returns True, if the option begins with one or two '-' and parameter is specified in one Option that is already specified in this CmdLineArguments
     */
    bool is_option_regular( const std::string parameter ) const;
    /**
     * @brief Look up, if the requested option is regular
     * @param parameter Name of the positional option or some string that is not known
     * @returns True, if the option does not begin with one '-' and parameter is specified in one Option that is already specified in this CmdLineArguments
     */
    bool is_option_positional( const std::string parameter ) const;
    /**
     * @brief Test, if actions are enabled
     * @returns True, if actions are enabled
     */
    bool are_actions_enabled() const;
    /**
     * @brief Look up, if the requested option is an action
     * @param parameter Name of the action or some string that is not known
     * @returns True, if the parameter is known as an action.
     */
    bool is_action( const std::string parameter ) const;
    
    /**
     * @brief Find a positional option by its name
     * @param option_name The name to look for
     * @returns The option found
     * @throws NotFound Only if the option is not found
     */
    const Option lookup_positional( const std::string option_name ) const;
    /**
     * @brief Find a regular option by its name or short option
     * @param parameter The name or short parameter to look for
     * @returns The option found
     * @throws NotFound Only if the option is not found
     */
    const Option lookup_option( const std::string parameter ) const;
    
    /**
     * @brief Get a copy of the vector with all positional options specified
     * @returns A copy of the vector with all positional options specified
     */
    std::vector< Option > options_positional() const;
    /**
     * @brief Get a copy of the vector with all regular options specified
     * @returns A copy of the vector with all regular options specified
     */
    std::vector< Option > options_regular() const;
    /**
     * @brief Get the possible actions that are defined for this command line arguments
     * @returns A list of possible actions
     */
    std::vector< Action > actions() const;
    
    /**
     * @brief Exception that is to be thrown, when something in this list is not found
     */
    class NotFound
        :   std::out_of_range
    {
    public:
        /**
         * @brief Constructor
         * @param what_arg A string that describes, what was not found
         */
        NotFound( const std::string& what_arg );
    };
};

} // namespace cmdlinecpp
