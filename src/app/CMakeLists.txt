add_executable(
    tiles Application.cpp

    # CORE
    Core/Window.hpp
    Core/Window.cpp
    Core/Event.hpp
    Core/Event.cpp

    # MATH
    Math/Rectangle.hpp
    Math/Rectangle.cpp

    # WORLD
    World/Cell.hpp
    World/Cell.cpp
    World/Chunk.hpp
    World/Chunk.cpp
    World/Landscape.hpp
    World/Landscape.cpp
)

target_include_directories(tiles PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

# Use precompiled headers.
target_precompile_headers(tiles PRIVATE pch.hpp pch.cpp)

# Link in glad and glfw libraries
target_link_libraries(tiles PRIVATE OpenGL32)
target_link_libraries(tiles PRIVATE glm)
target_link_libraries(tiles PRIVATE glad)
target_link_libraries(tiles PRIVATE glfw)

# Link in LWVL
target_link_libraries(tiles PRIVATE lwvl)

# Mark the project as C++ 17 and set the output directory.
set_target_properties(
    tiles PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO

    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
)

# Add shaders here
add_custom_target(
    shaders ALL
    DEPENDS

    ${CMAKE_BINARY_DIR}/bin/Data/Shaders/chunk.vert
    ${CMAKE_BINARY_DIR}/bin/Data/Shaders/chunk.frag
)
add_dependencies(tiles shaders)

add_custom_command(
    DEPENDS
    ${CMAKE_SOURCE_DIR}/res/Shaders/chunk.vert
    ${CMAKE_SOURCE_DIR}/res/Shaders/chunk.frag

    OUTPUT
    ${CMAKE_BINARY_DIR}/bin/Data/Shaders/chunk.vert
    ${CMAKE_BINARY_DIR}/bin/Data/Shaders/chunk.frag

    COMMENT "Copied shader folder to output"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/res/Shaders/
    ${CMAKE_BINARY_DIR}/bin/Data/Shaders/
)
