# - Locate NeL 3D library
# This module defines
#  NEL3D_LIBRARY, the library to link against
#  NEL_FOUND, if false, do not try to link to NEL3D
#  NEL_INCLUDE_DIR, where to find headers.

IF(NELMISC_LIBRARY AND NEL_INCLUDE_DIR)
  # in cache already
  SET(NEL_FIND_QUIETLY TRUE)
ENDIF(NELMISC_LIBRARY AND NEL_INCLUDE_DIR)

# Assume we find NeL and correct it along the way.
SET(NEL_FOUND "YES")

# determine the components to retrieve.
IF(NOT NeL_FIND_COMPONENTS)
  # We must always have nelmisc.
  SET(NeL_FIND_COMPONENTS nelmisc)
ELSE(NOT NeL_FIND_COMPONENTS)
  # Make sure we have nelmisc in the list.
  LIST(FIND NeL_FIND_COMPONENTS nelmisc TMP_FIND_NELMISC)
  IF(TMP_FIND_NELMISC EQUAL -1)
    LIST(APPEND NeL_FIND_COMPONENTS nelmisc)
  ENDIF(TMP_FIND_NELMISC EQUAL -1)
ENDIF(NOT NeL_FIND_COMPONENTS)  

# Find the path to the NeL includes.
FIND_PATH(NEL_INCLUDE_DIR nel/misc/types_nl.h 
  PATHS
  [HKEY_LOCAL_MACHINE\\SOFTWARE\\NeL\\NeL;]/include
  $ENV{ProgramFiles}/NeL/include
  $ENV{NEL_DIR}/include
  /usr/local/include
  /usr/include
  /sw/include
  /opt/local/include
  /opt/csw/include
  /opt/include)

# Make sure we found the include files.
IF(NOT NEL_INCLUDE_DIR)
  SET(NEL_FOUND "NO")
ENDIF(NOT NEL_INCLUDE_DIR)

# A utility macro to wrap NeL finds...
MACRO(FIND_NEL_LIBRARY MYLIBRARY)   
  FIND_LIBRARY(${MYLIBRARY}
               NAMES ${ARGN}
               PATHS 
				[HKEY_LOCAL_MACHINE\\SOFTWARE\\NeL\\NeL;]/lib
				$ENV{ProgramFiles}/NeL/lib
				$ENV{NEL_DIR}/lib
				/usr/local/lib
				/usr/lib
				/usr/local/X11R6/lib
				/usr/X11R6/lib
				/sw/lib
				/opt/local/lib
				/opt/csw/lib
				/opt/lib
				/usr/freeware/lib64)               
ENDMACRO(FIND_NEL_LIBRARY MYLIBRARY)


