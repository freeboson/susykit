cmake_minimum_required(VERSION 3.0.0)
enable_language(Fortran)

project(SusyKit)
include(ExternalProject)
enable_language(CXX)
enable_language(C)
enable_language(Fortran)

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
set(DARKSUSY_VERSION "5.1.2")
set(FEYNHIGGS_MD5 "edb73eafa6dab291bd8827242c16ac0a")
set(HIGGSBOUNDS_MD5 "47b93330d4e0fddcc23b381548db355b")
set(SOFTSUSY_MD5 "b7f7b76424b168d23e0afd6fbd063da2")
set(SUPERISO_RELIC_MD5 "97cf83c8bd90f2a9993c2209337f9e72")
set(MICROMEGAS_MD5 "322856e02ddf06c76077b65f3a64de5d")
set(DARKSUSY_MD5 "ee9d47aeb88980e2777a1c61b7d495d0")

ExternalProject_Add(
  SoftSusy
  URL "http://www.hepforge.org/archive/softsusy/softsusy-${SOFTSUSY_VERSION}.tar.gz"
  URL_HASH MD5=${SOFTSUSY_MD5}
  PREFIX "${CMAKE_CURRENT_BINARY_DIR}/ext/softsusy"
  DOWNLOAD_DIR "${CMAKE_CURRENT_BINARY_DIR}/distfiles"
  DOWNLOAD_NO_PROGRESS 1
  SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/ext/softsusy/source/"
  # patch not needed after 3.6.2
  PATCH_COMMAND patch src/softpars.h ${CMAKE_CURRENT_BINARY_DIR}/patches/softsusy/softpars_h.patch
  CONFIGURE_COMMAND ${CMAKE_CURRENT_BINARY_DIR}/ext/softsusy/source/configure
    CXX=${CMAKE_CXX_COMPILER} --enable-static
    --prefix=${CMAKE_CURRENT_BINARY_DIR}/ext/softsusy
  BUILD_COMMAND make
)
add_library(soft STATIC IMPORTED)
set_target_properties(soft PROPERTIES
  IMPORTED_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/ext/softsusy/lib/libsoft.a"
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_BINARY_DIR}/ext/softsusy/include"
  IMPORTED_LINK_INTERFACE_LANGUAGES "C;CXX;Fortran"
)


ExternalProject_Add(
  FeynHiggs
  URL "http://wwwth.mpp.mpg.de/members/heinemey/feynhiggs/newversion/FeynHiggs-${FEYNHIGGS_VERSION}.tar.gz"
  URL_HASH MD5=${FEYNHIGGS_MD5}
  PREFIX "${CMAKE_CURRENT_BINARY_DIR}/ext/feynhiggs"
  DOWNLOAD_DIR "${CMAKE_CURRENT_BINARY_DIR}/distfiles"
  DOWNLOAD_NO_PROGRESS 1
  SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/ext/feynhiggs/source/"
  CONFIGURE_COMMAND ${CMAKE_CURRENT_BINARY_DIR}/ext/feynhiggs/source/configure
    --static --64 --enable-full-g-2 --disable-vt100 CC=${CMAKE_C_COMPILER}
    --prefix=${CMAKE_CURRENT_BINARY_DIR}/ext/feynhiggs/ds BUILD_COMMAND make
)
file(GLOB FH_LIB_DIR ${CMAKE_CURRENT_BINARY_DIR}/ext/feynhiggs/ds/lib*)
add_library(FH STATIC IMPORTED)
set_target_properties(FH PROPERTIES
  IMPORTED_LOCATION "${FH_LIB_DIR}/libFH.a"
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_BINARY_DIR}/ext/feynhiggs/ds/include"
  IMPORTED_LINK_INTERFACE_LANGUAGES "C;Fortran"
)

ExternalProject_Add(
  HiggsBounds
  URL "http://www.hepforge.org/archive/higgsbounds/HiggsBounds-${HIGGSBOUNDS_VERSION}.tar.gz"
  URL_HASH MD5=${HIGGSBOUNDS_MD5}
  PREFIX "${CMAKE_CURRENT_BINARY_DIR}/ext/higgsbounds"
  DOWNLOAD_DIR "${CMAKE_CURRENT_BINARY_DIR}/distfiles"
  DOWNLOAD_NO_PROGRESS 1
  SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/ext/higgsbounds/source/"
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/ext/higgsbounds/source/"
  CONFIGURE_COMMAND ${CMAKE_CURRENT_BINARY_DIR}/ext/higgsbounds/source/configure 
  BUILD_COMMAND make libHB
  INSTALL_COMMAND install -d ${CMAKE_CURRENT_BINARY_DIR}/ext/higgsbounds/lib
    COMMAND install libHB.a ${CMAKE_CURRENT_BINARY_DIR}/ext/higgsbounds/lib
)
link_directories("${CMAKE_CURRENT_BINARY_DIR}/ext/higgsbounds/lib")

