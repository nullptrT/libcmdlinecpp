
include_directories( . )

file( GLOB LIBCMDLINECPP_HEADERS *.hpp )

set( LIBCMDLINECPP_SOURCE arguments.cpp
                          cmdline.cpp
                          exceptions.cpp
                          parameters.cpp )

if( CMDLINECPP_SHARED )
	add_library( cmdlinecpp SHARED ${LIBCMDLINECPP_SOURCE} )
	set( CMDLINECPP_PKGCONFIG_LIBNAME "libcmdlinecpp.so" )
else()
	add_library( cmdlinecpp STATIC ${LIBCMDLINECPP_SOURCE} )
	set( CMDLINECPP_PKGCONFIG_LIBNAME "libcmdlinecpp.a" )
endif()


add_subdirectory( examples )


if( CMDLINECPP_INSTALL_LIBS )
	install(TARGETS cmdlinecpp DESTINATION ${CMDLINECPP_LIB_INSTALL_DIR} )
	install(FILES ${LIBCMDLINECPP_HEADERS} DESTINATION ${CMDLINECPP_HEADER_INSTALL_DIR} )
endif()


