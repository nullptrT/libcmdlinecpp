/**
 * @file parameters.cpp
 * @brief A file that defines a map with the parameter name and value
 * @author Sebastian Lau <lauseb644 [at] gmail [dot] com>
 **/
/*
    LibCmdLineArgumentsC++: A simple parser for command line arguments with C++
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
{}


void CmdLineParameters::add_argument( const std::string argument ) {
    std::string empty;
    m_parameters.insert( std::make_pair(argument, empty) );
}


void CmdLineParameters::set( const std::string argument, const std::string parameter ) {
    try {
        m_parameters.at( argument ) = parameter;
    } catch ( std::out_of_range& oor ) {
        std::cerr << "Argument '" << argument << "' not found in parameter list." << std::endl;
    }
}


const std::string CmdLineParameters::get( const std::string argument ) const {
    try {
        return m_parameters.at( argument );
    } catch ( std::exception& e ) {
        std::cerr << "Argument '" << argument << "' not found in parameter list." << std::endl;
    }
    
    return std::string();
}


bool CmdLineParameters::has_argument( const std::string arguments ) const {
    try {
        m_parameters.at( arguments );
        return true;
    } catch ( std::out_of_range& oor ) {
        return false;
    }
}


const size_t CmdLineParameters::size() const {
    return m_parameters.size();
}


void CmdLineParameters::clear() {
    m_parameters.clear();
}



} // namespace cmdlinecpp
