cmake_minimum_required(VERSION 3.1.3)
enable_language(Fortran)

project(SusyKit)
include(ExternalProject)
include(ProcessorCount)

ProcessorCount(NCores)
if(NOT NCores EQUAL 0)
  math(EXPR NCoresPlusOne "${NCores} + 1")
  set(PARMAKE "-j${NCoresPlusOne}")
endif()

enable_language(CXX)
enable_language(C)
enable_language(Fortran)

#set(PROJECT_BINARY_DIR ${PROJECT_SOURCE_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOFTSUSY_VERSION "3.7.2")     #latest
set(FEYNHIGGS_VERSION "2.11.3")   #latest
set(HIGGSBOUNDS_VERSION "5.0.0beta")  #latest
set(SUPERISO_RELIC_VERSION "3.4") #latest
set(MICROMEGAS_VERSION "2.4.5")   #abandon
set(DARKSUSY_VERSION "5.1.2")     #update
set(FEYNHIGGS_MD5 "49f5ea1838cb233baffd85bbc1b0d87d")
set(HIGGSBOUNDS_MD5 "ec4903a77bc35e6ebc0a98278786c31e")
set(SOFTSUSY_MD5 "5efdbf71e43cfdb9c3ef619fe5abff8a")
set(SUPERISO_RELIC_MD5 "97cf83c8bd90f2a9993c2209337f9e72")
set(MICROMEGAS_MD5 "322856e02ddf06c76077b65f3a64de5d")
set(DARKSUSY_MD5 "ee9d47aeb88980e2777a1c61b7d495d0")

ExternalProject_Add(
  SoftSusy
  URL "https://www.hepforge.org/archive/softsusy/softsusy-${SOFTSUSY_VERSION}.tar.gz"
  URL_HASH MD5=${SOFTSUSY_MD5}
  PREFIX "${CMAKE_BINARY_DIR}/work/softsusy"
  DOWNLOAD_DIR "${PROJECT_SOURCE_DIR}/ext/distfiles"
  DOWNLOAD_NO_PROGRESS 1
  SOURCE_DIR "${CMAKE_BINARY_DIR}/work/softsusy/extract"
  # patch not needed after 3.6.2
  # PATCH_COMMAND patch src/softpars.h ${PROJECT_SOURCE_DIR}/patches/softsusy/softpars_h.patch
  CONFIGURE_COMMAND ${CMAKE_BINARY_DIR}/work/softsusy/extract/configure
    CXX=${CMAKE_CXX_COMPILER} --enable-static --with-pic --enable-shared=no
    --prefix=${CMAKE_BINARY_DIR} --includedir=${PROJECT_SOURCE_DIR}/ext/include
  BUILD_COMMAND make
)
add_library(soft STATIC IMPORTED)
set_target_properties(soft PROPERTIES
  IMPORTED_LOCATION "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libsoft.a"
  INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/ext/include"
  IMPORTED_LINK_INTERFACE_LANGUAGES "C;CXX;Fortran"
)


