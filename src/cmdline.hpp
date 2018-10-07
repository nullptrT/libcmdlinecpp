/**
 * @file cmdline.hpp
 * @brief A global singleton for parsing the command line arguments
 * @author Sebastian Lau <lauseb644 [at] gmail [dot] com>
 **/
/*
    LibCmdLineC++: A simple parser for command line arguments with C++
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

#include <map>
#include <memory>

#include "config.hpp"

#include "arguments.hpp"
#include "parameters.hpp"


namespace cmdlinecpp {

/**
 * @brief A global singleton that defines the command line arguments and is able to parse the parameters passed to a main function.
 */
class CmdLineInterface
{
    Data& m_current_option_requested;
protected:
    /**
     * @brief The definitions of arguments that have to be passed to the program
     * @note see CmdLineArguments for further information
     */
    CmdLineArguments* m_cmdline_arguments;
    
    /**
     * @brief The number of arguments found on command line for this execution of the binary
     */
    int m_argc;
    /**
     * @brief The parameters, that are passed to the binary, as list of strings
     */
    std::vector< std::string > m_argv;
    
    /**
     * @brief The parameters passed to the program ordered by its option key
     * @note see CmdLineParameters for further information
     */
    CmdLineParameters* m_cmdline_parameters;
    
    /**
     * @brief The name of the program, e.g. the binary's name
     */
    std::string m_program_name;
    /**
     * @brief The description of the program that can be printed in the help text
     */
    std::string m_program_description;
    /**
     * @brief The version of the program that can be printed in the help text
     */
    std::string m_program_version;
    /**
     * @brief A list of string that contain usage examples
     */
    std::vector< std::string > m_usage_examples;

public:

	/**
	 * @brief Get a reference to the cmdlineparser object
	 */
	static CmdLineInterface& get();

    /**
     * @brief Defines a new option to the command line interface
     * @param option The new option
     * @returns A reference to this object
     */
    CmdLineInterface& operator<<( Option option );
    /**
     * @brief Pass the number of command line arguments from a main function to the command line interface
     * @param argc The parameter argc as it can be received from the main function's parameters'
     * @returns A reference to this object
     * @note You can also pass argc later with the parse function
     */
    CmdLineInterface& operator<<( int argc );
    /**
     * @brief Pass the argv array of the command line arguments from a main function to the command line interface
     * @param argv The parameter argv as it can be received from the main function's parameters
     * @returns A reference to this object
     * @note You can also pass argc later with the parse function
     */
    CmdLineInterface& operator<<( char** argv );
    /**
     * @brief Defines a new option to the command line interface without streaming operator
     * @param option The new option
     */
    void define_option( Option option );
    
    /**
     * @brief Parse all command line parameters according to the specified options
     * @param argc The parameter argc as it can be received from the main function's parameters
     * @param argv The parameter argv as it can be received from the main function's parameters
     * @returns False, if there are not parameters to parse as the argv parameter of the main function was not passed to the command line interface yet.
     * @throws std::out_of_range If no parameter is specified for an argument or if not all positional arguments of the command line interface are specified
     * @note After parsing the command line parameters into this command line interface the values can be received with the operator>> and the to_variable functions.
     */
    bool parse();
    /**
     * @brief Parse all command line parameters according to the specified options
     * @returns False, if there are not parameters to parse as the argv parameter of the main function was not passed to the command line interface yet.
     * @throws std::out_of_range If no parameter is specified for an argument or if not all positional arguments of the command line interface are specified
     * @note After parsing the command line parameters into this command line interface the values can be received with the operator>> and the to_variable functions.
     */
    bool parse( int argc, char** argv );
    
