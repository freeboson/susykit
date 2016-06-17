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

set(SOFTSUSY_VERSION "3.7.2")     #latest
set(FEYNHIGGS_VERSION "2.11.3")   #latest
set(HIGGSBOUNDS_VERSION "4.2.1")  #update
set(SUPERISO_RELIC_VERSION "3.4") #latest
set(MICROMEGAS_VERSION "2.4.5")   #abandon
set(DARKSUSY_VERSION "5.1.2")     #update
set(FEYNHIGGS_MD5 "49f5ea1838cb233baffd85bbc1b0d87d")
set(HIGGSBOUNDS_MD5 "47b93330d4e0fddcc23b381548db355b")
set(SOFTSUSY_MD5 "5efdbf71e43cfdb9c3ef619fe5abff8a")
set(SUPERISO_RELIC_MD5 "97cf83c8bd90f2a9993c2209337f9e72")
set(MICROMEGAS_MD5 "322856e02ddf06c76077b65f3a64de5d")
set(DARKSUSY_MD5 "ee9d47aeb88980e2777a1c61b7d495d0")

ExternalProject_Add(
  SoftSusy
  URL "http://www.hepforge.org/archive/softsusy/softsusy-${SOFTSUSY_VERSION}.tar.gz"
  URL_HASH MD5=${SOFTSUSY_MD5}
  PREFIX "${CMAKE_BINARY_DIR}/work/softsusy"
  DOWNLOAD_DIR "${PROJECT_SOURCE_DIR}/ext/distfiles"
  DOWNLOAD_NO_PROGRESS 1
  SOURCE_DIR "${CMAKE_BINARY_DIR}/work/softsusy/extract"
  # patch not needed after 3.6.2
  # PATCH_COMMAND patch src/softpars.h ${PROJECT_SOURCE_DIR}/patches/softsusy/softpars_h.patch
  CONFIGURE_COMMAND ${CMAKE_BINARY_DIR}/work/softsusy/extract/configure
    CXX=${CMAKE_CXX_COMPILER} --enable-static --with-pic
    --prefix=${CMAKE_BINARY_DIR} --includedir=${PROJECT_SOURCE_DIR}/include
  BUILD_COMMAND make
)
add_library(soft STATIC IMPORTED)
set_target_properties(soft PROPERTIES
  IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/lib/libsoft.a"
#  INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/include"
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
  CONFIGURE_COMMAND ${CMAKE_BINARY_DIR}/work/feynhiggs/extract/configure
    --static --64 --enable-full-g-2 --disable-vt100 CC=${CMAKE_C_COMPILER}
    FC=${CMAKE_Fortran_COMPILER} CXX=${CMAKE_CXX_COMPILER}
    --prefix=${CMAKE_BINARY_DIR}/work/feynhiggs/installed
  BUILD_COMMAND make
  INSTALL_COMMAND make install
    COMMAND install -d ${PROJECT_SOURCE_DIR}/include/feynhiggs
    COMMAND install ${CMAKE_BINARY_DIR}/work/feynhiggs/installed/include/CFeynHiggs.h ${PROJECT_SOURCE_DIR}/include/feynhiggs
    COMMAND install ${CMAKE_BINARY_DIR}/work/feynhiggs/installed/include/CSLHA.h ${PROJECT_SOURCE_DIR}/include/feynhiggs
    COMMAND install ${CMAKE_BINARY_DIR}/work/feynhiggs/installed/include/FHCouplings.h ${PROJECT_SOURCE_DIR}/include/feynhiggs
    COMMAND install ${CMAKE_BINARY_DIR}/work/feynhiggs/installed/include/FHRecord.h ${PROJECT_SOURCE_DIR}/include/feynhiggs
    COMMAND install ${CMAKE_BINARY_DIR}/work/feynhiggs/installed/include/PDG.h ${PROJECT_SOURCE_DIR}/include/feynhiggs
    COMMAND install ${CMAKE_BINARY_DIR}/work/feynhiggs/installed/include/SLHA.h ${PROJECT_SOURCE_DIR}/include/feynhiggs
    COMMAND install ${CMAKE_BINARY_DIR}/work/feynhiggs/installed/include/SLHADefs.h ${PROJECT_SOURCE_DIR}/include/feynhiggs
    COMMAND install ${CMAKE_BINARY_DIR}/work/feynhiggs/installed/lib/libFH.a ${CMAKE_BINARY_DIR}/lib
)
add_library(FH STATIC IMPORTED)
set_target_properties(FH PROPERTIES
  IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/lib/libFH.a"
#  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_BINARY_DIR}/include"
  IMPORTED_LINK_INTERFACE_LANGUAGES "C;CXX;Fortran"
)

