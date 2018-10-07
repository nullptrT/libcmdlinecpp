/**
 * @file arguments.hpp
 * @brief A file that defines types of command line arguments
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

#include <stdexcept>
#include <string>
#include <utility>
#include <vector>


namespace cmdlinecpp {


#define TRUE "True"
#define FALSE "False"


class Data {
    std::string m_key;
    unsigned int m_appearence;
    
public:
    Data();
    Data( const std::string key );
    Data( const std::string key
        , const unsigned int appearence );
    
    bool is_valid() const;
    const std::string key() const;
    const unsigned int appearence() const;
        
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


class Option {
protected:
    std::pair< const std::string, const std::string > m_option;
    std::string m_help_text;
    Data::Type m_data_type;
    
public:
    Option( const std::string option_long
          , const std::string help_text = std::string("")
          , Data::Type data_type = Data::Type::String );
    Option( const std::string option_long
          , const char option_short
          , const std::string help_text = std::string("")
          , Data::Type data_type = Data::Type::String );
    
    bool is_positional() const;
    bool has_help_text() const;
    
    const Data::Type dataType() const;
    const std::string option() const;
    const char option_short() const;
    const std::string help_text() const;
    
    Option& operator<<( const std::string help_text );
};


class CmdLineArguments {
protected:
    std::vector< Option > m_positionals;
    std::vector< Option > m_options;
    
public:
    CmdLineArguments();
    
    void add_parameter_positional( const std::string param );
    void add_parameter_positional( Option option );
    void add_parameter_option( const std::string option, const char option_short );
    void add_parameter_option( Option option );
    
    bool is_option( const std::string parameter ) const;
    
    const Option lookup_positional( const std::string option_name );
    const Option lookup_option( const std::string option_name ) const;
    
    std::vector< Option > parameters_positional() const;
    std::vector< Option > parameters_options() const;
    
    class NotFound
        :   std::out_of_range
    {
    public:
        NotFound( const std::string& what_arg );
    };
};

} // namespace cmdlinecpp
