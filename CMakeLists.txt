cmake_minimum_required(VERSION 3.14)
project(ApartmentRecommender VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ApartmentRecommender
    src/main.cpp
    src/Apartment.cpp
    src/Apartment.h
)

target_include_directories(ApartmentRecommender PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(JSONCPP QUIET jsoncpp)
endif()

find_package(jsoncpp QUIET)

if(TARGET jsoncpp_lib)
    target_link_libraries(ApartmentRecommender PRIVATE jsoncpp_lib)
elseif(TARGET jsoncpp_static)
    target_link_libraries(ApartmentRecommender PRIVATE jsoncpp_static)
elseif(JSONCPP_FOUND)
    target_include_directories(ApartmentRecommender PRIVATE ${JSONCPP_INCLUDE_DIRS})
    target_link_libraries(ApartmentRecommender PRIVATE ${JSONCPP_LIBRARIES})
else()
    target_link_libraries(ApartmentRecommender PRIVATE jsoncpp)
endif()
