cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(DEFAULT_PROJECT_VERSION "0.0.${PATCH_VERSION}")
set(PROJECT_VERSION_OVERRIDE "${DEFAULT_PROJECT_VERSION}" CACHE STRING "Override project version")

project(ip_filter VERSION ${PROJECT_VERSION_OVERRIDE})

# Find Boost for tests (optional)
find_package(Boost COMPONENTS unit_test_framework QUIET)

# Library
add_library(ip_filter_lib src/ip_filter_lib.cpp)
target_include_directories(ip_filter_lib
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

set_target_properties(ip_filter_lib PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Executable
add_executable(ip_filter src/ip_filter.cpp)
target_link_libraries(ip_filter PRIVATE ip_filter_lib)

set_target_properties(ip_filter PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Tests (only if Boost is found)
if(Boost_FOUND)
    add_executable(ip_filter_tests tests/ip_filter_tests.cpp)
    target_link_libraries(ip_filter_tests
        ip_filter_lib
        ${Boost_LIBRARIES}
    )
    target_include_directories(ip_filter_tests
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src"
        ${Boost_INCLUDE_DIRS}
    )
    
    set_target_properties(ip_filter_tests PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    )
    
    enable_testing()
    add_test(NAME ip_filter_tests COMMAND ip_filter_tests)
endif()

# Compiler options
if (MSVC)
    target_compile_options(ip_filter_lib PRIVATE /W4)
    target_compile_options(ip_filter PRIVATE /W4)
    if(Boost_FOUND)
        target_compile_options(ip_filter_tests PRIVATE /W4)
    endif()
else ()
    target_compile_options(ip_filter_lib PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(ip_filter PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    if(Boost_FOUND)
        target_compile_options(ip_filter_tests PRIVATE
            -Wall -Wextra -pedantic -Werror
        )
    endif()
endif()

install(TARGETS ip_filter RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT pdiv@tpu.ru)

# Устанавливаем генератор CPack в зависимости от ОС
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(CPACK_GENERATOR "DEB")
elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set(CPACK_GENERATOR "TGZ")
endif()

include(CPack)
