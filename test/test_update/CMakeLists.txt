project(test_update)

file(GLOB TEST_LIST ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_executable(${PROJECT_NAME} ${TEST_LIST} ${SRC}/client.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE
    d3d11
    dwmapi
    gdi32
    d3dcompiler
    ws2_32
    winmm
    pthread
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${SRC}
)

add_test(
    NAME test_update_test
    COMMAND ${PROJECT_NAME}
)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})