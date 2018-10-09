/**
 * @file action.hpp
 * @brief A file that defines actions that are suitable as first command line argument
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


/**
 * @brief Class that defines an action that is suitable as first command line argument
 */
class Action {
protected:
    /**
     * @brief The name of this action
     */
    std::string m_name;
    /**
     * @brief The help text for this Option
     */
    std::string m_help_text;
    
public:
    /**
     * @brief Default constructor.
     */
    Action( const std::string name, const std::string help_text );
    
    /**
     * @brief Get the name of the action
     * @returns The name of the action
     */
    const std::string name() const;
    
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
    Action& operator<<( const std::string help_text );
};


} // namespace cmdlinecpp
