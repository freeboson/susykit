cmake_minimum_required(VERSION 2.8.9)
project(SusyKit)
set(PROJECT_BINARY_DIR ${PROJECT_SOURCE_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(include)

set(SOURCES_LCONS 
	src/parseutils.cpp src/parse.cpp src/model.cpp src/model_lookup.cpp
	src/special_lookups.cpp src/get_slha.cpp src/likeconfig.cpp
	src/likedatum.cpp src/loglike.cpp)

set(SOURCES_LSKIT 
	src/superiso_interface.cpp  src/micromegas_interface.cpp
	src/softsusy_interface.cpp src/feynhiggs_interface.cpp
	src/softsusy_opts.cpp)

set(SOURCES_CONSTRAIN 
	src/constraint.cpp src/constrain.cpp src/constrain_opts.cpp)

set(SOURCES_QPARSE src/qparse.cpp)

set(SOURCES_SDB2SLHA src/sdb2slha.cpp)

add_library(constrain STATIC ${SOURCES_LCONS})
target_compile_features(constrain PRIVATE cxx_range_for)

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

