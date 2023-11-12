cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

project(
  QDBusP2PExample
  VERSION 0.1.0
  DESCRIPTION "An example about how to use QDBus in peer to peer mode."
  HOMEPAGE_URL "https://github.com/black-desk/cpp-template"
  LANGUAGES CXX)

include(./cmake/GitSemver.cmake)

gitsemver(PROJECT_VERSION)

include(./cmake/CPM.cmake)

CPMFindPackage(
  NAME expected
  VERSION 1.0.0
  GITHUB_REPOSITORY TartanLlama/expected
  GIT_TAG v1.0.0
  GIT_SHALLOW ON
  OPTIONS "EXPECTED_BUILD_TESTS OFF"
  FIND_PACKAGE_ARGUMENTS "NAMES tl-expected"
  EXCLUDE_FROM_ALL ON)

find_package(
  Qt5
  COMPONENTS DBus)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include(./cmake/PFL.cmake)

pfl_init()

if(PROEJCTTEMPLATE_ENABLE_TESTING)
  enable_testing()
endif()

pfl_add_library(
  INTERNAL
  VERSION
  ${PROJECT_VERSION}
  INS
  ./src/QDBusP2PExample/configure.hpp.in
  SOURCES
  ./src/QDBusP2PExample/Server.hpp
  ./src/QDBusP2PExample/Server.cpp
  ./src/QDBusP2PExample/dbus.hpp
  ./src/QDBusP2PExample/dbus.cpp
  ./src/QDBusP2PExample/init.hpp
  ./src/QDBusP2PExample/init.cpp
  APPS
  server
  client
  COMPILE_FEATURES
  PUBLIC
  cxx_std_17
  LINK_LIBRARIES
  PUBLIC
  Qt::DBus
  tl::expected)

function(
  qtdbusp2pexample_add_dbus_adaptor
  target_name
  xml
  includes
  parent_class
  basename
  classname)
  set(DBUS_ADAPTOR_SOURCE)

  qt_add_dbus_adaptor(DBUS_ADAPTOR_SOURCE ${xml} ${includes} ${parent_class}
                      ${basename} ${classname})
  # FIXME(black_desk): `qt_add_dbus_adaptor` will use basename as include and
  # this cannot be configured at all. This is just a workaround.
  target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
  target_sources(${target_name} PRIVATE ${DBUS_ADAPTOR_SOURCE})
endfunction()

qtdbusp2pexample_add_dbus_adaptor(
  QDBusP2PExample
  ${PROJECT_SOURCE_DIR}/api/dbus/io.github.blackdesk.QDBusP2PExample.Server.xml
  QDBusP2PExample/Server.hpp
  QDBusP2PExample::Server
  src/QDBusP2PExample/ServerAdaptor
  ServerAdaptor)

function(qtdbusp2pexample_add_dbus_interface target xml basename) # include
  set_source_files_properties(${xml} NO_NAMESPACE ON)

  set(include "${ARGV3}")
  if(include)
    set_source_files_properties(${xml} PROPERTIES INCLUDE ${include})
  endif()

  set(INTERFACE_SOURCES)
  set(Qt5DBus_QDBUSXML2CPP_EXECUTABLE qdbusxml2cpp)
  qt_add_dbus_interface(INTERFACE_SOURCES ${xml} ${basename})
  target_sources(${target} PRIVATE ${INTERFACE_SOURCES})
endfunction()

qtdbusp2pexample_add_dbus_interface(
  QDBusP2PExample
  ${PROJECT_SOURCE_DIR}/api/dbus/io.github.blackdesk.QDBusP2PExample.Server.xml
  ./src/QDBusP2PExample/io_github_blackdesk_QDBusP2PExample_Server)
