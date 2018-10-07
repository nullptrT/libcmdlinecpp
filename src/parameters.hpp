/**
 * @file parameters.hpp
 * @brief A file that defines a map with the parameter name and value
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
#include <string>


namespace cmdlinecpp {


class CmdLineParameters {
protected:
    std::map< std::string, std::string > m_parameters;
    
public:
    CmdLineParameters();
    
    void add_argument( const std::string argument );
    
    void set( const std::string argument
            , const std::string parameter );
    
    const std::string get( const std::string argument ) const;
    
    bool has_argument( const std::string arguments ) const;
    
    const size_t size() const;
    void clear();
};


} // namespace cmdlinecpp
