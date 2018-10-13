/**
 * @file exceptions.hpp
 * @brief A file that defines exceptions
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

#include <stdexcept>


namespace cmdlinecpp {
namespace exceptions {


/**
 * @brief This exception is to be thrown if a data type is wrong
 */
class TypeError
    :   std::logic_error
{
public:
    /**
     * @brief Constructor
     * @param what_arg A string that describes, what data type was expected
     */
    TypeError( const std::string& what_arg );
};


} // namespace exceptions
} // namespace cmdlinecpp
