macro(SET_COMPILER_DEBUG_FLAG flag)
    CHECK_CXX_COMPILER_FLAG(${flag} "HAVE_CXX_COMPILER_FLAG${flag}")
    if("HAVE_CXX_COMPILER_FLAG${flag}")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${flag}")
    else()
        message(STATUS "Compiler flag ${flag} is not supported")
    endif()
endmacro()

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    add_definitions(-D_DEBUG)
    if(CMAKE_SYSTEM_NAME STREQUAL Linux)
        SET_COMPILER_DEBUG_FLAG(-O0)
        SET_COMPILER_DEBUG_FLAG(-fvisibility=default)
        SET_COMPILER_DEBUG_FLAG(-Wall)
        SET_COMPILER_DEBUG_FLAG(-Wextra)
        SET_COMPILER_DEBUG_FLAG(-Wconversion)
        SET_COMPILER_DEBUG_FLAG(-fno-inline)
        # setting linker warning flags
        set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--warn-common")
    endif()

    message(STATUS "Compiler flags are: ${CMAKE_CXX_FLAGS_DEBUG}")
endif()