# Find the library for each required library.
FOREACH(NL_F_COMPONENT ${NeL_FIND_COMPONENTS})
  # Check for NeL Misc
  IF(NL_F_COMPONENT STREQUAL "nelmisc")
    FIND_NEL_LIBRARY(NELMISC_LIBRARY nelmisc nelmisc_r)
    FIND_NEL_LIBRARY(NELMISC_LIBRARY_DEBUG nelmisc_d)
    IF(NOT NELMISC_LIBRARY)
      SET(NEL_FOUND "NO")
    ENDIF(NOT NELMISC_LIBRARY)
    IF(NELMISC_LIBRARY_DEBUG)
		SET(NELMISC_LIBRARY debug ${NELMISC_LIBRARY_DEBUG} optimized ${NELMISC_LIBRARY})
    ENDIF(NELMISC_LIBRARY_DEBUG)

  # Check for NeL 3D
  ELSEIF(NL_F_COMPONENT STREQUAL "nel3d")
    FIND_NEL_LIBRARY(NEL3D_LIBRARY nel3d nel3d_r)
    FIND_NEL_LIBRARY(NEL3D_LIBRARY_DEBUG nel3d_d)
    IF(NOT NEL3D_LIBRARY)
      SET(NEL_FOUND "NO")
    ENDIF(NOT NEL3D_LIBRARY)
    IF(NEL3D_LIBRARY_DEBUG)
		SET(NEL3D_LIBRARY debug ${NEL3D_LIBRARY_DEBUG} optimized ${NEL3D_LIBRARY})
    ENDIF(NEL3D_LIBRARY_DEBUG)
    
  # Check for NeL Georges
  ELSEIF(NL_F_COMPONENT STREQUAL "nelgeorges")
    FIND_NEL_LIBRARY(NELGEORGES_LIBRARY nelgeorges nelgeorges_r)
    FIND_NEL_LIBRARY(NELGEORGES_LIBRARY_DEBUG nelgeorges_d)
    IF(NOT NELGEORGES_LIBRARY)
      SET(NEL_FOUND "NO")
    ENDIF(NOT NELGEORGES_LIBRARY)
    IF(NELGEORGES_LIBRARY_DEBUG)
		SET(NELGEORGES_LIBRARY debug ${NELGEORGES_LIBRARY_DEBUG} optimized ${NELGEORGES_LIBRARY})
    ENDIF(NELGEORGES_LIBRARY_DEBUG)
    
  # Check for NeL Net
  ELSEIF(NL_F_COMPONENT STREQUAL "nelnet")
    FIND_NEL_LIBRARY(NELNET_LIBRARY nelnet nelnet_r)
    FIND_NEL_LIBRARY(NELNET_LIBRARY_DEBUG nelnet_d)
    IF(NOT NELNET_LIBRARY)
      SET(NEL_FOUND "NO")
    ENDIF(NOT NELNET_LIBRARY)
    IF(NELNET_LIBRARY_DEBUG)
		SET(NELNET_LIBRARY debug ${NELNET_LIBRARY_DEBUG} optimized ${NELNET_LIBRARY})
    ENDIF(NELNET_LIBRARY_DEBUG)
    
  # Check for NeL PACS
  ELSEIF(NL_F_COMPONENT STREQUAL "nelpacs")
	FIND_NEL_LIBRARY(NELPACS_LIBRARY nelpacs nelpacs_r)
	FIND_NEL_LIBRARY(NELPACS_LIBRARY_DEBUG nelpacs_d)
    IF(NOT NELPACS_LIBRARY)
      SET(NEL_FOUND "NO")
    ENDIF(NOT NELPACS_LIBRARY)
    IF(NELPACS_LIBRARY_DEBUG)
		SET(NELPACS_LIBRARY debug ${NELPACS_LIBRARY_DEBUG} optimized ${NELPACS_LIBRARY})
    ENDIF(NELPACS_LIBRARY_DEBUG)
    
  # Check for NeL Ligoscape
  ELSEIF(NL_F_COMPONENT STREQUAL "nelligo")
	FIND_NEL_LIBRARY(NELLIGO_LIBRARY nelligo nelligo_r)
	FIND_NEL_LIBRARY(NELLIGO_LIBRARY_DEBUG nelligo_d)
	IF(NOT NELLIGO_LIBRARY)
      SET(NEL_FOUND "NO")
    ENDIF(NOT NELLIGO_LIBRARY)
    IF(NELLIGO_LIBRARY_DEBUG)
		SET(NELLIGO_LIBRARY debug ${NELLIGO_LIBRARY_DEBUG} optimized ${NELLIGO_LIBRARY})
    ENDIF(NELLIGO_LIBRARY_DEBUG)
    
  # Check for NeL Sound Lowlevel
  ELSEIF(NL_F_COMPONENT STREQUAL "nelsnd_lowlevel")
	FIND_NEL_LIBRARY(NELSNDDRV_LIBRARY nelsnd_lowlevel nelsnd_lowlevel_r)
	FIND_NEL_LIBRARY(NELSNDDRV_LIBRARY_DEBUG nelsnd_lowlevel_d)
    IF(NOT NELSNDDRV_LIBRARY)
      SET(NEL_FOUND "NO")
    ENDIF(NOT NELSNDDRV_LIBRARY)
    IF(NELSNDDRV_LIBRARY_DEBUG)
		SET(NELSNDDRV_LIBRARY debug ${NELSNDDRV_LIBRARY_DEBUG} optimized ${NELSNDDRV_LIBRARY})
    ENDIF(NELSNDDRV_LIBRARY_DEBUG)
    
  # Check for NeL Sound
  ELSEIF(NL_F_COMPONENT STREQUAL "nelsound")
	FIND_NEL_LIBRARY(NELSOUND_LIBRARY nelsound nelsound_r)
	FIND_NEL_LIBRARY(NELSOUND_LIBRARY_DEBUG nelsound_d)
	IF(NOT NELSOUND_LIBRARY)
      SET(NEL_FOUND "NO")
    ENDIF(NOT NELSOUND_LIBRARY)
    IF(NELSOUND_LIBRARY_DEBUG)
		SET(NELSOUND_LIBRARY debug ${NELSOUND_LIBRARY_DEBUG} optimized ${NELSOUND_LIBRARY})
    ENDIF(NELSOUND_LIBRARY_DEBUG)
    
  # Output an error message if an unknown component is requested.
  ELSE(NL_F_COMPONENT STREQUAL "nelmisc")
    MESSAGE(ERROR " Unknown component ${NL_F_COMPONENT}!!")
    SET(NEL_FOUND "NO")
  ENDIF(NL_F_COMPONENT STREQUAL "nelmisc")
ENDFOREACH(NL_F_COMPONENT ${NeL_FIND_COMPONENTS})

IF(NEL_FOUND STREQUAL "YES")
  IF(NOT NEL_FIND_QUIETLY)
    MESSAGE(STATUS "Found NeL: ${NELMISC_LIBRARY}")
  ENDIF(NOT NEL_FIND_QUIETLY)
ELSE(NEL_FOUND STREQUAL "YES")
  IF(NOT NEL_FIND_QUIETLY)
    MESSAGE(STATUS "Warning: Unable to find NeL!")
  ENDIF(NOT NEL_FIND_QUIETLY)
ENDIF(NEL_FOUND STREQUAL "YES")
