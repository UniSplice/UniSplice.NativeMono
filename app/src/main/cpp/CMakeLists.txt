cmake_minimum_required(VERSION 3.22.1)

project("unisplicemono" VERSION 1.1.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(unisplicemono SHARED main.cpp unisplice.h
    unisplice/Main.cpp unisplice/Main.h
    unisplice/hook/DlOpen.cpp unisplice/hook/DlOpen.h
    unisplice/hook/Mono.cpp unisplice/hook/Mono.h)

target_include_directories(unisplicemono PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(shadowhook REQUIRED CONFIG)

target_link_libraries(unisplicemono
    shadowhook::shadowhook
    android
    log)
