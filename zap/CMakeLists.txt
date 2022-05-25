cmake_minimum_required(VERSION 3.10)

project(zap)

# pkg-config
find_package(PkgConfig)

# SDL2
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image)
PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS})

# readline
PKG_SEARCH_MODULE(READLINE REQUIRED readline)
include_directories(${READLINE_INCLUDE_DIRS})

# dependencies
include_directories(${PROJECT_SOURCE_DIR}/dep)

# directories
add_subdirectory(src/opengl)
add_subdirectory(src/core)
add_subdirectory(src/entity)
add_subdirectory(src/scenery)
add_subdirectory(src/timer)

# main executable
add_executable(${PROJECT_NAME} main.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_include_directories(${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}/src
    PRIVATE ${PROJECT_BINARY_DIR}/src
)
target_link_directories(${PROJECT_NAME}
    PRIVATE ${SDL2_LIBRARY_DIRS}
    PRIVATE ${SDL2IMAGE_LIBRARY_DIRS}
    PRIVATE ${SDL2TTF_LIBRARY_DIRS}
    PRIVATE ${READLINE_LIBRARY_DIRS}
)
target_link_libraries(${PROJECT_NAME}
    PRIVATE core
    PRIVATE entity
    PRIVATE opengl
    PRIVATE scenery
    PRIVATE timer
    PRIVATE ${SDL2_LIBRARIES}
    PRIVATE ${SDL2IMAGE_LIBRARIES}
    PRIVATE ${SDL2TTF_LIBRARIES}
    PRIVATE ${READLINE_LIBRARIES}
)