#.rst:
# FindLibCmdLineCPP
# -----------
#
# Try to find the LibCmdLineCPP cmdlineging library
#
# Once done this will define
#
# ::
#
#   LIBCMDLINECPP_FOUND - System has LibCmdLineCPP
#   LIBCMDLINECPP_INCLUDE_DIR - The LibCmdLineCPP include directory
#   LIBCMDLINECPP_LIBRARY - The libraries needed to use LibCmdLineCPP
#   LIBCMDLINECPP_VERSION_STRING - the version of LibCmdLineCPP found (since CMake 2.8.8)
#
#
#	libcmdlinecpp: A simple parser for command line arguments with C++
#	Copyright (C) 2018 Sebastian Lau
#
#	This library is free software; you can redistribute it and/or
#	modify it under the terms of the GNU Lesser General Public
#	License as published by the Free Software Foundation; either
#	version 3.0 of the License, or (at your option) any later version.
#
#	This library is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#	Lesser General Public License for more details.
#
#	You should have received a copy of the GNU Lesser General Public
#	License along with this library; if not, write to the Free Software
#	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
#	USA
#


find_path(LIBCMDLINECPP_INCLUDE_DIR NAMES libcmdlinecpp/cmdline.hpp)
find_library(LIBCMDLINECPP_LIBRARY NAMES cmdlinecpp)
find_library(LIBCMDLINECPP_LIBRARY_STATIC NAMES cmdlinecpp.a)

if(LIBCMDLINECPP_INCLUDE_DIR AND EXISTS "${LIBCMDLINECPP_INCLUDE_DIR}/libcmdlinecpp/cmdlinecppversion.hpp")
    file(STRINGS "${LIBCMDLINECPP_INCLUDE_DIR}/libcmdlinecpp/cmdlinecppversion.hpp" libcmdlinecpp_version_str
         REGEX "^#define[\t ]+LIBCMDLINECPP_DOTTED_VERSION[\t ]+\".*\"")

    string(REGEX REPLACE "^#define[\t ]+LIBCMDLINECPP_DOTTED_VERSION[\t ]+\"([^\"]*)\".*" "\\1"
           LIBCMDLINECPP_VERSION_STRING "${libcmdlinecpp_version_str}")
    unset(libcmdlinecpp_version_str)
endif()

# handle the QUIETLY and REQUIRED arguments and set LIBCMDLINECPP_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LibCmdLineCPP
                                  REQUIRED_VARS LIBCMDLINECPP_LIBRARY LIBCMDLINECPP_LIBRARY_STATIC LIBCMDLINECPP_INCLUDE_DIR
                                  VERSION_VAR LIBCMDLINECPP_VERSION_STRING)

mark_as_advanced(LIBCMDLINECPP_INCLUDE_DIR LIBCMDLINECPP_LIBRARY LIBCMDLINECPP_LIBRARY_STATIC)
