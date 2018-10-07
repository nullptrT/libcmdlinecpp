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
 * @brief A global singleton logger with severities that has a console and a file channel
 */
class CmdLineParser
{
private:
	static std::unique_ptr< CmdLineParser > ms_cmdlineparser;

	CmdLineParser();
	CmdLineParser( CmdLineParser const& another) = delete;
	CmdLineParser& operator=( CmdLineParser const& another );

protected:
    CmdLineArguments* m_cmdline_arguments;
    
    int m_argc;
    std::vector< std::string > m_argv;
    
    CmdLineParameters* m_cmdline_parameters;
    
    Data& m_current_option_requested;
    
    std::string m_program_name;
    std::string m_description;
    std::string m_version;
    std::vector< std::string > m_usage_examples;

public:

	/**
	 * @brief Get a reference to the cmdlineparser object
	 */
	static CmdLineParser& get();

    void define_option( Option option );
	CmdLineParser& operator<<( Option option );
    CmdLineParser& operator<<( int argc );
    CmdLineParser& operator<<( char** argv );
    
    bool parse();
    bool parse( int argc, char** argv );
    
    const std::string operator[]( const std::string key ) const;
    bool is_specified( const std::string key ) const;
    const Data::Type dataTypeOfOption( const std::string key ) const;
    
    void set_program_name( const std::string program_name );
    void set_description( const std::string description );
    void set_version( const std::string version );
    void add_usage_example( const std::string usage_example );    
    void print_help() const;
    
    void to_variable( const std::string key, bool& value );
    void to_variable( const std::string key, double& value );
    void to_variable( const std::string key, long& value );
    void to_variable( const std::string key, int& value );
    void to_variable( const std::string key, short& value );
    void to_variable( const std::string key, std::string& value );
    void to_variable( const std::string key, unsigned long& value );
    void to_variable( const std::string key, unsigned int& value );
    void to_variable( const std::string key, unsigned short& value );
    
    CmdLineParser& operator>>( Data option_requested );
    CmdLineParser& operator>>( bool& value );
    CmdLineParser& operator>>( double& value );
    CmdLineParser& operator>>( long& value );
    CmdLineParser& operator>>( int& value );
    CmdLineParser& operator>>( short& value );
    CmdLineParser& operator>>( std::string& value );
    CmdLineParser& operator>>( unsigned long& value );
    CmdLineParser& operator>>( unsigned int& value );
    CmdLineParser& operator>>( unsigned short& value );
};

} // namespace cmdlinecpp

/**
 * @def cmdline
 * @brief Macro that can be used to access the global command line parser singleton
 */
#define cmdline cmdlinecpp::CmdLineParser::get()
