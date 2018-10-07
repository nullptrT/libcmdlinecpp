/**
 * @file arguments.cpp
 * @brief A file that defines types of command line arguments
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

#include "arguments.hpp"


namespace cmdlinecpp {


Data::Data()
    :   m_key()
    ,   m_appearence()
{}


Data::Data( const std::string key )
    :   m_key( key )
    ,   m_appearence( 1 )
{}


Data::Data( const std::string key
          , const unsigned int appearence )
    :   m_key( key )
    ,   m_appearence( appearence )
{}


bool Data::is_valid() const {
    if ( m_appearence > 0 && m_key.length() > 0 ) {
        return true;
    }
    return false;
}


const std::string Data::key() const {
    return m_key;
}


const unsigned int Data::appearence() const {
    return m_appearence;
}


Option::Option( const std::string option_long
              , const std::string help_text
              , Data::Type data_type )
    :   m_option( option_long, std::string("") )
    ,   m_help_text( help_text )
    ,   m_data_type( data_type )
{}


Option::Option( const std::string option_long
              , const char option_short
              , const std::string help_text
              , Data::Type data_type )
    :   m_option( option_long, std::to_string(option_short) )
    ,   m_help_text( help_text )
    ,   m_data_type( data_type )
{
    if ( m_option.second.length() == 0 ) {
        throw new std::logic_error("The parameter option_short does not need to be the empty character.");
    }
}


bool Option::is_positional() const {
    if ( m_option.second.length() == 0 ) {
        return false;
    }
    return true;
}


bool Option::is_regular() const {
    return ! this->is_positional();
}


bool Option::has_help_text() const {
    if ( m_help_text.length() > 0 ) {
        return true;
    }
    return false;
}


const Data::Type Option::dataType() const {
    return m_data_type;
}


const std::string Option::option() const {
    return m_option.first;
}


const char Option::option_short() const {
    return m_option.second[0];
}


const std::string Option::help_text() const {
    return m_help_text;
}



Option& Option::operator<<( const std::string help_text ) {
    std::string help = help_text;
    if ( help_text.find("\n") != std::string::npos ) {
        while ( size_t pos = help_text.find("\n") != std::string::npos ) {
            help.insert( pos, " ");
            help.erase( pos+1, 1 );
        }
    }
    
    m_help_text += help_text;
    
    return *this;
}


CmdLineArguments::CmdLineArguments()
{}


void CmdLineArguments::add_option( Option option ) {
    if ( option.is_positional() ) {
        this->add_option_positional( option );
    } else {
        this->add_option_regular( option );
    }
}


void CmdLineArguments::add_option_positional( Option option ) {
    m_positionals.push_back( option );
}


void CmdLineArguments::add_option_regular( Option option ) {
    m_options.push_back( option );
}


bool CmdLineArguments::is_option_regular( const std::string parameter ) const {
    std::string option_str;
    if ( parameter.compare( 0, 1, "-" ) != 0 ) {
        return false;
    } else if ( parameter.compare( 0, 2, "--" ) == 0 ) {
        option_str = parameter.substr( 2 );
    } else {
        option_str = parameter.substr( 1 );
    }
    
    
    for ( unsigned int o = 0; o < m_options.size(); o++ ) {
        Option option = m_options.at( o );
        
        if ( option_str.compare( option.option() ) == 0
          || option_str.compare( std::to_string(option.option_short()) ) == 0
        ) {
           return true; 
        }
    }
    
    return false;
}


bool CmdLineArguments::is_option_positional( const std::string parameter ) const {
    std::string option_str;
    if ( parameter.compare( 0, 1, "-" ) == 0 ) {
        return false;
    }
    
    
    for ( unsigned int o = 0; o < m_options.size(); o++ ) {
        Option option = m_options.at( o );
        
        if ( option_str.compare( option.option() ) == 0 ) {
           return true; 
        }
    }
    
    return false;
}


const Option CmdLineArguments::lookup_positional( const std::string option_name ) const {
    for ( unsigned int o = 0; o < m_positionals.size(); o++ ) {
        if ( m_positionals.at(o).option().compare(option_name) == 0 ) {
            return m_positionals.at(o);
        }
    }

    throw new NotFound("Could not find positional option '"  + option_name + "'.");
}



const Option CmdLineArguments::lookup_option( const std::string option_name ) const {
    for ( unsigned int o = 0; o < m_options.size(); o++ ) {
        if ( m_options.at(o).option().compare(option_name) == 0 ) {
            return m_options.at(o);
        }
    }

    throw new NotFound("Could not find option '"  + option_name + "'.");
}



std::vector< Option > CmdLineArguments::options_positional() const {
    return m_positionals;
}


std::vector< Option > CmdLineArguments::options_regular() const {
    return m_options;
}


CmdLineArguments::NotFound::NotFound( const std::string& what_arg )
    :   std::out_of_range( what_arg )
{}


} // namespace cmdlinecpp
