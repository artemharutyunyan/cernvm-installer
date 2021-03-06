#
# CernVM installer
#
#
# Bulid system is based on:
# CMakeQt: CMake build system template for Qt based projects
# Author: Rohit Yadav <rohityadav89 AT gmail.com>
#

# Project Info
SET(PROJECT_NAME_SHORT   "cernvm-installer")
SET(PROJECT_NAME_LONG    "CernVM Installer")
SET(PROJECT_DESCRIPTION  "Cross-Platform installer for CernVM")
SET(PROJECT_COPYRIGHT    "Copyright (C) 2011 CernVM Project")
SET(PROJECT_CONTACT      "cernvm-talk@cern.ch")
SET(PROJECT_VENDOR       "CERN PH/SFT")
SET(ORG_WEBSITE          "http://cernvm.cern.ch")
 
# Configuring build type
IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "Debug" CACHE STRING
    #SET(CMAKE_BUILD_TYPE "Release" CACHE STRING
        "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
        FORCE)
    MESSAGE("Setting build type to '${CMAKE_BUILD_TYPE}'")
ELSE(NOT CMAKE_BUILD_TYPE)
    MESSAGE("Build type set to '${CMAKE_BUILD_TYPE}'")
ENDIF(NOT CMAKE_BUILD_TYPE)

IF(WIN32)
    # Building in release for WIN32
    SET(CMAKE_BUILD_TYPE "Release")
    SET_PROPERTY(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS_RELEASE QT_NO_DEBUG)
    # Use mingw's sprintf instead of windows's
    ADD_DEFINITIONS( -D__USE_MINGW_ANSI_STDIO=1 )
ENDIF(WIN32)

#  For projects using Git, use git version patch
# IF("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
#     SET(PATCH_OUT "0")
# ELSE("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
#     # Find Git Version Patch   
#     FIND_PROGRAM(GIT git)
#     IF(GIT)
#         EXECUTE_PROCESS(
#             WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#             COMMAND ${GIT} rev-parse --short HEAD 
#             OUTPUT_VARIABLE GIT_OUT OUTPUT_STRIP_TRAILING_WHITESPACE
#         )
#         SET(PATCH_OUT "0-git${GIT_OUT}")
#     ELSE(GIT)
#         SET(PATCH_OUT "0")
#     ENDIF(GIT)
# ENDIF("${CMAKE_BUILD_TYPE}" STREQUAL "Release")

SET(PROJECT_VERSION_PATCH "0")

# Project Version
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "4")
SET(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
SET(CPACK_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
SET(PROJECT_VERSION_MAJOR ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR})
SET(PROJECT_VERSION ${CPACK_PACKAGE_VERSION})

MESSAGE(STATUS "CernVM installer version: ${PROJECT_VERSION}")

STRING(TOLOWER ${PROJECT_NAME_SHORT} PROJECT_NAME_SHORT_LOWER)
PROJECT(${PROJECT_NAME_SHORT_LOWER}-${PROJECT_VERSION})

# Codename of the Project
SET(CODENAME "topsecret")

# System on which project is built
SITE_NAME(HOSTNAME)
SET(SYSNAME ${CMAKE_SYSTEM}-${CMAKE_SYSTEM_PROCESSOR})

# CMake Settings
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)
SET(CMAKE_COLOR_MAKEFILE ON)
SET(CMAKE_VERBOSE_MAKEFILE OFF)

# Set path to additional CMake modules
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

# Path to contribs
SET(CONTRIBS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/contribs)

# On Mac OS build a universal 'release' binary
IF(APPLE)
    #SET(CMAKE_BUILD_TYPE "Release")
    SET(CMAKE_BUILD_TYPE "Debug")
    SET(CMAKE_OSX_ARCHITECTURES i386 x86_64 )
    SET(CMAKE_OSX_DEPLOYMENT_TARGET "10.3")
ENDIF(APPLE)

IF (UNIX)
    SET(WITH_PROFILING FALSE CACHE BOOL "Build with profiling support" )
    SET(CMAKE_INSTALL_PREFIX /usr)
ENDIF(UNIX)

# Search and include Qt4 parameters
# Set Qt min required version and add modules here

IF(NOT WIN32)
  FIND_PACKAGE(Qt4 4.6.0 COMPONENTS QtCore QtGui QtNetwork REQUIRED )
#  FIND_PACKAGE(Threads REQUIRED)
#  FIND_PACKAGE(ZLIB REQUIRED)
#  FIND_PACKAGE(GLIB REQUIRED) 
ELSE (NOT WIN32)
    # FindQt4.cmake is so brain-dead when Xcompiling
    SET(QT_LIBRARY_DIR ${CONTRIBS_PATH}/lib)
    SET(QT_INCLUDE_DIR ${CONTRIBS_PATH}/include/qt4)
    ADD_DEFINITIONS( -I${CONTRIBS_PATH}/include/qt4/QtCore)
    ADD_DEFINITIONS( -I${CONTRIBS_PATH}/include/qt4/QtGui)
    ADD_DEFINITIONS( -I${CONTRIBS_PATH}/include/qt4/QtNetwork)
    ADD_DEFINITIONS( -I${CONTRIBS_PATH}/include/qt4/)
    ADD_DEFINITIONS( -DQT_NO_OPENSSL)
    FIND_PACKAGE(Qt4 4.6.0 COMPONENTS QtCore QtGui QtNetwork REQUIRED )
