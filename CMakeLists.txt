cmake_minimum_required(VERSION 3.21.0)

project(miniRT)

set (SRCS_MAIN
        src/main.c
)

set (SRCS
    src/render/color.c
    src/render/hit_record.c
    src/render/hittable.c
    src/render/sphere.c
    src/render/hittable_list.c
    src/double.c
    src/random.c )

set (EXEC_NAME miniRT)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Iinclude -ldl -lglfw -pthread -lm")

add_executable(${EXEC_NAME} ${SRCS_MAIN} ${SRCS})
target_include_directories(${EXEC_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(lib/Libft)
add_subdirectory(lib/MLX42)
add_subdirectory(lib/get_next_line)

target_link_libraries(${EXEC_NAME} PRIVATE ft)
target_link_libraries(${EXEC_NAME} PRIVATE mlx42)
target_link_libraries(${EXEC_NAME} PRIVATE get_next_line)
target_link_libraries(${EXEC_NAME} PRIVATE m)
