cmake_minimum_required(VERSION 3.16.3)


project(miniRT)

set (SRCS_MAIN
        src/main.c
)

file (GLOB_RECURSE SRCS "src/*.c")

set (EXEC_NAME miniRT)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Iinclude -ldl -lglfw -pthread -lm -g -O0")

add_executable(${EXEC_NAME} ${SRCS_MAIN} ${SRCS})
target_include_directories(${EXEC_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${EXEC_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/MLX42/include/MLX42/)

add_subdirectory(lib/Libft)
add_subdirectory(lib/MLX42)
add_subdirectory(lib/get_next_line)

find_package(Threads REQUIRED)
target_link_libraries(${EXEC_NAME} PRIVATE Threads::Threads)
target_link_libraries(${EXEC_NAME} PRIVATE ft)
target_link_libraries(${EXEC_NAME} PRIVATE mlx42)
target_link_libraries(${EXEC_NAME} PRIVATE get_next_line)
target_link_libraries(${EXEC_NAME} PRIVATE m)
target_link_libraries(${EXEC_NAME} PRIVATE dl)
