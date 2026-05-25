##
# CRootWriter - COLA Library Module for ROOT data storage support.
# Copyright (C) 2025-2026 Savva Savenkov
#
# This file is part of CRoot
#
# CRootWriter is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# CRootWriter is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with CRootWriter.  If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.22)
project(
    CRootWriter
    VERSION 1.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)

find_package(COLA REQUIRED)
find_package(ROOT REQUIRED)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CMAKE_INSTALL_PREFIX ${COLA_DIR})
set(COLA_MODULE_NAME CRootWriter)

set(COLA_CMAKE_CXX_CLANG_TIDY
    ""
    CACHE STRING "Passed to CMAKE_CXX_CLANG_TIDY for ${COLA_MODULE_NAME} targets"
)

set(UNIGEN_SRCS UniGen/src/UEvent.cxx UniGen/src/UParticle.cxx UniGen/src/URun.cxx)

set(SRCS
    CRootWriter/CRootWriterFactory.cc
    CRootWriter/CRootWriter.cc
    CUniGenWriter/CUniGenWriter.cc
    CNativeRootWriter/CNativeRootWriter.cc
    CAAMCCWriter/CAAMCCWriter.cc
    CRootModule.cc
    ${UNIGEN_SRCS}
)
add_library(CRootWriter SHARED ${SRCS})

target_compile_features(CRootWriter PUBLIC cxx_std_17)

target_include_directories(
    CRootWriter
    PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>"
           "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/CRootWriter>"
           "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/UniGen/include>"
           "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/CUniGenWriter>"
           "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/CNativeRootWriter>"
           "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/CAAMCCWriter>"
           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${COLA_MODULE_NAME}>"
)

set(CRootWriter_PUBLIC_HEADER
    "CRootWriter/CRootWriter.hh;CRootWriter/CRootWriterFactory.hh;\
UniGen/include/UEvent.hh;UniGen/include/UParticle.hh;UniGen/include/URun.hh;CUniGenWriter/CUniGenWriter.hh;CNativeRootWriter/CNativeRootWriter.hh;\
CAAMCCWriter/CAAMCCWriter.hh;CRootModule.hh"
)

set_target_properties(
    CRootWriter
    PROPERTIES PUBLIC_HEADER "${CRootWriter_PUBLIC_HEADER}"
               CXX_CLANG_TIDY "${COLA_CMAKE_CXX_CLANG_TIDY}"
               INSTALL_RPATH_USE_LINK_PATH ON
               BUILD_WITH_INSTALL_RPATH TRUE
)

# don't lint external UniGen code
set_source_files_properties(${UNIGEN_SRCS} PROPERTIES SKIP_LINTING TRUE)

# Generate dictionaries for I/O classes
root_generate_dictionary(
    Dict_CRootWriter
    ${COLA_DIR}/include/COLA.hh
    ${COLA_DIR}/include/LorentzVector.hh
    UniGen/include/URun.hh
    UniGen/include/UEvent.hh
    UniGen/include/UParticle.hh
    LINKDEF
    LinkDef.hh
    MODULE
    CRootWriter
)

target_link_libraries(CRootWriter PUBLIC COLA ROOT::RIO ${ROOT_LIBRARIES})

# Check for COLA_FETCH_MODE to prevent install instructions from generating
if (NOT COLA_FETCH_MODE)
    # Configure config files
    configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/cmake/${COLA_MODULE_NAME}Config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/${COLA_MODULE_NAME}Config.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${COLA_MODULE_NAME}
    )

    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/${COLA_MODULE_NAME}ConfigVersion.cmake
        VERSION ${CRootWriter_Version}
        COMPATIBILITY AnyNewerVersion
    )

    # Install library
    install(
        TARGETS CRootWriter
        EXPORT CRootWriterExport
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${COLA_MODULE_NAME}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${COLA_MODULE_NAME}
        INCLUDES
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${COLA_MODULE_NAME}
    )

    # Install export file and config files
    install(EXPORT CRootWriterExport DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${COLA_MODULE_NAME})

    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${COLA_MODULE_NAME}Config.cmake"
                  "${CMAKE_CURRENT_BINARY_DIR}/${COLA_MODULE_NAME}ConfigVersion.cmake"
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${COLA_MODULE_NAME}
    )

    # Install ROOT dicts for I/O
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/lib${COLA_MODULE_NAME}_rdict.pcm"
                  "${CMAKE_CURRENT_BINARY_DIR}/lib${COLA_MODULE_NAME}.rootmap"
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/${COLA_MODULE_NAME}
    )

    setup_cola_quality_tools(EXCLUDE_PATTERNS ".*/UniGen/.*" ".*/COLA/.*")
endif ()