    /**
     * @brief Get the value for an argument
     * @param key The name of the argument to look up
     * @returns The value found for the argument as string or an empty string, if no argument for key exists
     * @note You should have added options to this command line interface and have parsed the command line parameters first
     */
    const std::string operator[]( const std::string key ) const;
    /**
     * @brief Test, if an argument specified by key is specified by this command line interface
     * @param key The name of the argument to look for
     * @returns True, if the an option with the name key is found in this command line interface
     * @note You should have added options to this command line interface and have parsed the command line parameters first
     */
    bool is_specified( const std::string key ) const;
    /**
     * @brief Get the data type used by an option that is specified for this command line interface
     * @param key The name of the argument to look for
     * @returns The Data::Type used by the option looked up or Data::Type::Void, if the option was not found in this command line interface
     */
    const Data::Type dataTypeOfOption( const std::string key ) const;
    
    /**
     * @brief Set the name of the program, e.g. the binary's name
     * @param program_name The name of the program
     */
    void set_program_name( const std::string program_name );
    /**
     * @brief Set the description of the program that can be printed in the help text
     * @param description The description of the program
     */
    void set_program_description( const std::string program_description );
    /**
     * @brief Set the version of the program that can be printed in the help text
     * @param version The version of the program
     */
    void set_program_version( const std::string program_version );
    /**
     * @brief Define the program
     * @param program_name The name of the program
     * @param program_description The description of the program
     * @param program_version The version of the program
     */
    void define_program( const std::string program_name
                       , const std::string program_description
                       , const std::string program_version );
    /**
     * @brief Add an usage example for this program that can be printed in the help
     * @param usage_example An usage example that describes one of the standard usages of the program
     */
    void add_usage_example( const std::string usage_example );
    /**
     * @brief Print a formatted help
     */
    void print_help() const;
    
    /**
     * @brief Specify the requested data's key and occurence to get as next output of the operator>> functions
     * @param data_requested A Data that specifies at least a key and optionally a appearence number, if the parameter is to be passed multiple times to the program
     */
    CmdLineInterface& operator>>( Data data_requested );
    /**
     * @brief Get the value of the last requested Data to a bool reference
     * @param value The reference to the bool variable to write the value to
     * @throws TypeError If the Data::Type of the parameter is not bool or Data was not specified properly for requesting a value
     */
    CmdLineInterface& operator>>( bool& value );
    /**
     * @brief Get the value of the last requested Data to a double reference
     * @param value The reference to the double variable to write the value to
     * @throws TypeError If the Data::Type of the parameter is not double or Data was not specified properly for requesting a value
     */
    CmdLineInterface& operator>>( double& value );
    /**
     * @brief Get the value of the last requested Data to a long reference
     * @param value The reference to the long variable to write the value to
     * @throws TypeError If the Data::Type of the parameter is not long or Data was not specified properly for requesting a value
     */
    CmdLineInterface& operator>>( long& value );
    /**
     * @brief Get the value of the last requested Data to a int reference
     * @param value The reference to the int variable to write the value to
     * @throws TypeError If the Data::Type of the parameter is not int or Data was not specified properly for requesting a value
     */
    CmdLineInterface& operator>>( int& value );
    /**
     * @brief Get the value of the last requested Data to a short reference
     * @param value The reference to the short variable to write the value to
     * @throws TypeError If the Data::Type of the parameter is not short or Data was not specified properly for requesting a value
     */
    CmdLineInterface& operator>>( short& value );
    /**
     * @brief Get the value of the last requested Data to a std::string reference
     * @param value The reference to the std::string variable to write the value to
     * @throws TypeError If the Data::Type of the parameter is not std::string or Data was not specified properly for requesting a value
     */
    CmdLineInterface& operator>>( std::string& value );
    /**
     * @brief Get the value of the last requested Data to a unsigned long reference
     * @param value The reference to the unsigned long variable to write the value to
     * @throws TypeError If the Data::Type of the parameter is not unsigned long or Data was not specified properly for requesting a value
     */
    CmdLineInterface& operator>>( unsigned long& value );
    /**
     * @brief Get the value of the last requested Data to a unsigned int reference
     * @param value The reference to the unsigned int variable to write the value to
     * @throws TypeError If the Data::Type of the parameter is not unsigned int or Data was not specified properly for requesting a value
     */
    CmdLineInterface& operator>>( unsigned int& value );
    /**
     * @brief Get the value of the last requested Data to a unsigned short reference
     * @param value The reference to the unsigned short variable to write the value to
     * @throws TypeError If the Data::Type of the parameter is not unsigned short or Data was not specified properly for requesting a value
     */
    CmdLineInterface& operator>>( unsigned short& value );
    
