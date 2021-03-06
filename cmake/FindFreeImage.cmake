#
# Try to find the FreeImage library and include path.
# Once done this will define
#
# FREEIMAGE_FOUND
# FREEIMAGE_INCLUDE_PATH
# FREEIMAGE_LIBRARY
#

if(WIN32)
    find_path(FREEIMAGE_INCLUDE_PATH FreeImage.h
                ${CMAKE_BINARY_DIR}/extern
                DOC "The directory where FreeImage.h resides")
	
    find_library(FREEIMAGE_LIBRARY
                NAMES FreeImage freeimage
                PATHS
                ${CMAKE_BINARY_DIR}/extern
                DOC "The FreeImage library")
else()
    find_path(FREEIMAGE_INCLUDE_PATH FreeImage.h
                /usr/include
                /usr/local/include
                /sw/include
                /opt/local/include
                DOC "The directory where FreeImage.h resides")
    find_library(FREEIMAGE_LIBRARY
                NAMES FreeImage freeimage
                PATHS
                /usr/lib64
                /usr/lib
                /usr/local/lib64
                /usr/local/lib
                /sw/lib
                /opt/local/lib
                DOC "The FreeImage library")
endif()

set(FREEIMAGE_LIBRARIES ${FREEIMAGE_LIBRARY})

if(FREEIMAGE_INCLUDE_PATH AND FREEIMAGE_LIBRARY)
    set(FREEIMAGE_FOUND TRUE CACHE BOOL "Set to TRUE if FreeImage is found, FALSE otherwise")
else()
    set(FREEIMAGE_FOUND FALSE CACHE BOOL "Set to TRUE if FreeImage is found, FALSE otherwise")
endif()

