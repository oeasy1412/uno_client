cmake_minimum_required(VERSION 3.15)

project(uno_client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/UNO_ImGui/*.cpp)
# include_directories(${PROJECT_SOURCE_DIR}/thirdparty)
add_executable(app ${SRC_LIST})

target_link_libraries(app PRIVATE
    d3d11
    dwmapi
    gdi32
    d3dcompiler
    ws2_32
    winmm
)
target_compile_options(app PRIVATE
    -static-libgcc
    -static-libstdc++
    # -O2
    # -DNDEBUG
)

set(OUTPUT ${CMAKE_SOURCE_DIR})
set(EXECUTABLE_OUTPUT_PATH ${OUTPUT})