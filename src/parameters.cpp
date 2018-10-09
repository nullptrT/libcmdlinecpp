/**
 * @file parameters.cpp
 * @brief A file that defines a map with the parameter name and value
 * @author Sebastian Lau <lauseb644 [at] gmail [dot] com>
 **/
/*
    LibCmdLineArgumentsC++: A simple parser for command line option_names with C++
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

#include <iostream>

#include "parameters.hpp"


namespace cmdlinecpp {


CmdLineParameters::CmdLineParameters()
    :   m_parameters()
    ,   m_last_positionals_list()
    ,   m_action()
{}


CmdLineParameters::CmdLineParameters( const CmdLineArguments* cl_args )
    :   m_parameters()
{
    for ( unsigned int o = 0
        ; o < cl_args->options_regular().size()
        ; o++
    ) {
        Option option = cl_args->options_regular().at( o );
        this->add_option_key( option.option() );
        
        if ( option.dataType() == Data::Type::Bool ) {
            this->set( option.option(), FALSE );
        }
    }
    
    for ( unsigned int o = 0
        ; o < cl_args->options_positional().size()
        ; o++
    ) {
        Option option = cl_args->options_positional().at( o );
        this->add_option_key( option.option() );
    }
}



void CmdLineParameters::add_option_key( const std::string option_name ) {
    std::string empty;
    m_parameters.insert( std::make_pair(option_name, empty) );
}


void CmdLineParameters::set( const std::string option_name, const std::string parameter ) {
    try {
        m_parameters.at( option_name ) = parameter;
    } catch ( std::out_of_range& oor ) {
        std::cerr << "Argument '" << option_name << "' not found in parameter list." << std::endl;
    }
}


void CmdLineParameters::set_action( const std::string action ) {
    m_action = action;
}


const std::string CmdLineParameters::get( const std::string option_name ) const {
    try {
         return m_parameters.at( option_name );
    } catch ( std::exception& e ) {
        std::cerr << "Argument '" << option_name << "' not found in parameter list." << std::endl;
    }
    
    return std::string("");
}


const std::string CmdLineParameters::get_action() const {
    return m_action;
}


void CmdLineParameters::set_last_positionals( std::vector< std::string > last_positionals_list ) {
    m_last_positionals_list = last_positionals_list;
}


std::vector< std::string > CmdLineParameters::get_last_positionals() const {
    return m_last_positionals_list;
}


bool CmdLineParameters::has_value( const std::string option_name ) const {
    try {
        if ( m_parameters.at( option_name ).length() > 0 ) {
            return true;
        }
    } catch ( std::out_of_range& oor ) {
        return false;
    }
    return false;
}


bool CmdLineParameters::is_specified( const std::string option_name ) const {
    try {
        m_parameters.at( option_name );
        return true;
    } catch ( std::out_of_range& oor ) {
        return false;
    }
    return false;
}



const size_t CmdLineParameters::size() const {
    return m_parameters.size();
}


void CmdLineParameters::clear() {
    m_parameters.clear();
}



} // namespace cmdlinecpp
