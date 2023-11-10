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

find_package(
  Qt5
  COMPONENTS DBus)

include(./cmake/PFL.cmake)

pfl_init()

if(PROEJCTTEMPLATE_ENABLE_TESTING)
  enable_testing()
endif()

# build library ProjectTemplate
pfl_add_library(
  INTERNAL
  VERSION
  ${PROJECT_VERSION}
  INS
  ./src/QDBusP2PExample/configure.hpp.in
  SOURCES
  ./src/QDBusP2PExample/Server.hpp
  ./src/QDBusP2PExample/Server.cpp
  ./src/QDBusP2PExample/Client.hpp
  ./src/QDBusP2PExample/Client.cpp
  APPS
  server
  client
  COMPILE_FEATURES
  PUBLIC
  cxx_std_17)
