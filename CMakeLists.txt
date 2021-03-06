# This file is open source software, licensed to you under the terms
# of the Apache License, Version 2.0 (the "License").  See the NOTICE file
# distributed with this work for additional information regarding copyright
# ownership.  You may not use this file except in compliance with the License.
#
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.
#

#
# Copyright (C) 2020 ScyllaDB
#

cmake_minimum_required (VERSION 3.5)

project(parquet4seastar LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

list (APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package (Seastar REQUIRED)
find_package (Snappy REQUIRED)
find_package (ZLIB REQUIRED)
set(MIN_Thrift_VERSION 0.11.0)
find_package (Thrift ${MIN_Thrift_VERSION} REQUIRED)

add_library (parquet4seastar STATIC
    include/parquet4seastar/bit_stream_utils.hh
    include/parquet4seastar/bpacking.hh
    include/parquet4seastar/bytes.hh
    include/parquet4seastar/column_chunk_reader.hh
    include/parquet4seastar/column_chunk_writer.hh
    include/parquet4seastar/compression.hh
    include/parquet4seastar/cql_reader.hh
    include/parquet4seastar/exception.hh
    include/parquet4seastar/encoding.hh
    include/parquet4seastar/file_reader.hh
    include/parquet4seastar/file_writer.hh
    include/parquet4seastar/logical_type.hh
    include/parquet4seastar/overloaded.hh
    include/parquet4seastar/parquet_types.h
    include/parquet4seastar/reader_schema.hh
    include/parquet4seastar/record_reader.hh
    include/parquet4seastar/rle_encoding.hh
    include/parquet4seastar/thrift_serdes.hh
    include/parquet4seastar/writer_schema.hh
    include/parquet4seastar/y_combinator.hh
    src/column_chunk_reader.cc
    src/compression.cc
    src/cql_reader.cc
    src/encoding.cc
    src/file_reader.cc
    src/logical_type.cc
    src/parquet_types.cpp
    src/record_reader.cc
    src/reader_schema.cc
    src/thrift_serdes.cc
    src/writer_schema.cc
)

target_compile_options(parquet4seastar PRIVATE
    -Wall
    -Wextra
    -Wno-unused-parameter
    -Wno-missing-field-initializers
    -fdiagnostics-color=always
)

target_link_libraries(parquet4seastar
    Seastar::seastar
    Thrift::thrift
    ZLIB::ZLIB
    Snappy::snappy
)

target_include_directories (parquet4seastar
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/parquet4seastar)

install(TARGETS parquet4seastar
    EXPORT parquet4seastar-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT parquet4seastar-targets
    FILE
        parquet4seastarTargets.cmake
    NAMESPACE
        parquet4seastar::
    DESTINATION
        ${INSTALL_CONFIGDIR}
)

include(CMakePackageConfigHelpers)
configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/parquet4seastarConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/parquet4seastarConfig.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

install(FILES
    ${CMAKE_CURRENT_LIST_DIR}/cmake/FindThrift.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/parquet4seastarConfig.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)

configure_file(${CMAKE_CURRENT_LIST_DIR}/cmake/FindThrift.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/FindThrift.cmake
    COPYONLY)

export(EXPORT parquet4seastar-targets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/parquet4seastarTargets.cmake
    NAMESPACE parquet4seastar::)

export(PACKAGE parquet4seastar)

enable_testing ()
add_subdirectory (examples)
add_subdirectory (tests)
add_subdirectory (apps)
