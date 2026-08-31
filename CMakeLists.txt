cmake_minimum_required(VERSION 3.21)

project(LemmingsGameEngine VERSION 1.0.0 LANGUAGES CXX)

find_package(SFML 3 COMPONENTS Graphics Window Audio System CONFIG REQUIRED)

add_executable(lemmings
    Actor.cpp
    GameController.cpp
    GameWorld.cpp
    StudentWorld.cpp
    main.cpp
)

target_sources(lemmings PRIVATE
    Actor.h
    Coord.h
    GameConstants.h
    GameController.h
    GameWorld.h
    GraphObject.h
    Level.h
    StudentWorld.h
)

target_compile_features(lemmings PRIVATE cxx_std_17)

if(MSVC)
    target_compile_options(lemmings PRIVATE /W4 /permissive-)
else()
    target_compile_options(lemmings PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_link_libraries(lemmings PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::Audio
    SFML::System
)

add_custom_command(
    TARGET lemmings POST_BUILD
    COMMAND "${CMAKE_COMMAND}"
            "-DAPP_EXECUTABLE=$<TARGET_FILE:lemmings>"
            "-DDEPENDENCY_DIR=$<TARGET_FILE_DIR:SFML::Audio>"
            "-DDESTINATION=$<TARGET_FILE_DIR:lemmings>"
            -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CopyRuntimeDependencies.cmake"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/Assets"
            "$<TARGET_FILE_DIR:lemmings>/Assets"
    COMMENT "Copying runtime dependencies and assets"
)

set_property(
    TARGET lemmings
    PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:lemmings>"
)
