cmake_minimum_required(VERSION 3.0.0)
project(SusyKit)
include(ExternalProject)

set(PROJECT_BINARY_DIR ${PROJECT_SOURCE_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOFTSUSY_VERSION "3.6.2")
set(FEYNHIGGS_VERSION "2.11.2")
set(HIGGSBOUNDS_VERSION "4.2.1")
set(SUPERISO_RELIC_VERSION "3.4")
set(MICROMEGAS_VERSION "2.4.5")

ExternalProject_Add(
  SoftSusy
  URL "http://www.hepforge.org/archive/softsusy/softsusy-${SOFTSUSY_VERSION}.tar.gz"
  PREFIX "${CMAKE_CURRENT_BINARY_DIR}/ext/softsusy"
  DOWNLOAD_DIR "${CMAKE_CURRENT_BINARY_DIR}/ext/softsusy/.download"
  DOWNLOAD_NO_PROGRESS 1
  SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/ext/softsusy/source/"
  CONFIGURE_COMMAND ${CMAKE_CURRENT_BINARY_DIR}/ext/softsusy/source/configure CXX=${CMAKE_CXX_COMPILER} --enable-static --prefix=${CMAKE_CURRENT_BINARY_DIR}/ext/softsusy
  BUILD_COMMAND make
)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/ext/softsusy/include)
link_directories("${CMAKE_CURRENT_BINARY_DIR}/ext/softsusy/lib")

ExternalProject_Add(
  FeynHiggs
  URL "http://wwwth.mpp.mpg.de/members/heinemey/feynhiggs/newversion/FeynHiggs-${FEYNHIGGS_VERSION}.tar.gz"
  PREFIX "${CMAKE_CURRENT_BINARY_DIR}/ext/feynhiggs"
  DOWNLOAD_DIR "${CMAKE_CURRENT_BINARY_DIR}/ext/feynhiggs/.download"
  DOWNLOAD_NO_PROGRESS 1
  SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/ext/feynhiggs/source/"
  CONFIGURE_COMMAND ${CMAKE_CURRENT_BINARY_DIR}/ext/feynhiggs/source/configure --static --64 --enable-full-g-2 --disable-vt100 CC=${CMAKE_C_COMPILER} --prefix=${CMAKE_CURRENT_BINARY_DIR}/ext/feynhiggs/ds
  BUILD_COMMAND make
)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/ext/feynhiggs/ds/include)
file(GLOB FH_LIB_DIR ${CMAKE_CURRENT_BINARY_DIR}/ext/feynhiggs/ds/lib*)
link_directories(${FH_LIB_DIR})

ExternalProject_Add(
  HiggsBounds
  URL "http://www.hepforge.org/archive/higgsbounds/HiggsBounds-${HIGGSBOUNDS_VERSION}.tar.gz"
  PREFIX "${CMAKE_CURRENT_BINARY_DIR}/ext/higgsbounds"
  DOWNLOAD_DIR "${CMAKE_CURRENT_BINARY_DIR}/ext/higgsbounds/.download"
  DOWNLOAD_NO_PROGRESS 1
  SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/ext/higgsbounds/source/"
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/ext/higgsbounds/source/"
  CONFIGURE_COMMAND ${CMAKE_CURRENT_BINARY_DIR}/ext/higgsbounds/source/configure 
  BUILD_COMMAND make libHB
  INSTALL_COMMAND install -d ${CMAKE_CURRENT_BINARY_DIR}/ext/higgsbounds/lib COMMAND install libHB.a ${CMAKE_CURRENT_BINARY_DIR}/ext/higgsbounds/lib
)
link_directories("${CMAKE_CURRENT_BINARY_DIR}/ext/higgsbounds/lib")