ExternalProject_Add(
  FeynHiggs
  URL "http://wwwth.mpp.mpg.de/members/heinemey/feynhiggs/newversion/FeynHiggs-${FEYNHIGGS_VERSION}.tar.gz"
  URL_HASH MD5=${FEYNHIGGS_MD5}
  PREFIX "${CMAKE_BINARY_DIR}/work/feynhiggs"
  DOWNLOAD_DIR "${PROJECT_SOURCE_DIR}/ext/distfiles"
  DOWNLOAD_NO_PROGRESS 1
  SOURCE_DIR "${CMAKE_BINARY_DIR}/work/feynhiggs/extract"
  PATCH_COMMAND perl -pi -e "s/^LIBDIRSUFFIX = .*/LIBDIRSUFFIX = /" ${CMAKE_BINARY_DIR}/work/feynhiggs/extract/configure
    COMMAND patch -N src/Util/extself.c ${PROJECT_SOURCE_DIR}/patches/feynhiggs/extself_c.patch
  CONFIGURE_COMMAND ${CMAKE_BINARY_DIR}/work/feynhiggs/extract/configure
    --64 --enable-full-g-2 --disable-vt100 CC=${CMAKE_C_COMPILER}
    FC=${CMAKE_Fortran_COMPILER} CXX=${CMAKE_CXX_COMPILER}
    --prefix=${CMAKE_BINARY_DIR}/work/feynhiggs/installed
    FFLAGS="-fPIC" CFLAGS="-fPIC"
  BUILD_COMMAND make lib
  INSTALL_COMMAND make install
    COMMAND install -d ${PROJECT_SOURCE_DIR}/ext/include/feynhiggs
    COMMAND install ${CMAKE_BINARY_DIR}/work/feynhiggs/installed/include/CFeynHiggs.h ${PROJECT_SOURCE_DIR}/ext/include/feynhiggs
    COMMAND install ${CMAKE_BINARY_DIR}/work/feynhiggs/installed/include/CSLHA.h ${PROJECT_SOURCE_DIR}/ext/include/feynhiggs
    COMMAND install ${CMAKE_BINARY_DIR}/work/feynhiggs/installed/include/FHCouplings.h ${PROJECT_SOURCE_DIR}/ext/include/feynhiggs
    COMMAND install ${CMAKE_BINARY_DIR}/work/feynhiggs/installed/include/FHRecord.h ${PROJECT_SOURCE_DIR}/ext/include/feynhiggs
    COMMAND install ${CMAKE_BINARY_DIR}/work/feynhiggs/installed/include/PDG.h ${PROJECT_SOURCE_DIR}/ext/include/feynhiggs
    COMMAND install ${CMAKE_BINARY_DIR}/work/feynhiggs/installed/include/SLHA.h ${PROJECT_SOURCE_DIR}/ext/include/feynhiggs
    COMMAND install ${CMAKE_BINARY_DIR}/work/feynhiggs/installed/include/SLHADefs.h ${PROJECT_SOURCE_DIR}/ext/include/feynhiggs
    COMMAND install ${CMAKE_BINARY_DIR}/work/feynhiggs/installed/lib/libFH.a ${CMAKE_BINARY_DIR}/lib
)
add_library(FH STATIC IMPORTED)
set_target_properties(FH PROPERTIES
  IMPORTED_LOCATION "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libFH.a"
  INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/ext/include"
  IMPORTED_LINK_INTERFACE_LANGUAGES "C;CXX;Fortran"
)

ExternalProject_Add(
  HiggsBounds
  URL "https://www.hepforge.org/archive/higgsbounds/HiggsBounds-${HIGGSBOUNDS_VERSION}.tar.gz"
  URL_HASH MD5=${HIGGSBOUNDS_MD5}
  PREFIX "${CMAKE_CURRENT_BINARY_DIR}/work/higgsbounds"
  DOWNLOAD_DIR "${PROJECT_SOURCE_DIR}/ext/distfiles"
  DOWNLOAD_NO_PROGRESS 1
  SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/work/higgsbounds/extract"
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/work/higgsbounds/extract"
#  PATCH_COMMAND patch -N configure ${PROJECT_SOURCE_DIR}/patches/higgsbounds/configure.patch
  CONFIGURE_COMMAND ${CMAKE_CURRENT_BINARY_DIR}/work/higgsbounds/extract/configure
  BUILD_COMMAND make libHB
  INSTALL_COMMAND install libHB.a ${CMAKE_CURRENT_BINARY_DIR}/lib
)
add_library(HB STATIC IMPORTED)
set_target_properties(HB PROPERTIES
  IMPORTED_LOCATION "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libHB.a"
  IMPORTED_LINK_INTERFACE_LANGUAGES "C;Fortran"
)

