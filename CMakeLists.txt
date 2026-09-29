cmake_minimum_required(VERSION 3.20)

#################################    COLORS    ################################

string(ASCII 27 ESC)
set(COLOR_RESET     "${ESC}[m")
set(COLOR_RED       "${ESC}[31m")
set(COLOR_GREEN     "${ESC}[32m")
set(COLOR_YELLOW    "${ESC}[33m")
set(COLOR_BLUE      "${ESC}[34m")
set(COLOR_MAGENTA   "${ESC}[35m")
set(COLOR_CYAN      "${ESC}[36m")
set(COLOR_WHITE     "${Esc}[37m")

#################################    MACROS    ################################

macro(add_inc_recursive)
  foreach (path ${ARGV})
    unset(found_inc)
    file(GLOB_RECURSE found_inc ${path}/*.h)
    list(APPEND PROJECT_INCLUDES ${found_inc})
  endforeach ()
endmacro()

macro(add_src_recursive)
  foreach (path ${ARGV})
    unset(found_src)
    file(GLOB_RECURSE found_src ${path}/*.c)
    list(APPEND PROJECT_SOURCES ${found_src})
  endforeach ()
endmacro()

################################    PROJECT    ################################
project(rayc C)

set(PROJECT_DIR "${CMAKE_CURRENT_LIST_DIR}")
set(CMAKE_C_STANDARD 23)

include_directories(${PROJECT_DIR}/include)

add_inc_recursive(${PROJECT_DIR}/include)
add_src_recursive(${PROJECT_DIR}/src)

add_executable(rayc ${PROJECT_INCLUDES} ${PROJECT_SOURCES})

################################     SDL2     ################################

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_TTF REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)