ExternalProject_Add(
  SuperIso_Relic
  URL "http://superiso.in2p3.fr/download/superiso_relic_v${SUPERISO_RELIC_VERSION}.tar.bz2"
  PREFIX "${CMAKE_CURRENT_BINARY_DIR}/ext/superiso_relic"
  DOWNLOAD_DIR "${CMAKE_CURRENT_BINARY_DIR}/ext/superiso_relic/.download"
  DOWNLOAD_NO_PROGRESS 1
  SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/ext/superiso_relic/source/"
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/ext/superiso_relic/source/"
  CONFIGURE_COMMAND ${CMAKE_CURRENT_BINARY_DIR}/ext/superiso_relic/source/configure --with-cc=${CMAKE_C_COMPILER} --with-fc=gfortran
  BUILD_COMMAND make static COMMAND make libisospin.a INSTALL_COMMAND install -d ${CMAKE_CURRENT_BINARY_DIR}/ext/superiso_relic/lib COMMAND install ${CMAKE_CURRENT_BINARY_DIR}/ext/superiso_relic/source/src/libisospin.a ${CMAKE_CURRENT_BINARY_DIR}/ext/superiso_relic/lib COMMAND install -d ${CMAKE_CURRENT_BINARY_DIR}/ext/superiso_relic/include COMMAND install ${CMAKE_CURRENT_BINARY_DIR}/ext/superiso_relic/source/src/include.h ${CMAKE_CURRENT_BINARY_DIR}/ext/superiso_relic/include COMMAND install ${CMAKE_CURRENT_BINARY_DIR}/ext/superiso_relic/source/src/omega.h ${CMAKE_CURRENT_BINARY_DIR}/ext/superiso_relic/include
)
include_directories("${CMAKE_CURRENT_BINARY_DIR}/ext/superiso_relic/include")
link_directories("${CMAKE_CURRENT_BINARY_DIR}/ext/superiso_relic/lib")

ExternalProject_Add(
  MicrOMEGAs
  URL "https://lapth.cnrs.fr/micromegas/downloadarea/code/micromegas_${MICROMEGAS_VERSION}.tgz"
  PREFIX "${CMAKE_CURRENT_BINARY_DIR}/ext/micromegas"
  DOWNLOAD_DIR "${CMAKE_CURRENT_BINARY_DIR}/ext/micromegas/.download"
  DOWNLOAD_NO_PROGRESS 1
  SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/ext/micromegas/source/"
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/ext/micromegas/source/"
  CONFIGURE_COMMAND make
  BUILD_COMMAND make -C MSSM
  INSTALL_COMMAND ""
)
include_directories("${CMAKE_CURRENT_BINARY_DIR}/ext/micromegas/source")
#link_directories("${CMAKE_CURRENT_BINARY_DIR}/ext/higgsbounds/lib")

set(SOURCES_LCONS 
  src/parseutils.cpp src/parse.cpp src/model.cpp src/model_lookup.cpp
  src/special_lookups.cpp src/get_slha.cpp src/likeconfig.cpp src/likedatum.cpp
  src/loglike.cpp
)

set(SOURCES_LSKIT 
  src/superiso_interface.cpp src/softsusy_interface.cpp
  src/feynhiggs_interface.cpp src/softsusy_opts.cpp
  src/micromegas_interface.cpp
)

set(SOURCES_CONSTRAIN 
  src/constraint.cpp src/constrain.cpp src/constrain_opts.cpp
)

set(SOURCES_QPARSE src/qparse.cpp)

set(SOURCES_SDB2SLHA src/sdb2slha.cpp)

set(SOURCES_QPOINT src/qpoint_softsusy_opts.cpp src/qpoint.cpp)

add_library(constrain STATIC ${SOURCES_LCONS})
target_compile_features(constrain PRIVATE cxx_range_for)
target_compile_features(constrain PRIVATE cxx_generalized_initializers)

add_library(susykit STATIC ${SOURCES_LSKIT})
target_compile_features(susykit PRIVATE cxx_range_for)
target_compile_features(susykit PRIVATE cxx_generalized_initializers)

add_executable(qparse ${SOURCES_QPARSE})
target_compile_features(qparse PRIVATE cxx_range_for)
target_link_libraries(qparse constrain)

add_executable(sdb2slha ${SOURCES_SDB2SLHA})
target_compile_features(sdb2slha PRIVATE cxx_range_for)
target_link_libraries(sdb2slha constrain)

add_executable(constrain-bin ${SOURCES_CONSTRAIN})
set_target_properties(constrain-bin PROPERTIES OUTPUT_NAME constrain)
target_compile_features(constrain-bin PRIVATE cxx_range_for)
target_link_libraries(constrain-bin constrain)

set(SOURCES_SOFTEST
  src/softsusy_interface.cpp src/softsusy_opts.cpp src/qpoint_softsusy_opts.cpp
  src/softest.cpp
)
add_executable(softest ${SOURCES_SOFTEST})
target_compile_features(softest PRIVATE cxx_range_for)
target_link_libraries(softest constrain soft)

add_executable(qpoint ${SOURCES_QPOINT})
target_compile_features(qpoint PRIVATE cxx_range_for)
target_link_libraries(qpoint constrain susykit FH)