ExternalProject_Add(
  SuperIso_Relic
  URL "http://superiso.in2p3.fr/download/superiso_relic_v${SUPERISO_RELIC_VERSION}.tar.bz2"
  URL_HASH MD5=${SUPERISO_RELIC_MD5}
  PREFIX "${CMAKE_BINARY_DIR}/work/superiso_relic"
  DOWNLOAD_DIR "${PROJECT_SOURCE_DIR}/ext/distfiles"
  DOWNLOAD_NO_PROGRESS 1
  SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/work/superiso_relic/extract"
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/work/superiso_relic/extract"
  PATCH_COMMAND patch -N src/include.h ${PROJECT_SOURCE_DIR}/patches/superiso/include_h.patch
  CONFIGURE_COMMAND ${CMAKE_BINARY_DIR}/work/superiso_relic/extract/configure --with-cc=${CMAKE_C_COMPILER} --with-fc=${CMAKE_Fortran_COMPILER}
  BUILD_COMMAND CC=${CMAKE_C_COMPILER} FC=${CMAKE_Fortran_COMPILER} make static
    COMMAND make ${PARMAKE} libisospin.a librelic.a
    COMMAND make libisospin.a librelic.a
  INSTALL_COMMAND install ${CMAKE_CURRENT_BINARY_DIR}/work/superiso_relic/extract/src/libisospin.a ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
    COMMAND install ${CMAKE_CURRENT_BINARY_DIR}/work/superiso_relic/extract/src/relic/librelic.a ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
    COMMAND install -d ${PROJECT_SOURCE_DIR}/ext/include/superiso_relic
    COMMAND install ${CMAKE_CURRENT_BINARY_DIR}/work/superiso_relic/extract/src/include.h ${PROJECT_SOURCE_DIR}/ext/include/superiso_relic
    COMMAND install ${CMAKE_CURRENT_BINARY_DIR}/work/superiso_relic/extract/src/omega.h ${PROJECT_SOURCE_DIR}/ext/include/superiso_relic
)
add_library(isospin STATIC IMPORTED)
set_target_properties(isospin PROPERTIES
  IMPORTED_LOCATION "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libisospin.a"
  INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/ext/include"
  IMPORTED_LINK_INTERFACE_LANGUAGES "C;Fortran"
)
add_library(relic STATIC IMPORTED)
set_target_properties(relic PROPERTIES
  IMPORTED_LOCATION "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/librelic.a"
  INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/ext/include"
  IMPORTED_LINK_INTERFACE_LANGUAGES "C;Fortran"
)

#i just can't...
#ExternalProject_Add(
#  MicrOMEGAs
#  URL "https://lapth.cnrs.fr/micromegas/downloadarea/code/micromegas_${MICROMEGAS_VERSION}.tgz"
#  URL_HASH MD5=${MICROMEGAS_MD5}
#  PREFIX "${CMAKE_CURRENT_BINARY_DIR}/ext/micromegas"
#  DOWNLOAD_DIR "${PROJECT_SOURCE_DIR}/ext/distfiles"
#  DOWNLOAD_NO_PROGRESS 1
#  SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/ext/micromegas/source/"
#  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/ext/micromegas/source/"
#  CONFIGURE_COMMAND make
#  BUILD_COMMAND make -C MSSM
#  INSTALL_COMMAND ""
#)
#include_directories("${CMAKE_CURRENT_BINARY_DIR}/ext/micromegas/source")
#link_directories("${CMAKE_CURRENT_BINARY_DIR}/ext/micromegas/lib")
#set(MICRO_LIBS
#  ${CMAKE_CURRENT_BINARY_DIR}/ext/micromegas/source/sources/micromegas.a
#  ${CMAKE_CURRENT_BINARY_DIR}/ext/micromegas/source/MSSM/work/work_aux.a
#  ${CMAKE_CURRENT_BINARY_DIR}/ext/micromegas/source/MSSM/lib/aLib.a
#  ${CMAKE_CURRENT_BINARY_DIR}/ext/micromegas/source/CalcHEP_Src/lib/dynamic_me.a
#  ${CMAKE_CURRENT_BINARY_DIR}/ext/micromegas/source/CalcHEP_Src/lib/libSLHAplus.a
#  ${CMAKE_CURRENT_BINARY_DIR}/ext/micromegas/source/CalcHEP_Src/lib/num_c.a
#  ${CMAKE_CURRENT_BINARY_DIR}/ext/micromegas/source/CalcHEP_Src/lib/serv.a
#  ${CMAKE_CURRENT_BINARY_DIR}/ext/micromegas/source/CalcHEP_Src/lib/sqme_aux.so
#)

