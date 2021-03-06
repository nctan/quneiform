project(cuneiform C CXX)
cmake_minimum_required(VERSION 2.6.0)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")
 
enable_testing()

include(outtree)
include(build_number)
include(version)
include(uninstall)
include(update_samples)
include(options)
include(find_shared_memory)
include(code_check)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
if(WIN32)
  set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
endif()

include(CheckCXXCompilerFlag)

include(msvc)
include(flags_common)
include(flags_release)
include(flags_debug)
include(install_files)
include(library_link)
include(find_image_libs)
include(man_pages)
    
# Iconv
find_package(Iconv)
if(NOT ICONV_FOUND)
    message(FATAL_ERROR "Iconv library required")
endif()

# PDF poppler
find_package(Poppler)
if(POPPLER_FOUND)
    set(CF_PDF_SUPPORT ON)
endif()

# TIFF
find_package(TIFF)
if(TIFF_FOUND)
    set(CF_TIFF_SUPPORT ON)
endif()

# Boost
if(WIN32)
    set(Boost_USE_STATIC_LIBS OFF)
	
	if(MSVC)
        add_definitions(-DBOOST_ALL_NO_LIB)
	endif()
endif()

find_package(Boost 1.41)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    find_package(Boost 1.41 COMPONENTS serialization)

    if(Boost_SERIALIZATION_FOUND)
        set(CF_SERIALIZE True)
        set(CF_USE_SHMEM True)
    else()
        message(STATUS "Boost serialization library not found.")
        set(CF_USE_SHMEM False)
        message(STATUS "Shared memory extensions will not build.")
    endif()
else()
    message(FATAL_ERROR "Boost library required")
endif()

include(use_doxygen)
include(system_info)
include(make_config)
include(make_package)
include(scanner)

add_subdirectory(datafiles)
add_subdirectory(contrib)
add_subdirectory(src)
add_subdirectory(cli)
add_subdirectory(api)

if(CF_NO_GUI)
    message(STATUS "No GUI will be build")
    set(CF_GUI False)
else()
    add_subdirectory(gui)
    message(STATUS "Qt GUI will be build")
    set(CF_GUI True)
endif()

add_subdirectory(utils)
add_subdirectory(tests)

include(summary)
