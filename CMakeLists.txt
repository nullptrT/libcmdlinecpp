project(libcmdlinecpp)

cmake_minimum_required( VERSION 3.0 )

set( LIBCMDLINECPP_STABLE_VERSION 0 )
set( LIBCMDLINECPP_RELEASE 1 )
set( LIBCMDLINECPP_PATCH_VERSION 0 )
set( LIBCMDLINECPP_VERSION "${LIBCMDLINECPP_STABLE_VERSION}.${LIBCMDLINECPP_RELEASE}.${LIBCMDLINECPP_PATCH_VERSION}" )
add_definitions( -DLIBCMDLINECPP_VERSION="${LIBCMDLINECPP_VERSION}" )

set( LIBCMDLINECPP_SRC_DIR ${PROJECT_SOURCE_DIR}/src )

if( UNIX )
	add_definitions( -std=c++11 )
endif()
add_definitions(-DCMDLINECPP_SRC_DIR=${LIBCMDLINECPP_SRC_DIR})


if( CMDLINECPP_DESTDIR )
else()
	set( CMDLINECPP_DESTDIR "${CMAKE_INSTALL_PREFIX}" )
endif()

if( CMDLINECPP_HEADER_INSTALL_DIR )
else()
	set( CMDLINECPP_HEADER_INSTALL_DIR ${CMDLINECPP_DESTDIR}/include/liblogcpp )
endif()

if ( CMDLINECPP_LIB_INSTALL_DIR )
else()
	set ( CMDLINECPP_LIB_INSTALL_DIR ${CMDLINECPP_DESTDIR}/lib )
endif()


add_subdirectory( src )


if( CMDLINECPP_INSTALL_LIBS )
	install(FILES ${PROJECT_SOURCE_DIR}/lib/LibCmdLineCPPConfig.cmake DESTINATION "${CMDLINECPP_LIB_INSTALL_DIR}/cmake/LibCmdLineCPP" )
	configure_file(${PROJECT_SOURCE_DIR}/lib/libcmdlinecpp.pc.in ${CMAKE_CURRENT_BINARY_DIR}/libcmdlinecpp.pc @ONLY)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libcmdlinecpp.pc DESTINATION "${CMDLINECPP_LIB_INSTALL_DIR}/pkgconfig" )
endif()


if( UNIX )
	find_package(Doxygen)
	if(DOXYGEN_FOUND)
		message("-- Enabled doc_cmdlinecpp target for make")
		configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
		add_custom_target(doc_cmdlinecpp ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMENT "Generating documentation with Doxygen")
	endif()
endif()