    /**
     * @brief Write the parameter's value to a bool reference
     * @param key The name of the option to look up
     * @param value The reference to the bool variable to write the value to
     * @throws TypeError If the Data::Type of the parameter is not bool
     * @note Use this as alternative to the default streaming API's operator>>
     */
    void to_variable( const std::string key, bool& value );
    /**
     * @brief Write the parameter's value to a bool reference
     * @param key The name of the option to look up
     * @param value The reference to the bool variable to write the value to
     * @throws TypeError If the Data::Type of the parameter is not bool
     * @note Use this as alternative to the default streaming API's operator>>
     */
    void to_variable( const std::string key, double& value );
    /**
     * @brief Write the parameter's value to a long reference
     * @param key The name of the option to look up
     * @param value The reference to the long variable to write the value to
     * @throws TypeError If the Data::Type of the parameter is not long
     * @note Use this as alternative to the default streaming API's operator>>
     */
    void to_variable( const std::string key, long& value );
    /**
     * @brief Write the parameter's value to a int reference
     * @param key The name of the option to look up
     * @param value The reference to the int variable to write the value to
     * @throws TypeError If the Data::Type of the parameter is not int
     * @note Use this as alternative to the default streaming API's operator>>
     */
    void to_variable( const std::string key, int& value );
    /**
     * @brief Write the parameter's value to a short reference
     * @param key The name of the option to look up
     * @param value The reference to the short variable to write the value to
     * @throws TypeError If the Data::Type of the parameter is not short
     * @note Use this as alternative to the default streaming API's operator>>
     */
    void to_variable( const std::string key, short& value );
    /**
     * @brief Write the parameter's value to a std::string reference
     * @param key The name of the option to look up
     * @param value The reference to the std::string variable to write the value to
     * @throws TypeError If the Data::Type of the parameter is not std::string
     * @note Use this as alternative to the default streaming API's operator>>
     */
    void to_variable( const std::string key, std::string& value );
    /**
     * @brief Write the parameter's value to a unsigned long reference
     * @param key The name of the option to look up
     * @param value The reference to the unsigned long variable to write the value to
     * @throws TypeError If the Data::Type of the parameter is not unsigned long
     * @note Use this as alternative to the default streaming API's operator>>
     */
    void to_variable( const std::string key, unsigned long& value );
    /**
     * @brief Write the parameter's value to a unsigned int reference
     * @param key The name of the option to look up
     * @param value The reference to the unsigned int variable to write the value to
     * @throws TypeError If the Data::Type of the parameter is not unsigned int
     * @note Use this as alternative to the default streaming API's operator>>
     */
    void to_variable( const std::string key, unsigned int& value );
    /**
     * @brief Write the parameter's value to a unsigned short reference
     * @param key The name of the option to look up
     * @param value The reference to the unsigned short variable to write the value to
     * @throws TypeError If the Data::Type of the parameter is not unsigned short
     * @note Use this as alternative to the default streaming API's operator>>
     */
    void to_variable( const std::string key, unsigned short& value );
    
    
private:
	static std::unique_ptr< CmdLineInterface > ms_cmdlineparser;

	CmdLineInterface();
	CmdLineInterface( CmdLineInterface const& another) = delete;
	CmdLineInterface& operator=( CmdLineInterface const& another );
};

} // namespace cmdlinecpp

/**
 * @def cmdline
 * @brief Macro that can be used to access the global command line parser singleton
 */
#define cmdline cmdlinecpp::CmdLineInterface::get()
