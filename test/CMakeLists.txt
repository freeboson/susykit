set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test/bin)

set(CATCH_HPP_URL "https://raw.githubusercontent.com/philsquared/Catch/master/single_include/catch.hpp")

if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/ext/include/catch/catch.hpp)
    file(DOWNLOAD ${CATCH_HPP_URL} ${CMAKE_CURRENT_SOURCE_DIR}/ext/include/catch/catch.hpp TLS_VERIFY ON)
endif()

add_library(catch_main OBJECT
    "src/catch_main.cpp"
)
target_include_directories(catch_main PRIVATE "ext/include")

add_executable(constrain_unit
    $<TARGET_OBJECTS:catch_main>
    "src/unit-basic.cpp"
)
target_include_directories(constrain_unit PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/include
)
target_link_libraries(constrain_unit constrain)