ENDIF(NOT WIN32)

IF (NOT WIN32)
    #FindQt4.cmake is so brain-dead when Xcompiling
    SET(CVMINSTALLER_LIBS ${QT_QTCORE_LIBRARY}
                          ${QT_QTGUI_LIBRARY}
                          ${QT_QTNETWORK_LIBRARY})
#                          ${CMAKE_THREAD_LIBS_INIT}
#                          -lglib-2.0 -lrt -lgtk-x11-2.0 -lgdk-x11-2.0 -latk-1.0 -lgio-2.0 -lpangoft2-1.0 -lgdk_pixbuf-2.0 -lm -lpangocairo-1.0 -lcairo -lpango-1.0 -lfreetype -lfontconfig -lgobject-2.0 -lgmodule-2.0 -lgthread-2.0 -lrt -lglib-2.0 -ldl
#                          ${ZLIB_LIBRARIES}) # Add here any other QT module or other library
    INCLUDE(${QT_USE_FILE})
    ADD_DEFINITIONS(${QT_DEFINITIONS})
ELSE (NOT WIN32)
    SET (CVMINSTALLER_LIBS # Manually specify paths
                           ${CONTRIBS_PATH}/lib/libQtNetwork.a 
                           ${CONTRIBS_PATH}/lib/libQtGui.a
                           ${CONTRIBS_PATH}/lib/libQtCore.a
                           ws2_32 winmm imm32)
ENDIF(NOT WIN32)

IF (QT_LINK_STATIC)
    #SET(CPACK_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}."-static")
    #SET(PROJECT_VERSION ${CPACK_PACKAGE_VERSION})
    SET (CVMINSTALLER_LIBS # Manually specify paths
                           ${CONTRIBS_PATH}/lib/libQtNetwork.a 
                           ${CONTRIBS_PATH}/lib/libQtGui.a
                           ${CONTRIBS_PATH}/lib/libQtCore.a
			   -lSM -lz -lX11 -lXext -lXrender -lpng -ljpeg -lmng -ltiff -lfontconfig -lpthread -lrt ) 
ENDIF (QT_LINK_STATIC)


# Layout of CernVMInstaller installation
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set common dirs for MAC and UNIX
SET(DEFAULT_BIN_SUBDIR     bin)
SET(DEFAULT_LIB_SUBDIR     lib)
SET(DEFAULT_DATA_SUBDIR    share)
SET(DEFAULT_SRC_SUBDIR     src)
SET(DEFAULT_INCLUDE_SUBDIR include)

# Set CVMSINTALLER path params
SET(CVMINSTALLER_BIN_SUBDIR     "${DEFAULT_BIN_SUBDIR}"        CACHE STRING "Subdirectory where executables will be installed")
SET(CVMINSTALLER_SUBDIR         "${DEFAULT_LIB_SUBDIR}"        CACHE STRING "Subdirectory where libraries will be installed")
SET(CVMINSTALLER_DATA_SUBDIR    "${DEFAULT_DATA_SUBDIR}"       CACHE STRING "Subdirectory where the data files will be installed")
SET(CVMINSTALLER_SRC_SUBDIR     "${DEFAULT_SRC_SUBDIR}"        CACHE STRING "Subdirectory where the source is")
SET(CVMINSTALLER_INCLUDE_SUBDIR "${DEFAULT_INCLUDE_SUBDIR}"    CACHE STRING "Subdirectory where header files will be installed")

# Full paths for the installation
SET(CVMINSTALLER_BIN_DIR     ${CMAKE_INSTALL_PREFIX}/${CVMINSTALLER_BIN_SUBDIR})
SET(CVMINSTALLER_LIB_DIR     ${CMAKE_INSTALL_PREFIX}/${CVMINSTALLER_LIB_SUBDIR})
SET(CVMINSTALLER_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/${CVMINSTALLER_INCLUDE_SUBDIR})
SET(CVMINSTALLER_DESKTOP_DIR ${CMAKE_INSTALL_PREFIX}/${CVMINSTALLER_DATA_SUBDIR}/applications/)
SET(CVMINSTALLER_PIXMAPS_DIR ${CMAKE_INSTALL_PREFIX}/${CVMINSTALLER_DATA_SUBDIR}/pixmaps/)

# Recursively add sub dirs
SUBDIRS(src)
SUBDIRS(nsis)

# Copy the CVMINSTALLER pixmap
INSTALL (FILES share/cernvm-installer.png DESTINATION ${CVMINSTALLER_PIXMAPS_DIR})
INSTALL (FILES share/cernvm-installer.desktop DESTINATION ${CVMINSTALLER_DESKTOP_DIR})
#############################################################
# Enable packaging