if("${CMAKE_Fortran_COMPILER_ID}" MATCHES "Intel")
  set(DS_FCFLAGS "-O -extend_source")
elseif("${CMAKE_Fortran_COMPILER_ID}" MATCHES "GNU")
  set(DS_FCFLAGS "-O -ffixed-line-length-none")
endif()

ExternalProject_Add(
  DarkSUSY
  URL "http://www.fysik.su.se/~edsjo/darksusy/tars/darksusy-${DARKSUSY_VERSION}.tar.gz"
  URL_HASH MD5=${DARKSUSY_MD5}
  PREFIX "${CMAKE_BINARY_DIR}/work/darksusy"
  DOWNLOAD_DIR "${PROJECT_SOURCE_DIR}/ext/distfiles"
  DOWNLOAD_NO_PROGRESS 1
  SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/work/darksusy/extract"
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/work/darksusy/extract"
  PATCH_COMMAND patch -N src/ini/dsinit.f ${PROJECT_SOURCE_DIR}/patches/darksusy/dsinit.patch
    COMMAND install ${PROJECT_SOURCE_DIR}/ext/include/feynhiggs/SLHA.h  ${CMAKE_CURRENT_BINARY_DIR}/work/darksusy/extract/include
    COMMAND install ${PROJECT_SOURCE_DIR}/ext/include/feynhiggs/SLHADefs.h  ${CMAKE_CURRENT_BINARY_DIR}/work/darksusy/extract/include
    COMMAND install ${PROJECT_SOURCE_DIR}/ext/include/feynhiggs/FHCouplings.h  ${CMAKE_CURRENT_BINARY_DIR}/work/darksusy/extract/include
    COMMAND install ${PROJECT_SOURCE_DIR}/ext/include/feynhiggs/PDG.h  ${CMAKE_CURRENT_BINARY_DIR}/work/darksusy/extract/include
  CONFIGURE_COMMAND ${CMAKE_CURRENT_BINARY_DIR}/work/darksusy/extract/configure
    CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER}
    FC=${CMAKE_Fortran_COMPILER} FCFLAGS=${DS_FCFLAGS}
    --prefix=${CMAKE_CURRENT_BINARY_DIR}
  BUILD_COMMAND make ${PARMAKE} darksusy libisajet
  INSTALL_COMMAND install -d ${PROJECT_SOURCE_DIR}/ext/include/darksusy
    COMMAND make install_tables
    COMMAND sh -c "cp -prf ${CMAKE_CURRENT_BINARY_DIR}/work/darksusy/extract/share/DarkSUSY ${CMAKE_CURRENT_BINARY_DIR}/share/"
    COMMAND sh -c "cp -pr ${CMAKE_CURRENT_BINARY_DIR}/work/darksusy/extract/include/ds*.h ${PROJECT_SOURCE_DIR}/ext/include/darksusy"
    COMMAND install ${CMAKE_CURRENT_BINARY_DIR}/work/darksusy/extract/lib/libdarksusy.a ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
    COMMAND install ${CMAKE_CURRENT_BINARY_DIR}/work/darksusy/extract/lib/libisajet.a ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
)
add_dependencies(DarkSUSY HiggsBounds FeynHiggs)
add_library(darksusy STATIC IMPORTED)
set_target_properties(darksusy PROPERTIES
  IMPORTED_LOCATION "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libdarksusy.a"
  INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/ext/include"
  IMPORTED_LINK_INTERFACE_LANGUAGES "C;CXX;Fortran"
)
add_library(isajet STATIC IMPORTED)
set_target_properties(isajet PROPERTIES
  IMPORTED_LOCATION "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libisajet.a"
  INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/ext/include"
  IMPORTED_LINK_INTERFACE_LANGUAGES "C;CXX;Fortran"
)

