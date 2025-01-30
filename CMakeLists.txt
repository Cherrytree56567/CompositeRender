cmake_minimum_required(VERSION 3.10)
project(CompositeRender)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Check for Emscripten
if(${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten" OR EMSCRIPTEN_BUILD)
    message(STATUS "Configuring for Emscripten")
    set(EMSCRIPTEN ON)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")  # Generate an HTML file
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_GLFW=3 -s USE_WEBGL2=1 -s ALLOW_MEMORY_GROWTH=1")
else()
    message(STATUS "Configuring for Native Build")
    set(EMSCRIPTEN OFF)
endif()

# Vendor libraries directory
set(VENDOR_DIR "${CMAKE_SOURCE_DIR}/vendor")

# ----------------------
# TinyXML2
# ----------------------
add_library(tinyxml2 STATIC
    ${VENDOR_DIR}/tinyxml2/tinyxml2.cpp
)
target_include_directories(tinyxml2 PUBLIC
    ${VENDOR_DIR}/tinyxml2/
)
target_link_libraries(tinyxml2 PRIVATE)

# ----------------------
# Python
# ----------------------
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux" OR EMSCRIPTEN_BUILD)
    add_library(python STATIC
        ${VENDOR_DIR}/penguinv/src/image_function_helper.cpp
    )
    find_package(Python3 3.12 REQUIRED COMPONENTS Development)
    target_link_libraries(python PRIVATE tinyxml2 Python3::Python)
else()
    add_library(python STATIC
        ${VENDOR_DIR}/penguinv/src/image_function_helper.cpp
    )
    target_include_directories(python PUBLIC
        ${VENDOR_DIR}/python/include/
    )
    link_directories(${VENDOR_DIR}/python/libs)
    target_link_libraries(python PRIVATE tinyxml2 python312)
endif()

# ----------------------
# Main Application
# ----------------------
file(GLOB_RECURSE SRC "src/*.cpp" "src/*/*.cpp")

add_executable(CompositeRenderer ${SRC})

# ----------------------
# STB IMAGE
# ----------------------
target_include_directories(CompositeRenderer PUBLIC
    ${VENDOR_DIR}/stb
)

# Link libraries to the application (do it once for both Emscripten and native builds)
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux" OR EMSCRIPTEN_BUILD)
    target_link_libraries(CompositeRenderer PRIVATE tinyxml2 Python3::Python)
else()
    target_link_libraries(CompositeRenderer PRIVATE tinyxml2 python)
endif()