INCLUDE(InstallRequiredSystemLibraries)

SET(CPACK_SYSTEM_NAME ${CMAKE_SYSTEM_NAME})
SET(CPACK_PACKAGE_NAME ${PROJECT_NAME_SHORT})
SET(CPACK_PACKAGE_DESCRIPTION "${PROJECT_NAME_SHORT} ${PROJECT_DESCRIPTION}")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_NAME_SHORT} ${PROJECT_DESCRIPTION}")
SET(CPACK_PACKAGE_VENDOR ${PROJECT_VENDOR})
SET(CPACK_PACKAGE_CONTACT ${PROJECT_CONTACT})
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README")
SET(CPACK_RESOURCE_FILE_README ${CMAKE_SOURCE_DIR}/README)
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
SET(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/share/cernvm-installer.png")

# Force Package Name
SET(CPACK_PACKAGE_FILE_NAME ${PROJECT_NAME}-${CPACK_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR})

# NSIS Windows installer
IF(WIN32 AND NOT UNIX)
    # There is a bug in NSI that does not handle full unix paths properly. Make
    # sure there is at least one set of four (4) backslashes.
    SET(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/share\\\\cernvm-installer.png")
    SET(CPACK_NSIS_INSTALLED_ICON_NAME "\\\\cernvm-installer.exe")
    SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} CVMINSTALLER")
    SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\cernvm.cern.ch")
    SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\cernvm.cern.ch")
    SET(CPACK_NSIS_CONTACT "cernvm-talk@cern.ch")
    SET(CPACK_NSIS_MODIFY_PATH ON)
   
    SET(CPACK_PACKAGE_EXECUTABLES "cernvm-installer" "CVMINSTALLLER")
    INSTALL (FILES share/cernvm-installer.png DESTINATION ${CVMINSTALLER_PIXMAPS_DIR})
ENDIF(WIN32 AND NOT UNIX)
    
# RPM packages
INCLUDE(${CMAKE_MODULE_PATH}/RpmBuild.cmake)
IF(RPMBUILD_FOUND AND NOT WIN32)
    SET(CPACK_GENERATOR "RPM")
    SET(CPACK_RPM_PACKAGE_SUMMARY ${CPACK_PACKAGE_DESCRIPTION_SUMMARY})
    SET(CPACK_RPM_PACKAGE_NAME ${CPACK_PACKAGE_NAME})
    SET(CPACK_RPM_PACKAGE_VERSION ${PROJECT_VERSION})
    SET(CPACK_RPM_PACKAGE_LICENSE "BSD License")
    SET(CPACK_RPM_PACKAGE_GROUP "unknown")
    SET(CPACK_RPM_PACKAGE_VENDOR ${PROJECT_VENDOR})
    SET(CPACK_RPM_PACKAGE_DESCRIPTION ${PROJECT_DESCRIPTION})
    SET(CPACK_RPM_PACKAGE_ARCHITECTURE noarch)
    SET(CPACK_RPM_PACKAGE_DEPENDS "qt ( >= 4.6 ), qt-x11 ( >= 4.6 ) ") 
    #SET(CPACK_SET_DESTDIR ON)
ENDIF(RPMBUILD_FOUND AND NOT WIN32)

# Debian packages
INCLUDE (${CMAKE_MODULE_PATH}/DpkgBuild.cmake)
IF(DPKG_FOUND AND NOT WIN32)
    SET(CPACK_GENERATOR "DEB")
    SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION ${CPACK_PACKAGE_DESCRIPTION})
    SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${CPACK_PACKAGE_DESCRIPTION_SUMMARY})
    SET(CPACK_SET_DESTDIR TRUE)
    SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE i386) # Change to your architecture here
    SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt4-core ( >= 4.6 ), libqt4-gui ( >= 4.6 ), libqt4-network ( >= 4.6 )")
ENDIF(DPKG_FOUND AND NOT WIN32)

# Mac App Bundle
IF(APPLE)
    SET(CPACK_GENERATOR "DragNDrop")
    #SET(CPACK_PACKAGE_EXECUTABLES "${CMAKE_CURRENT_BINARY_DIR}/../bin/cernvm-installer" "CVMINSTALLER")#cernvm-installer" "CVMINSTALLLER")
    # Libraries are bundled directly
    SET(CPACK_COMPONENT_LIBRARIES_HIDDEN TRUE)
    # Bundle Properties
    SET(MACOSX_BUNDLE_BUNDLE_NAME cernvm-installer)
    SET(MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION})
    SET(MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION})
    SET(MACOSX_BUNDLE_LONG_VERSION_STRING "Version ${PROJECT_VERSION}")
ENDIF(APPLE)

IF(NOT APPLE)
#SET(CPACK_PACKAGE_EXECUTABLES "cernvm-installer" "CVMINSTALLLER")
ENDIF(NOT APPLE)
INCLUDE(CPack)