set(SOURCES_LCONS
  src/constrain/parseutils.cpp src/constrain/parse.cpp
  src/constrain/model.cpp src/constrain/model_lookup.cpp
  src/constrain/special_lookups.cpp src/constrain/get_slha.cpp
  src/constrain/likeconfig.cpp src/constrain/likedatum.cpp
  src/constrain/loglike.cpp src/constrain/gaussian.cpp
  src/constrain/simple_datum.cpp src/constrain/interpolated_data.cpp
  src/constrain/upper_gaussian.cpp src/constrain/smeared_limit.cpp
  src/constrain/experimental_data.cpp src/constrain/smeared_upper_limit.cpp
  src/constrain/smeared_lower_limit.cpp
)

set(SOURCES_LSKIT
  src/susykit/superiso_interface.cpp src/susykit/softsusy_interface.cpp
  src/susykit/feynhiggs_interface.cpp src/susykit/softsusy_opts.cpp
  src/susykit/darksusy_interface.cpp
  #  src/susykit/micromegas_interface.cpp
)

set(SOURCES_CONSTRAIN
  src/constrain-bin/constraint.cpp src/constrain-bin/constrain.cpp
  src/constrain-bin/constrain_opts.cpp
  src/constrain-bin/datum_constraint.cpp src/constrain-bin/chi2_constraint.cpp
)

set(SOURCES_QPARSE src/qparse/qparse.cpp)

set(SOURCES_SDB2SLHA src/sdb2slha/sdb2slha.cpp)

set(SOURCES_QPOINT src/qpoint/qpoint_softsusy_opts.cpp src/qpoint/qpoint.cpp)

add_library(constrain STATIC ${SOURCES_LCONS})
target_compile_definitions(constrain PRIVATE SOFTSUSY_VERSION=${SOFTSUSY_VERSION})
target_include_directories(constrain PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(susykit STATIC ${SOURCES_LSKIT})
add_dependencies(susykit SoftSusy SuperIso_Relic DarkSUSY FeynHiggs)
target_include_directories(susykit PUBLIC
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/ext/include
)

add_executable(qparse ${SOURCES_QPARSE})
target_link_libraries(qparse constrain)

add_executable(sdb2slha ${SOURCES_SDB2SLHA})
target_link_libraries(sdb2slha constrain)

add_executable(constrain-bin ${SOURCES_CONSTRAIN})
set_target_properties(constrain-bin PROPERTIES OUTPUT_NAME constrain)
target_link_libraries(constrain-bin constrain)

set(SOURCES_SOFTEST
  src/susykit/softsusy_interface.cpp src/susykit/softsusy_opts.cpp
  src/qpoint/qpoint_softsusy_opts.cpp src/testing/softest.cpp
)
add_executable(softest ${SOURCES_SOFTEST})
add_dependencies(softest constrain SoftSusy FeynHiggs)
target_link_libraries(softest constrain soft)

add_executable(qpoint ${SOURCES_QPOINT})
target_link_libraries(qpoint constrain susykit soft darksusy FH isajet HB isospin relic)

set_target_properties(
  constrain
  susykit
  qparse
  sdb2slha
  constrain-bin
  softest
  qpoint
  PROPERTIES
  CXX_STANDARD 14
  CXX_STANDRD_REQUIRED ON
)

add_subdirectory(test)