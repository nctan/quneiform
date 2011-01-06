set(CPACK_SYSTEM_NAME i686)
set(CPACK_PACKAGE_VENDOR "Serj Poltavski")
set(CPACK_PACKAGE_CONTACT "serj.poltavski@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${CPACK_PACKAGE_NAME} - Optical Recognition Software")
set(CPACK_PACKAGE_VERSION "${CF_VERSION_MAJOR}.${CF_VERSION_MINOR}.${CF_VERSION_PATCH}")
set(CPACK_PACKAGE_VERSION_MAJOR ${CF_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${CF_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${CF_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING.txt")

if(APPLE)
    set(CPACK_GENERATOR "PackageMaker")
    set(CPACK_PACKAGE_NAME "Quneiform")
    set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/doc/README.mac.txt")
    set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_SOURCE_DIR}/doc/WELCOME.mac.txt")
elseif(UNIX AND NOT APPLE)
    set(CPACK_GENERATOR "DEB;TBZ2;TGZ")
    set(CPACK_STRIP_FILES "bin/cuneiform")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Cuneiform optical recognition software")
    set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/readme.txt")
    # Debian
    set(CPACK_DEBIAN_PACKAGE_SECTION "graphics")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Serj Poltavski <serj.poltavski@gmail.com>")
    set(CF_DEB_DEPENDS "libc6 (>= 2.4), libgcc1 (>= 1:4.1.1), libstdc++6 (>= 4.4.0)")
    if(CF_USE_ZIP)
        set(CF_DEB_DEPENDS "${CF_DEB_DEPENDS}, libzip (>= 0.9)")
    endif()
    if(CF_SERIALIZE)
        set(CF_DEB_DEPENDS "${CF_DEB_DEPENDS}, libboost-serialization (>= 1.40)")
    endif()
    set(CPACK_DEBIAN_PACKAGE_DEPENDS ${CF_DEB_DEPENDS})
elseif(WIN32)
 # no yet
endif()


include(CPack)
