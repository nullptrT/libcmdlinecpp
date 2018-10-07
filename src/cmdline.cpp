/**
 * @file cmdline.cpp
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

#include <iostream>
#include <sstream>

#include "cmdline.hpp"
#include "exceptions.hpp"


namespace cmdlinecpp {

std::unique_ptr< CmdLineParser > CmdLineParser::ms_cmdlineparser;


CmdLineParser::CmdLineParser()
    :   m_cmdline_arguments( new CmdLineArguments() )
    ,   m_argc( 1 )
    ,   m_argv()
    ,   m_cmdline_parameters( new CmdLineParameters() )
    ,   m_current_option_requested( *new Data() )
    ,   m_program_name()
    ,   m_description()
    ,   m_version()
    ,   m_usage_examples()
{}


CmdLineParser& CmdLineParser::get() {

    if( !ms_cmdlineparser ) {
        ms_cmdlineparser.reset( new CmdLineParser() );
    }

    return *ms_cmdlineparser;
}


void CmdLineParser::define_option( Option option ) {
    *this << option;
}


CmdLineParser& CmdLineParser::operator<<( Option option ) {
    bool found_option = false;
    for ( unsigned int o = 0; o < m_cmdline_arguments->parameters_options().size(); o++ ) {
        Option option_existing = m_cmdline_arguments->parameters_options().at( o );
        if ( option.option().compare( option_existing.option() ) == 0 ) {
            found_option = true;
            break;
        }
    }
    
    bool found_positional = false;
    for ( unsigned int p = 0; p < m_cmdline_arguments->parameters_positional().size(); p++ ) {
        if ( option.option().compare( m_cmdline_arguments->parameters_positional().at( p ).option() ) ) {
            found_positional = true;
            break;
        }
    }
    
    if ( !found_option && !found_positional ) {
        if ( option.is_positional() ) {
            m_cmdline_arguments->add_parameter_positional( option );
        } else {
            m_cmdline_arguments->add_parameter_option( option );
        }
    }
    
    return *this;
}


CmdLineParser& CmdLineParser::operator<<( int argc ) {
    m_argc = argc;
    return *this;
}


CmdLineParser& CmdLineParser::operator<<( char** argv ) {
    if ( m_argc == 1 ) {
        throw new std::out_of_range("You need to define a size integer with argc before defining argv.");
    }
    
    for ( unsigned int a = 0; a < m_argc; a++ ) {
        std::string argument( argv[a] );
        m_argv.push_back( argument );
    }
    
    return *this;
}


bool CmdLineParser::parse() {
    if ( m_argv.size() == 0 ) {
        std::cerr << "No parameters to parse." << std::endl;
        return false;
    } else {
        m_cmdline_parameters->clear();
        
        for ( unsigned int o = 0
            ; o < m_cmdline_arguments->parameters_options().size()
            ; o++
        ) {
            Option option = m_cmdline_arguments->parameters_options().at( o );
            m_cmdline_parameters->add_argument( option.option() );
            
            if ( option.dataType() == Data::Type::Bool ) {
                m_cmdline_parameters->set( option.option(), FALSE );
            }
        }
        
        for ( unsigned int o = 0
            ; o < m_cmdline_arguments->parameters_positional().size()
            ; o++
        ) {
            Option option = m_cmdline_arguments->parameters_positional().at( o );
            m_cmdline_parameters->add_argument( option.option() );
        }
    }
    
    unsigned int positional = 0;
    unsigned int a = m_argv.size() - 1;
    while ( positional < m_cmdline_arguments->parameters_positional().size()
         && a > 1
    ) {
        Option argument_name_positional = m_cmdline_arguments->parameters_positional().at( m_cmdline_arguments->parameters_positional().size() - positional );
        
        m_cmdline_parameters->set( argument_name_positional.option(), m_argv.at(a) );
        
        ++positional;
        --a;
    }
    

    if ( a != m_cmdline_arguments->parameters_positional().size() ) {
        throw new std::out_of_range("Not all positional arguments are specified.");
    }

    unsigned int begin_positionals = a;
    unsigned int options = 0;
    a = 1;
    while ( a < begin_positionals ) {
        std::string parameter = m_argv.at( a );
        
        if ( m_cmdline_arguments->is_option( parameter ) ) {
            Option option = m_cmdline_arguments->lookup_option( parameter );
            
            if ( option.dataType() == Data::Type::Bool ) {
                m_cmdline_parameters->set( parameter, TRUE );
                ++options;
                ++a;
            } else {
                if ( a+1 == begin_positionals ) {
                    throw new std::out_of_range("No parameter argument specified for '" + parameter + "'.");
                } else if ( parameter.compare( 0, 1, "-" ) ) {
                    throw new std::out_of_range("No parameter argument specified for '" + parameter + "'.");
                }
                
                m_cmdline_parameters->set( parameter, m_argv.at(a+1) );
                
                ++options;
                a += 2;
            }
        }
        
        
    }
    
    return true;
}


bool CmdLineParser::parse( int argc, char** argv ) {
    m_argc = argc;
    *this << argv;
    
    return this->parse();
}


const std::string CmdLineParser::operator[]( const std::string key ) const {
    return m_cmdline_parameters->get( key );
}


bool CmdLineParser::is_specified( const std::string key ) const {
    if ( m_cmdline_parameters->get( key ).length() > 0 ) {
        return true;
    }
    return false;
}


const Data::Type CmdLineParser::dataTypeOfOption( const std::string key ) const {
    try {
        Option option = m_cmdline_arguments->lookup_option( key );
        return option.dataType();
    } catch ( CmdLineArguments::NotFound& clanf ) {
        try {
            Option option = m_cmdline_arguments->lookup_positional( key );
            return option.dataType();
        } catch ( CmdLineArguments::NotFound& clanf ) {
            return Data::Type::Void;
        }
    }
    
    return Data::Type::Void;
}


void CmdLineParser::set_program_name( const std::string program_name ) {
    m_program_name = program_name;
}


void CmdLineParser::set_description( const std::string description ) {
    m_description = description;
}


void CmdLineParser::set_version( const std::string version ) {
    m_version = version;
}


void CmdLineParser::add_usage_example( const std::string usage_example ) {
    m_usage_examples.push_back( usage_example );
}


void CmdLineParser::print_help() const {
    
}


void CmdLineParser::to_variable( const std::string key, bool& value ) {
    std::string bool_value = m_cmdline_parameters->get( key );
    
    if ( bool_value == TRUE ) {
        value = true;
    } else {
        value = false;
    }
}


void CmdLineParser::to_variable( const std::string key, double& value ) {
    m_current_option_requested = Data( key );
    if ( !m_current_option_requested.is_valid() ) {
        throw new std::out_of_range("Requested option not set with a Data object.");
    }
    
    try {
        Option option = m_cmdline_arguments->lookup_option( m_current_option_requested.key() );
        if ( option.dataType() != Data::Type::Double ) {
            throw new exceptions::TypeError("Expected double as data type.");
        }
    } catch ( CmdLineArguments::NotFound& clanf ) {
        try {
            Option option = m_cmdline_arguments->lookup_positional( m_current_option_requested.key() );
            if ( option.dataType() != Data::Type::Double ) {
                throw new exceptions::TypeError("Expected double as data type.");
            }
        } catch ( CmdLineArguments::NotFound& clanf ) {
            throw new CmdLineArguments::NotFound("Did not find '" + m_current_option_requested.key() + "' as option.");
        }
    }
    
    std::istringstream istring( m_cmdline_parameters->get( key ) );
    
    istring >> value;
}


void CmdLineParser::to_variable( const std::string key, long& value ) {
    m_current_option_requested = Data( key );
    if ( !m_current_option_requested.is_valid() ) {
        throw new std::out_of_range("Requested option not set with a Data object.");
    }
    
    try {
        Option option = m_cmdline_arguments->lookup_option( m_current_option_requested.key() );
        if ( option.dataType() != Data::Type::Long ) {
            throw new exceptions::TypeError("Expected long as data type.");
        }
    } catch ( CmdLineArguments::NotFound& clanf ) {
        try {
            Option option = m_cmdline_arguments->lookup_positional( m_current_option_requested.key() );
            if ( option.dataType() != Data::Type::Long ) {
                throw new exceptions::TypeError("Expected long as data type.");
            }
        } catch ( CmdLineArguments::NotFound& clanf ) {
            throw new CmdLineArguments::NotFound("Did not find '" + m_current_option_requested.key() + "' as option.");
        }
    }
    
    std::istringstream istring( m_cmdline_parameters->get( key ) );
    
    istring >> value;
}


void CmdLineParser::to_variable( const std::string key, int& value ) {
    m_current_option_requested = Data( key );
    if ( !m_current_option_requested.is_valid() ) {
        throw new std::out_of_range("Requested option not set with a Data object.");
    }
    
    try {
        Option option = m_cmdline_arguments->lookup_option( m_current_option_requested.key() );
        if ( option.dataType() != Data::Type::Int ) {
            throw new exceptions::TypeError("Expected int as data type.");
        }
    } catch ( CmdLineArguments::NotFound& clanf ) {
        try {
            Option option = m_cmdline_arguments->lookup_positional( m_current_option_requested.key() );
            if ( option.dataType() != Data::Type::Int ) {
                throw new exceptions::TypeError("Expected int as data type.");
            }
        } catch ( CmdLineArguments::NotFound& clanf ) {
            throw new CmdLineArguments::NotFound("Did not find '" + m_current_option_requested.key() + "' as option.");
        }
    }
    
    std::istringstream istring( m_cmdline_parameters->get( key ) );
    
    istring >> value;
}


void CmdLineParser::to_variable( const std::string key, short& value ) {
    m_current_option_requested = Data( key );
    if ( !m_current_option_requested.is_valid() ) {
        throw new std::out_of_range("Requested option not set with a Data object.");
    }
    
    try {
        Option option = m_cmdline_arguments->lookup_option( m_current_option_requested.key() );
        if ( option.dataType() != Data::Type::Short ) {
            throw new exceptions::TypeError("Expected short as data type.");
        }
    } catch ( CmdLineArguments::NotFound& clanf ) {
        try {
            Option option = m_cmdline_arguments->lookup_positional( m_current_option_requested.key() );
            if ( option.dataType() != Data::Type::Short ) {
                throw new exceptions::TypeError("Expected short as data type.");
            }
        } catch ( CmdLineArguments::NotFound& clanf ) {
            throw new CmdLineArguments::NotFound("Did not find '" + m_current_option_requested.key() + "' as option.");
        }
    }
    
    std::istringstream istring( m_cmdline_parameters->get( key ) );
    
    istring >> value;
}


void CmdLineParser::to_variable( const std::string key, std::string& value ) {
    m_current_option_requested = Data( key );
    if ( !m_current_option_requested.is_valid() ) {
        throw new std::out_of_range("Requested option not set with a Data object.");
    }
    
    try {
        Option option = m_cmdline_arguments->lookup_option( m_current_option_requested.key() );
        if ( option.dataType() != Data::Type::String ) {
            throw new exceptions::TypeError("Expected std::string as data type.");
        }
    } catch ( CmdLineArguments::NotFound& clanf ) {
        try {
            Option option = m_cmdline_arguments->lookup_positional( m_current_option_requested.key() );
            if ( option.dataType() != Data::Type::String ) {
                throw new exceptions::TypeError("Expected std::string as data type.");
            }
        } catch ( CmdLineArguments::NotFound& clanf ) {
            throw new CmdLineArguments::NotFound("Did not find '" + m_current_option_requested.key() + "' as option.");
        }
    }
    
    std::istringstream istring( m_cmdline_parameters->get( key ) );
    
    istring >> value;
}


void CmdLineParser::to_variable( const std::string key, unsigned long& value ) {
    m_current_option_requested = Data( key );
    if ( !m_current_option_requested.is_valid() ) {
        throw new std::out_of_range("Requested option not set with a Data object.");
    }
    
    try {
        Option option = m_cmdline_arguments->lookup_option( m_current_option_requested.key() );
        if ( option.dataType() != Data::Type::UnsignedLong ) {
            throw new exceptions::TypeError("Expected unsigned long as data type.");
        }
    } catch ( CmdLineArguments::NotFound& clanf ) {
        try {
            Option option = m_cmdline_arguments->lookup_positional( m_current_option_requested.key() );
            if ( option.dataType() != Data::Type::UnsignedLong ) {
                throw new exceptions::TypeError("Expected unsigned long as data type.");
            }
        } catch ( CmdLineArguments::NotFound& clanf ) {
            throw new CmdLineArguments::NotFound("Did not find '" + m_current_option_requested.key() + "' as option.");
        }
    }
    
    std::istringstream istring( m_cmdline_parameters->get( key ) );
    
    istring >> value;
}


void CmdLineParser::to_variable( const std::string key, unsigned int& value ) {
    m_current_option_requested = Data( key );
    if ( !m_current_option_requested.is_valid() ) {
        throw new std::out_of_range("Requested option not set with a Data object.");
    }
    
    try {
        Option option = m_cmdline_arguments->lookup_option( m_current_option_requested.key() );
        if ( option.dataType() != Data::Type::UnsignedInt ) {
            throw new exceptions::TypeError("Expected unsigned int as data type.");
        }
    } catch ( CmdLineArguments::NotFound& clanf ) {
        try {
            Option option = m_cmdline_arguments->lookup_positional( m_current_option_requested.key() );
            if ( option.dataType() != Data::Type::UnsignedInt ) {
                throw new exceptions::TypeError("Expected unsigned int as data type.");
            }
        } catch ( CmdLineArguments::NotFound& clanf ) {
            throw new CmdLineArguments::NotFound("Did not find '" + m_current_option_requested.key() + "' as option.");
        }
    }
    
    std::istringstream istring( m_cmdline_parameters->get( key ) );
    
    istring >> value;
}


void CmdLineParser::to_variable( const std::string key, unsigned short& value ) {
    m_current_option_requested = Data( key );
    if ( !m_current_option_requested.is_valid() ) {
        throw new std::out_of_range("Requested option not set with a Data object.");
    }
    
    try {
        Option option = m_cmdline_arguments->lookup_option( m_current_option_requested.key() );
        if ( option.dataType() != Data::Type::UnsignedShort ) {
            throw new exceptions::TypeError("Expected unsigned short as data type.");
        }
    } catch ( CmdLineArguments::NotFound& clanf ) {
        try {
            Option option = m_cmdline_arguments->lookup_positional( m_current_option_requested.key() );
            if ( option.dataType() != Data::Type::UnsignedShort ) {
                throw new exceptions::TypeError("Expected unsigned short as data type.");
            }
        } catch ( CmdLineArguments::NotFound& clanf ) {
            throw new CmdLineArguments::NotFound("Did not find '" + m_current_option_requested.key() + "' as option.");
        }
    }
    
    std::istringstream istring( m_cmdline_parameters->get( key ) );
    
    istring >> value;
}


CmdLineParser& CmdLineParser::operator>>( Data option_requested ) {
    m_current_option_requested = option_requested;
    
    return *this;
}


CmdLineParser& CmdLineParser::operator>>( bool& value ) {
    if ( !m_current_option_requested.is_valid() ) {
        throw new std::out_of_range("Requested option not set with a Data object.");
    }
    
    this->to_variable( m_current_option_requested.key(), value );
    
    return *this;
}


CmdLineParser& CmdLineParser::operator>>( double& value ) {
    if ( !m_current_option_requested.is_valid() ) {
        throw new std::out_of_range("Requested option not set with a Data object.");
    }
    
    this->to_variable( m_current_option_requested.key(), value );
    
    return *this;
}


CmdLineParser& CmdLineParser::operator>>( long& value ) {
    if ( !m_current_option_requested.is_valid() ) {
        throw new std::out_of_range("Requested option not set with a Data object.");
    }
    
    this->to_variable( m_current_option_requested.key(), value );
    
    return *this;
}


CmdLineParser& CmdLineParser::operator>>( int& value ) {
    if ( !m_current_option_requested.is_valid() ) {
        throw new std::out_of_range("Requested option not set with a Data object.");
    }
    
    this->to_variable( m_current_option_requested.key(), value );
    
    return *this;
}


CmdLineParser& CmdLineParser::operator>>( short& value ) {
    if ( !m_current_option_requested.is_valid() ) {
        throw new std::out_of_range("Requested option not set with a Data object.");
    }
    
    this->to_variable( m_current_option_requested.key(), value );
    
    return *this;
}


CmdLineParser& CmdLineParser::operator>>( std::string& value ) {
    if ( !m_current_option_requested.is_valid() ) {
        throw new std::out_of_range("Requested option not set with a Data object.");
    }
    
    this->to_variable( m_current_option_requested.key(), value );
    
    return *this;
}


CmdLineParser& CmdLineParser::operator>>( unsigned long& value ) {
    if ( !m_current_option_requested.is_valid() ) {
        throw new std::out_of_range("Requested option not set with a Data object.");
    }
    
    this->to_variable( m_current_option_requested.key(), value );
    
    return *this;
}


CmdLineParser& CmdLineParser::operator>>( unsigned int& value ) {
    if ( !m_current_option_requested.is_valid() ) {
        throw new std::out_of_range("Requested option not set with a Data object.");
    }
    
    this->to_variable( m_current_option_requested.key(), value );
    
    return *this;
}


CmdLineParser& CmdLineParser::operator>>( unsigned short& value ) {
    if ( !m_current_option_requested.is_valid() ) {
        throw new std::out_of_range("Requested option not set with a Data object.");
    }
    
    this->to_variable( m_current_option_requested.key(), value );
    
    return *this;
}


} // namespace cmdlinecpp

