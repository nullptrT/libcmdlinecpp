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
#include <stdexcept>
#include <sstream>

#include "cmdline.hpp"
#include "exceptions.hpp"


namespace cmdlinecpp {

std::unique_ptr< CmdLineInterface > CmdLineInterface::ms_cmdlineparser;


CmdLineInterface::CmdLineInterface()
    :   m_cmdline_arguments( new CmdLineArguments() )
    ,   m_argc( 0 )
    ,   m_argv()
    ,   m_cmdline_parameters( new CmdLineParameters() )
    ,   m_current_option_requested( *new Data() )
    ,   m_program_name()
    ,   m_program_description()
    ,   m_program_version()
    ,   m_usage_examples()
{
    *this << Option( "help", "h", "Print the help for this program", Data::Type::Bool );
}


CmdLineInterface& CmdLineInterface::get() {

    if( !ms_cmdlineparser ) {
        ms_cmdlineparser.reset( new CmdLineInterface() );
    }

    return *ms_cmdlineparser;
}


void CmdLineInterface::define_option( Option option ) {
    *this << option;
}


void CmdLineInterface::define_action( Action action ) {
    *this << action;
}


CmdLineInterface& CmdLineInterface::operator<<( Action action ) {
    m_cmdline_arguments->add_action( action );
    
    return *this;
}


CmdLineInterface& CmdLineInterface::operator<<( Option option ) {
    bool found_option = false;
    for ( unsigned int o = 0; o < m_cmdline_arguments->options_regular().size(); o++ ) {
        Option option_existing = m_cmdline_arguments->options_regular().at( o );
        if ( option.option().compare( option_existing.option() ) == 0 ) {
            found_option = true;
            break;
        }
    }
    
    bool found_positional = false;
    for ( unsigned int p = 0; p < m_cmdline_arguments->options_positional().size(); p++ ) {
        if ( option.option().compare( m_cmdline_arguments->options_positional().at( p ).option() ) == 0 ) {
            found_positional = true;
            break;
        }
    }
    
    if ( !found_option && !found_positional ) {
        m_cmdline_arguments->add_option( option );
    }
    
    return *this;
}


CmdLineInterface& CmdLineInterface::operator<<( int argc ) {
    m_argc = argc;
    return *this;
}


CmdLineInterface& CmdLineInterface::operator<<( char** argv ) {
    if ( m_argc == 0 ) {
        throw std::out_of_range("You need to define a size integer with argc before defining argv.");
    }
    
    for ( unsigned int a = 0; a < m_argc; a++ ) {
        std::string argument( argv[a] );
        m_argv.push_back( argument );
    }
    
    return *this;
}


bool CmdLineInterface::parse() {
    if ( m_argv.size() == 0 ) {
        std::cerr << "No parameters to parse." << std::endl;
        return false;
    } else {
        m_cmdline_parameters = new CmdLineParameters( m_cmdline_arguments );
    }
    
    unsigned int options_regular = 0;
    unsigned int options_positional = 0;
    unsigned int a = 1;
    while ( a < m_argv.size() ) {
        std::string argument = m_argv.at( a );
        if ( m_cmdline_arguments->is_option_regular( argument ) ) {
            // Is regular option
            ++options_regular;
            
            Option option = m_cmdline_arguments->lookup_option( argument );
            
            if ( option.dataType() != Data::Type::Bool ) {
                if ( a+1 == m_argv.size() ) {
                    throw std::out_of_range("The data type of '" + argument + "' is not bool, but there are no more parameters to parse.");
                } else {
                    m_cmdline_parameters->set( option.option(), m_argv.at(a+1) );
                    a += 2;
                }
            } else {
                m_cmdline_parameters->set( option.option(), TRUE );
                ++a;
            }
            
        } else {
            // Is positional option
            
            if ( options_positional == m_cmdline_arguments->options_positional().size() - 1
              && m_argv.size() > a + 1
            ) {
                std::vector< std::string > last_positionals;
                for ( unsigned int p = 0; p < m_argv.size(); p++ ) {
                    last_positionals.push_back( m_argv.at( p ) );
                }
                
                m_cmdline_parameters->set_last_positionals( last_positionals );
                break;
            } else {
                Option option = m_cmdline_arguments->options_positional().at( options_positional );
                
                m_cmdline_parameters->set( option.option(), argument );
            }
            
            ++options_positional;
            ++a;
        }
    }
    
    if ( options_positional != m_cmdline_arguments->options_positional().size() ) {
        throw std::out_of_range("Not all positional arguments are specified.");
    }

    return true;
}


bool CmdLineInterface::parse( int argc, char** argv ) {
    m_argc = argc;
    *this << argv;
    
    return this->parse();
}


const std::string CmdLineInterface::operator[]( const std::string key ) const {
    return m_cmdline_parameters->get( key );
}


bool CmdLineInterface::is_specified( const std::string key ) const {
    if ( m_cmdline_parameters->is_specified( key ) ) {
        Option option = m_cmdline_arguments->lookup_option( key );
        if ( m_cmdline_parameters->get( key ).length() ==  0 && option.dataType() != Data::Type::Bool ) {
            return false;
        } else if ( m_cmdline_parameters->get( key ).length() == 0 && option.dataType() == Data::Type::Bool ) {
            return true;
        } else {
            return true;
        }
    }
    
    return false;
}


bool CmdLineInterface::actions_enabled() const {
    if ( m_cmdline_arguments->actions().size() > 0 ) {
        return true;
    }
    return false;
}


const Data::Type CmdLineInterface::dataTypeOfOption( const std::string key ) const {
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


void CmdLineInterface::set_program_name( const std::string program_name ) {
    m_program_name = program_name;
}


void CmdLineInterface::set_program_description( const std::string program_description ) {
    m_program_description = program_description;
}


void CmdLineInterface::set_program_version( const std::string program_version ) {
    m_program_version = program_version;
}


void CmdLineInterface::define_program( const std::string program_name, const std::string program_description, const std::string program_version ) {
    this->set_program_name( program_name );
    this->set_program_description( program_description );
    this->set_program_version( program_version );
}


void CmdLineInterface::add_usage_example( const std::string usage_example ) {
    m_usage_examples.push_back( usage_example );
}


bool CmdLineInterface::help_requested() const {
    if ( m_cmdline_parameters->get("help").compare(TRUE) == 0 ) {
        return true;
    }
    return false;
}


void CmdLineInterface::print_help() const {
    std::cout << m_program_name << " v" << m_program_version << std::endl;
    std::cout << m_program_description << std::endl;
    std::cout << std::endl;
    std::cout << m_program_name << " [--help,-h]";
    if ( this->actions_enabled() ) {
        std::cout << " <ACTION>";
    }
    std::cout << " [OPTIONS...]";
    
    for ( unsigned int o = 0
        ; o < m_cmdline_arguments->options_positional().size()
        ; o++
    ) {
        Option option = m_cmdline_arguments->options_positional().at( o );
        
        std::cout << " <" << option.option() << ">";
    }
    
    if ( m_usage_examples.size() > 0 ) {
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << "Usage:" << std::endl;
        
        for ( unsigned int u = 0; u < m_usage_examples.size(); u++ ) {
            std::cout << m_usage_examples.at( u ) << std::endl;
        }
    }
    
    if ( this->actions_enabled() ) {
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << "ACTIONS:" << std::endl;
        
        for ( unsigned int a = 0
            ; a < m_cmdline_arguments->actions().size()
            ; a++
        ) {
            Action action = m_cmdline_arguments->actions().at( a );
            
            std::cout << "\t" << action.name();
            std::cout << "\t\t";
            
            const std::string help_text = action.help_text();
            for ( unsigned int pos = 0; pos < help_text.length()+59; pos += 60 ) {
                if ( pos > 0 ) {
                    std::cout << "\t\t\t\t";
                }
                if ( pos + 60 > help_text.length() ) {
                    std::cout << help_text.substr( pos );
                    break;
                } else {
                    std::cout << help_text.substr( pos, pos+60 ) << std::endl;
                }
            }
            
            std::cout << std::endl;
        }
    }
    
    if ( m_cmdline_arguments->options_regular().size() > 0 ) {
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << "OPTIONS:" << std::endl;
        
        for ( unsigned int o = 0
            ; o < m_cmdline_arguments->options_regular().size()
            ; o++
        ) {
            Option option = m_cmdline_arguments->options_regular().at( o );
            
            std::cout << "\t-" << option.option_short() << ", --" << option.option();
            if ( option.option().length() < 5 ) {
                std::cout << "\t\t\t";
            } else {
                std::cout << "\t\t";
            }
            
            const std::string help_text = option.help_text();
            for ( unsigned int pos = 0; pos < help_text.length()+59; pos += 60 ) {
                if ( pos > 0 ) {
                    std::cout << "\t\t\t\t";
                }
                if ( pos + 60 > help_text.length() ) {
                    std::cout << help_text.substr( pos );
                    break;
                } else {
                    std::cout << help_text.substr( pos, pos+60 ) << std::endl;
                }
            }
            
            std::cout << std::endl;
        }
    }
    
    if ( m_cmdline_arguments->options_positional().size() > 0 ) {
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << "POSITIONAL ARGUMENTS:" << std::endl;
        for ( unsigned int o = 0
            ; o < m_cmdline_arguments->options_positional().size()
            ; o++
        ) {
            Option option = m_cmdline_arguments->options_positional().at( o );
            
            std::cout << "\t" << option.option();
            if ( option.option().length() < 5 ) {
                std::cout << "\t\t";
            } else {
                std::cout << "\t";
            }
            
            const std::string help_text = option.help_text();
            for ( unsigned int pos = 0; pos < help_text.length()+59; pos += 60 ) {
                if ( pos > 0 ) {
                    std::cout << "\t\t\t\t";
                }
                if ( pos + 60 > help_text.length() ) {
                    std::cout << help_text.substr( pos );
                    break;
                } else {
                    std::cout << help_text.substr( pos, pos+60 ) << std::endl;
                }
            }
            
            std::cout << std::endl;
        }
    }
}


void CmdLineInterface::print_help_if_requested() const {
    if ( this->help_requested() ) {
        this->print_help();
        std::exit( 0 );
    }
}


const std::string CmdLineInterface::get_selected_action() const {
    if ( !this->actions_enabled() ) {
        throw std::out_of_range("Actions are not enabled.");
    }
    
    return m_cmdline_parameters->get_action();
}


void CmdLineInterface::to_variable( const std::string key, bool& value ) {
    m_current_option_requested = Data( key );
    if ( !m_current_option_requested.is_valid() ) {
        throw std::out_of_range("Requested option not set with a Data object.");
    }
    
    try {
        Option option = m_cmdline_arguments->lookup_option( m_current_option_requested.key() );
        if ( option.dataType() != Data::Type::Double ) {
            throw exceptions::TypeError("Expected double as data type.");
        }
    } catch ( CmdLineArguments::NotFound& clanf ) {
        try {
            Option option = m_cmdline_arguments->lookup_positional( m_current_option_requested.key() );
            if ( option.dataType() != Data::Type::Double ) {
                throw exceptions::TypeError("Expected double as data type.");
            }
        } catch ( CmdLineArguments::NotFound& clanf ) {
            throw CmdLineArguments::NotFound("Did not find '" + m_current_option_requested.key() + "' as option.");
        }
    }
    
    std::string bool_value = m_cmdline_parameters->get( key );
    
    if ( bool_value == TRUE ) {
        value = true;
    } else {
        value = false;
    }
}


void CmdLineInterface::to_variable( const std::string key, double& value ) {
    m_current_option_requested = Data( key );
    if ( !m_current_option_requested.is_valid() ) {
        throw std::out_of_range("Requested option not set with a Data object.");
    }
    
    try {
        Option option = m_cmdline_arguments->lookup_option( m_current_option_requested.key() );
        if ( option.dataType() != Data::Type::Double ) {
            throw exceptions::TypeError("Expected double as data type.");
        }
    } catch ( CmdLineArguments::NotFound& clanf ) {
        try {
            Option option = m_cmdline_arguments->lookup_positional( m_current_option_requested.key() );
            if ( option.dataType() != Data::Type::Double ) {
                throw exceptions::TypeError("Expected double as data type.");
            }
        } catch ( CmdLineArguments::NotFound& clanf ) {
            throw CmdLineArguments::NotFound("Did not find '" + m_current_option_requested.key() + "' as option.");
        }
    }
    
    std::istringstream istring( m_cmdline_parameters->get( key ) );
    
    istring >> value;
}


void CmdLineInterface::to_variable( const std::string key, long& value ) {
    m_current_option_requested = Data( key );
    if ( !m_current_option_requested.is_valid() ) {
        throw std::out_of_range("Requested option not set with a Data object.");
    }
    
    try {
        Option option = m_cmdline_arguments->lookup_option( m_current_option_requested.key() );
        if ( option.dataType() != Data::Type::Long ) {
            throw exceptions::TypeError("Expected long as data type.");
        }
    } catch ( CmdLineArguments::NotFound& clanf ) {
        try {
            Option option = m_cmdline_arguments->lookup_positional( m_current_option_requested.key() );
            if ( option.dataType() != Data::Type::Long ) {
                throw exceptions::TypeError("Expected long as data type.");
            }
        } catch ( CmdLineArguments::NotFound& clanf ) {
            throw CmdLineArguments::NotFound("Did not find '" + m_current_option_requested.key() + "' as option.");
        }
    }
    
    std::istringstream istring( m_cmdline_parameters->get( key ) );
    
    istring >> value;
}


void CmdLineInterface::to_variable( const std::string key, int& value ) {
    m_current_option_requested = Data( key );
    if ( !m_current_option_requested.is_valid() ) {
        throw std::out_of_range("Requested option not set with a Data object.");
    }
    
    try {
        Option option = m_cmdline_arguments->lookup_option( m_current_option_requested.key() );
        if ( option.dataType() != Data::Type::Int ) {
            throw exceptions::TypeError("Expected int as data type.");
        }
    } catch ( CmdLineArguments::NotFound& clanf ) {
        try {
            Option option = m_cmdline_arguments->lookup_positional( m_current_option_requested.key() );
            if ( option.dataType() != Data::Type::Int ) {
                throw exceptions::TypeError("Expected int as data type.");
            }
        } catch ( CmdLineArguments::NotFound& clanf ) {
            throw CmdLineArguments::NotFound("Did not find '" + m_current_option_requested.key() + "' as option.");
        }
    }
    
    std::istringstream istring( m_cmdline_parameters->get( key ) );
    
    istring >> value;
}


void CmdLineInterface::to_variable( const std::string key, short& value ) {
    m_current_option_requested = Data( key );
    if ( !m_current_option_requested.is_valid() ) {
        throw std::out_of_range("Requested option not set with a Data object.");
    }
    
    try {
        Option option = m_cmdline_arguments->lookup_option( m_current_option_requested.key() );
        if ( option.dataType() != Data::Type::Short ) {
            throw exceptions::TypeError("Expected short as data type.");
        }
    } catch ( CmdLineArguments::NotFound& clanf ) {
        try {
            Option option = m_cmdline_arguments->lookup_positional( m_current_option_requested.key() );
            if ( option.dataType() != Data::Type::Short ) {
                throw exceptions::TypeError("Expected short as data type.");
            }
        } catch ( CmdLineArguments::NotFound& clanf ) {
            throw CmdLineArguments::NotFound("Did not find '" + m_current_option_requested.key() + "' as option.");
        }
    }
    
    std::istringstream istring( m_cmdline_parameters->get( key ) );
    
    istring >> value;
}


void CmdLineInterface::to_variable( const std::string key, std::string& value ) {
    m_current_option_requested = Data( key );
    if ( !m_current_option_requested.is_valid() ) {
        throw std::out_of_range("Requested option not set with a Data object.");
    }
    
    try {
        Option option = m_cmdline_arguments->lookup_option( m_current_option_requested.key() );
        if ( option.dataType() != Data::Type::String ) {
            throw exceptions::TypeError("Expected std::string as data type.");
        }
    } catch ( CmdLineArguments::NotFound& clanf ) {
        try {
            Option option = m_cmdline_arguments->lookup_positional( m_current_option_requested.key() );
            if ( option.dataType() != Data::Type::String ) {
                throw exceptions::TypeError("Expected std::string as data type.");
            }
        } catch ( CmdLineArguments::NotFound& clanf ) {
            throw CmdLineArguments::NotFound("Did not find '" + m_current_option_requested.key() + "' as option.");
        }
    }
    
    std::istringstream istring( m_cmdline_parameters->get( key ) );
    
    istring >> value;
}


void CmdLineInterface::to_variable( const std::string key, unsigned long& value ) {
    m_current_option_requested = Data( key );
    if ( !m_current_option_requested.is_valid() ) {
        throw std::out_of_range("Requested option not set with a Data object.");
    }
    
    try {
        Option option = m_cmdline_arguments->lookup_option( m_current_option_requested.key() );
        if ( option.dataType() != Data::Type::UnsignedLong ) {
            throw exceptions::TypeError("Expected unsigned long as data type.");
        }
    } catch ( CmdLineArguments::NotFound& clanf ) {
        try {
            Option option = m_cmdline_arguments->lookup_positional( m_current_option_requested.key() );
            if ( option.dataType() != Data::Type::UnsignedLong ) {
                throw exceptions::TypeError("Expected unsigned long as data type.");
            }
        } catch ( CmdLineArguments::NotFound& clanf ) {
            throw CmdLineArguments::NotFound("Did not find '" + m_current_option_requested.key() + "' as option.");
        }
    }
    
    std::istringstream istring( m_cmdline_parameters->get( key ) );
    
    istring >> value;
}


void CmdLineInterface::to_variable( const std::string key, unsigned int& value ) {
    m_current_option_requested = Data( key );
    if ( !m_current_option_requested.is_valid() ) {
        throw std::out_of_range("Requested option not set with a Data object.");
    }
    
    try {
        Option option = m_cmdline_arguments->lookup_option( m_current_option_requested.key() );
        if ( option.dataType() != Data::Type::UnsignedInt ) {
            throw exceptions::TypeError("Expected unsigned int as data type.");
        }
    } catch ( CmdLineArguments::NotFound& clanf ) {
        try {
            Option option = m_cmdline_arguments->lookup_positional( m_current_option_requested.key() );
            if ( option.dataType() != Data::Type::UnsignedInt ) {
                throw exceptions::TypeError("Expected unsigned int as data type.");
            }
        } catch ( CmdLineArguments::NotFound& clanf ) {
            throw CmdLineArguments::NotFound("Did not find '" + m_current_option_requested.key() + "' as option.");
        }
    }
    
    std::istringstream istring( m_cmdline_parameters->get( key ) );
    
    istring >> value;
}


void CmdLineInterface::to_variable( const std::string key, unsigned short& value ) {
    m_current_option_requested = Data( key );
    if ( !m_current_option_requested.is_valid() ) {
        throw std::out_of_range("Requested option not set with a Data object.");
    }
    
    try {
        Option option = m_cmdline_arguments->lookup_option( m_current_option_requested.key() );
        if ( option.dataType() != Data::Type::UnsignedShort ) {
            throw exceptions::TypeError("Expected unsigned short as data type.");
        }
    } catch ( CmdLineArguments::NotFound& clanf ) {
        try {
            Option option = m_cmdline_arguments->lookup_positional( m_current_option_requested.key() );
            if ( option.dataType() != Data::Type::UnsignedShort ) {
                throw exceptions::TypeError("Expected unsigned short as data type.");
            }
        } catch ( CmdLineArguments::NotFound& clanf ) {
            throw CmdLineArguments::NotFound("Did not find '" + m_current_option_requested.key() + "' as option.");
        }
    }
    
    std::istringstream istring( m_cmdline_parameters->get( key ) );
    
    istring >> value;
}


CmdLineInterface& CmdLineInterface::operator>>( Data data_requested ) {
    m_current_option_requested = data_requested;
    
    return *this;
}


CmdLineInterface& CmdLineInterface::operator>>( bool& value ) {
    if ( !m_current_option_requested.is_valid() ) {
        throw std::out_of_range("Requested option not set with a Data object.");
    }
    
    this->to_variable( m_current_option_requested.key(), value );
    
    return *this;
}


CmdLineInterface& CmdLineInterface::operator>>( double& value ) {
    if ( !m_current_option_requested.is_valid() ) {
        throw std::out_of_range("Requested option not set with a Data object.");
    }
    
    this->to_variable( m_current_option_requested.key(), value );
    
    return *this;
}


CmdLineInterface& CmdLineInterface::operator>>( long& value ) {
    if ( !m_current_option_requested.is_valid() ) {
        throw std::out_of_range("Requested option not set with a Data object.");
    }
    
    this->to_variable( m_current_option_requested.key(), value );
    
    return *this;
}


CmdLineInterface& CmdLineInterface::operator>>( int& value ) {
    if ( !m_current_option_requested.is_valid() ) {
        throw std::out_of_range("Requested option not set with a Data object.");
    }
    
    this->to_variable( m_current_option_requested.key(), value );
    
    return *this;
}


CmdLineInterface& CmdLineInterface::operator>>( short& value ) {
    if ( !m_current_option_requested.is_valid() ) {
        throw std::out_of_range("Requested option not set with a Data object.");
    }
    
    this->to_variable( m_current_option_requested.key(), value );
    
    return *this;
}


CmdLineInterface& CmdLineInterface::operator>>( std::string& value ) {
    if ( !m_current_option_requested.is_valid() ) {
        throw std::out_of_range("Requested option not set with a Data object.");
    }
    
    this->to_variable( m_current_option_requested.key(), value );
    
    return *this;
}


CmdLineInterface& CmdLineInterface::operator>>( unsigned long& value ) {
    if ( !m_current_option_requested.is_valid() ) {
        throw std::out_of_range("Requested option not set with a Data object.");
    }
    
    this->to_variable( m_current_option_requested.key(), value );
    
    return *this;
}


CmdLineInterface& CmdLineInterface::operator>>( unsigned int& value ) {
    if ( !m_current_option_requested.is_valid() ) {
        throw std::out_of_range("Requested option not set with a Data object.");
    }
    
    this->to_variable( m_current_option_requested.key(), value );
    
    return *this;
}


CmdLineInterface& CmdLineInterface::operator>>( unsigned short& value ) {
    if ( !m_current_option_requested.is_valid() ) {
        throw std::out_of_range("Requested option not set with a Data object.");
    }
    
    this->to_variable( m_current_option_requested.key(), value );
    
    return *this;
}


} // namespace cmdlinecpp

