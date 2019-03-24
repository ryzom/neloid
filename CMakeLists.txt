#-----------------------------------------------------------------------------
#
# Neloid
#    Author: Matt Raykowski
#    Author: Based on work by Thomas Egi.
#
# Notes:
#   * Changing install location: add -DCMAKE_INSTALL_PREFIX:PATH=/my/new/path
#   * Changing NeL prefix: add -DNEL_PREFIX_PATH:PATH=/my/nel/install
#
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# Load some macros.
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")
INCLUDE(CMakeModules/nel.cmake)

#-----------------------------------------------------------------------------
# Force out of source builds.
CHECK_OUT_OF_SOURCE()

IF(WIN32)
  SET(CMAKE_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/../3rdParty/include;${CMAKE_INCLUDE_PATH}")
  SET(CMAKE_LIBRARY_PATH "${CMAKE_SOURCE_DIR}/../3rdParty/lib;${CMAKE_LIBRARY_PATH}")
ENDIF(WIN32)

IF(DEFINED NEL_PREFIX_PATH)
  SET(CMAKE_INCLUDE_PATH "${NEL_PREFIX_PATH}/include;${CMAKE_INCLUDE_PATH}")
  SET(CMAKE_LIBRARY_PATH "${NEL_PREFIX_PATH}/lib;${CMAKE_LIBRARY_PATH}")
ELSE(DEFINED NEL_PREFIX_PATH)
  SET(NEL_PREFIX_PATH "" CACHE FILEPATH "NeL Installation Location")
ENDIF(DEFINED NEL_PREFIX_PATH)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(Neloid CXX C)
SET(NL_VERSION_MAJOR 0)
SET(NL_VERSION_MINOR 1)
SET(NL_VERSION_PATCH 0)
SET(NL_VERSION "${NL_VERSION_MAJOR}.${NL_VERSION_MINOR}.${NL_VERSION_PATCH}")

#-----------------------------------------------------------------------------
# Redirect output files
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/lib)

#-----------------------------------------------------------------------------
# Set CMake 2.6 Policies.
IF(COMMAND cmake_policy)
  # Works around warnings libraries linked against that don't
  # have absolute paths (e.g. -lpthreads)
  cmake_policy(SET CMP0003 NEW)

  # Works around warnings about escaped quotes in ADD_DEFINITIONS 
  # statements
  cmake_policy(SET CMP0005 OLD)
ENDIF(COMMAND cmake_policy)

#-----------------------------------------------------------------------------
# Set default config options
#
NL_SETUP_DEFAULT_OPTIONS()
NL_SETUP_PREFIX_PATHS()

#-----------------------------------------------------------------------------
# Override default options
OPTION(BUILD_CLIENT			"Build the client"			ON )

#-----------------------------------------------------------------------------
#Platform specifics

FIND_PACKAGE(Threads REQUIRED)
FIND_PACKAGE(LibXml2 REQUIRED)

NL_SETUP_BUILD()

FIND_PACKAGE(NeL COMPONENTS nelmisc nel3d nelsound nelsnd_lowlevel nelnet)
FIND_PACKAGE(CEGUI)

IF(BUILD_CLIENT)
  FIND_PACKAGE(FreeType)
  FIND_PACKAGE(Jpeg)
  FIND_PACKAGE(PNG)
ENDIF(BUILD_CLIENT)


IF(WIN32)
  INCLUDE(${CMAKE_ROOT}/Modules/Platform/Windows-cl.cmake)
  SET(WINSOCK2_LIB ws2_32.lib)
ENDIF(WIN32)

NL_SETUP_BUILD_FLAGS()

# This tells the application(s) where to fidn the installed data.
ADD_DEFINITIONS(-DDATA_DIR="\\"${NL_SHARE_PREFIX}/neloid/\\"")

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(include)

INSTALL(DIRECTORY data/
        DESTINATION share/neloid/data
        COMPONENT data
        PATTERN "CVS" EXCLUDE
        PATTERN ".svn" EXCLUDE
        PATTERN "Makefile*" EXCLUDE)

# packaging information
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Neloid")
SET(CPACK_PACKAGE_VENDOR "NeL")
SET(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_SOURCE_DIR}/README)
SET(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/COPYING)
SET(CPACK_PACKAGE_VERSION_MAJOR "${NL_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${NL_VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${NL_VERSION_PATCH}")
SET(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_BINARY_DIR};Neloid;ALL;/")
SET(CPACK_PACKAGE_EXECUTABLES "neloid-${NL_VERSION}" "neloid")

## Source Packages
SET(CPACK_PACKAGE_FILE_NAME "neloid-${NL_VERSION}")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "neloid-${NL_VERSION}")
IF(WIN32)
  SET(CPACK_GENERATOR "NSIS")
  SET(CPACK_SOURCE_GENERATOR "ZIP")
ELSE(WIN32)
  SET(CPACK_GENERATOR "TGZ")
  SET(CPACK_SOURCE_GENERATOR "TGZ")
ENDIF(WIN32)
set(CPACK_SOURCE_IGNORE_FILES
        "~$"
        "\\\\.cvsignore$"
        "^${CMAKE_SOURCE_DIR}.*/CVS/"
        "^${CMAKE_SOURCE_DIR}.*/\\\\.svn/"
        "^${CMAKE_SOURCE_DIR}/debian/"
        "^${CMAKE_SOURCE_DIR}/old/")

IF(WIN32)
  IF(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    # Install "Debug" specific stuff here.
    SET(CMAKE_INSTALL_DEBUG_LIBRARIES TRUE)
    message(status "install debug libraries.")
  ELSE(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    # Install "Release" Specific stuff here.
    SET(CMAKE_INSTALL_DEBUG_LIBRARIES TRUE)
  ENDIF(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
  
  
  IF(BUILD_CLIENT)
    INSTALL(FILES ${NELDRVOPENGL_DLL} ${NELDRVDIRECT3D_DLL} DESTINATION bin)
  ENDIF(BUILD_CLIENT)
    
  INCLUDE(InstallRequiredSystemLibraries)
ENDIF(WIN32)

INCLUDE(CPack)

cpack_add_install_type(Full DISPLAY_NAME "Full Install")
cpack_add_component_group(Application 
  EXPANDED 
  DESCRPTION "The runtime application.")
cpack_add_component(runtime
  DISPLAY_NAME "Application Runtime"
  DESCRIPTION "Hello World Application"
  GROUP Application
  INSTALL_TYPES Full)
cpack_add_component(Unspecified
  DISPLAY_NAME "Application Runtime Libraries"
  DESCRIPTION "Hello World Application Libraries"
  GROUP Application
  INSTALL_TYPES Full)
cpack_add_component(data
  DISPLAY_NAME "Application Data"
  DESCRIPTION "Hello World Application Data"
  GROUP Application
  INSTALL_TYPES Full) 