ExternalProject_Add(
  SuperIso_Relic
  URL "http://superiso.in2p3.fr/download/superiso_relic_v${SUPERISO_RELIC_VERSION}.tar.bz2"
  URL_HASH MD5=${SUPERISO_RELIC_MD5}
  PREFIX "${CMAKE_CURRENT_BINARY_DIR}/ext/superiso_relic"
  DOWNLOAD_DIR "${CMAKE_CURRENT_BINARY_DIR}/distfiles"
  DOWNLOAD_NO_PROGRESS 1
  SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/ext/superiso_relic/source/"
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/ext/superiso_relic/source/"
  PATCH_COMMAND patch src/include.h ${CMAKE_CURRENT_BINARY_DIR}/patches/superiso/include_h.patch
  CONFIGURE_COMMAND ${CMAKE_CURRENT_BINARY_DIR}/ext/superiso_relic/source/configure
    --with-cc=${CMAKE_C_COMPILER} --with-fc=${CMAKE_Fortran_COMPILER}
  BUILD_COMMAND make static
    COMMAND make libisospin.a
  INSTALL_COMMAND install -d ${CMAKE_CURRENT_BINARY_DIR}/ext/superiso_relic/lib
    COMMAND install ${CMAKE_CURRENT_BINARY_DIR}/ext/superiso_relic/source/src/libisospin.a
      ${CMAKE_CURRENT_BINARY_DIR}/ext/superiso_relic/lib
    COMMAND install -d ${CMAKE_CURRENT_BINARY_DIR}/ext/superiso_relic/include
    COMMAND install ${CMAKE_CURRENT_BINARY_DIR}/ext/superiso_relic/source/src/include.h
      ${CMAKE_CURRENT_BINARY_DIR}/ext/superiso_relic/include
    COMMAND install ${CMAKE_CURRENT_BINARY_DIR}/ext/superiso_relic/source/src/omega.h
      ${CMAKE_CURRENT_BINARY_DIR}/ext/superiso_relic/include
)
add_library(isospin STATIC IMPORTED)
set_target_properties(isospin PROPERTIES
  IMPORTED_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/ext/superiso_relic/lib/libisospin.a"
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_BINARY_DIR}/ext/superiso_relic/include"
  IMPORTED_LINK_INTERFACE_LANGUAGES "C;Fortran"
)

ExternalProject_Add(
  MicrOMEGAs
  URL "https://lapth.cnrs.fr/micromegas/downloadarea/code/micromegas_${MICROMEGAS_VERSION}.tgz"
  URL_HASH MD5=${MICROMEGAS_MD5}
  PREFIX "${CMAKE_CURRENT_BINARY_DIR}/ext/micromegas"
  DOWNLOAD_DIR "${CMAKE_CURRENT_BINARY_DIR}/distfiles"
  DOWNLOAD_NO_PROGRESS 1
  SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/ext/micromegas/source/"
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/ext/micromegas/source/"
  CONFIGURE_COMMAND make
  BUILD_COMMAND make -C MSSM
  INSTALL_COMMAND ""
)
include_directories("${CMAKE_CURRENT_BINARY_DIR}/ext/micromegas/source")
link_directories("${CMAKE_CURRENT_BINARY_DIR}/ext/micromegas/lib")
set(MICRO_LIBS
  ${CMAKE_CURRENT_BINARY_DIR}/ext/micromegas/source/sources/micromegas.a
  ${CMAKE_CURRENT_BINARY_DIR}/ext/micromegas/source/MSSM/work/work_aux.a
  ${CMAKE_CURRENT_BINARY_DIR}/ext/micromegas/source/MSSM/lib/aLib.a
  ${CMAKE_CURRENT_BINARY_DIR}/ext/micromegas/source/CalcHEP_Src/lib/dynamic_me.a
  ${CMAKE_CURRENT_BINARY_DIR}/ext/micromegas/source/CalcHEP_Src/lib/libSLHAplus.a
  ${CMAKE_CURRENT_BINARY_DIR}/ext/micromegas/source/CalcHEP_Src/lib/num_c.a
  ${CMAKE_CURRENT_BINARY_DIR}/ext/micromegas/source/CalcHEP_Src/lib/serv.a
  ${CMAKE_CURRENT_BINARY_DIR}/ext/micromegas/source/CalcHEP_Src/lib//sqme_aux.so
)

if("${CMAKE_Fortran_COMPILER_ID}" MATCHES "Intel")
  set(DS_FCFLAGS "-O -extend_source")
elseif("${CMAKE_Fortran_COMPILER_ID}" MATCHES "GNU")
  set(DS_FCFLAGS "-O -ffixed-line-length-none")
endif()

ExternalProject_Add(
  DarkSUSY
  URL "http://www.fysik.su.se/~edsjo/darksusy/tars/darksusy-${DARKSUSY_VERSION}.tar.gz"
  URL_HASH MD5=${DARKSUSY_MD5}
  PREFIX "${CMAKE_CURRENT_BINARY_DIR}/ext/darksusy"
  DOWNLOAD_DIR "${CMAKE_CURRENT_BINARY_DIR}/distfiles"
  DOWNLOAD_NO_PROGRESS 1
  SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/ext/darksusy/source/"
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/ext/darksusy/source/"
  PATCH_COMMAND patch src/ini/dsinit.f ${CMAKE_CURRENT_BINARY_DIR}/patches/darksusy/dsinit.patch
  CONFIGURE_COMMAND ${CMAKE_CURRENT_BINARY_DIR}/ext/darksusy/source/configure
    CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER}
    FC=${CMAKE_Fortran_COMPILER} FCFLAGS=${DS_FCFLAGS}
    --prefix=${CMAKE_CURRENT_BINARY_DIR}/ext/darksusy
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
)

set(SOURCES_LCONS 
  src/parseutils.cpp src/parse.cpp src/model.cpp src/model_lookup.cpp
  src/special_lookups.cpp src/get_slha.cpp src/likeconfig.cpp src/likedatum.cpp
  src/loglike.cpp
)

set(SOURCES_LSKIT 
  src/superiso_interface.cpp src/softsusy_interface.cpp
  src/feynhiggs_interface.cpp src/softsusy_opts.cpp
  src/darksusy_interface.cpp
  #  src/micromegas_interface.cpp
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
target_link_libraries(susykit soft FH isospin)

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
target_link_libraries(qpoint constrain susykit)