ExternalProject_Add(
  HiggsBounds
  URL "http://www.hepforge.org/archive/higgsbounds/HiggsBounds-${HIGGSBOUNDS_VERSION}.tar.gz"
  URL_HASH MD5=${HIGGSBOUNDS_MD5}
  PREFIX "${CMAKE_CURRENT_BINARY_DIR}/work/higgsbounds"
  DOWNLOAD_DIR "${PROJECT_SOURCE_DIR}/ext/distfiles"
  DOWNLOAD_NO_PROGRESS 1
  SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/work/higgsbounds/extract"
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/work/higgsbounds/extract"
  CONFIGURE_COMMAND ${CMAKE_CURRENT_BINARY_DIR}/work/higgsbounds/extract/configure
  BUILD_COMMAND make libHB
  INSTALL_COMMAND install libHB.a ${CMAKE_CURRENT_BINARY_DIR}/lib
)
add_library(HB STATIC IMPORTED)
set_target_properties(HB PROPERTIES
  IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/lib/libHB.a"
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
  PATCH_COMMAND patch src/include.h ${PROJECT_SOURCE_DIR}/patches/superiso/include_h.patch
  CONFIGURE_COMMAND ${CMAKE_BINARY_DIR}/ext/superiso_relic/source/configure
    --with-cc=${CMAKE_C_COMPILER} --with-fc=${CMAKE_Fortran_COMPILER}
  BUILD_COMMAND make static
    COMMAND make libisospin.a
  INSTALL_COMMAND install ${CMAKE_CURRENT_BINARY_DIR}/work/superiso_relic/extract/src/libisospin.a
      ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
    COMMAND install -d ${PROJECT_SOURCE_DIR}/include/superiso_relic
    COMMAND install ${CMAKE_CURRENT_BINARY_DIR}/work/superiso_relic/extract/src/include.h
      ${PROJECT_SOURCE_DIR}/include/superiso_relic
    COMMAND install ${CMAKE_CURRENT_BINARY_DIR}/work/superiso_relic/extract/src/omega.h
      ${PROJECT_SOURCE_DIR}/include/superiso_relic
)
add_library(isospin STATIC IMPORTED)
set_target_properties(isospin PROPERTIES
  IMPORTED_LOCATION "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libisospin.a"
#  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_BINARY_DIR}/include"
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
  PREFIX "${CMAKE_BINARY_DIR}/work/darksusy"
  DOWNLOAD_DIR "${PROJECT_SOURCE_DIR}/ext/distfiles"
  DOWNLOAD_NO_PROGRESS 1
  SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/work/darksusy/extract"
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/work/darksusy/extract"
  PATCH_COMMAND patch src/ini/dsinit.f ${CMAKE_CURRENT_BINARY_DIR}/patches/darksusy/dsinit.patch
  CONFIGURE_COMMAND ${CMAKE_CURRENT_BINARY_DIR}/work/darksusy/extract/configure
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
add_dependencies(susykit SoftSusy SuperIso_Relic DarkSUSY HiggsBounds FeynHiggs)
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
add_dependencies(softest constrain SoftSusy FeynHiggs)
target_compile_features(softest PRIVATE cxx_range_for)
target_link_libraries(softest constrain soft)

add_executable(qpoint ${SOURCES_QPOINT})
target_compile_features(qpoint PRIVATE cxx_range_for)
target_link_libraries(qpoint constrain susykit)

