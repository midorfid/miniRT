cmake_minimum_required(VERSION 3.16.3)


project(miniRT)

set (SRCS_MAIN
        src/main.c
)

set (SRCS
    src/render/color.c
    src/render/hit_record.c
    src/render/hittable.c
    src/render/sphere.c
    src/render/moving_sphere.c
    src/render/hittable_list.c
    src/render/aabb.c
    src/render/bvh.c
    src/random.c
    src/render/material.c
    src/textures/texture.c
    src/textures/checker_pattern.c
    src/textures/solid_colour.c
    src/textures/perlin.c
    src/render/quad.c
    src/render/box.c
    src/render/matrix3.c
    src/render/instance.c
    src/render/constant_medium.c
    src/render/material_isotropic.c
    src/render_context.c
    src/scenes.c
    src/render/material_dielectric.c
    src/render/diffuse_light.c
    src/render/material_lambertian.c
    src/render/material_metal.c
    src/threads/thread_pool.c 
    src/image_save.c )

set (EXEC_NAME miniRT)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Iinclude -ldl -lglfw -pthread -lm")

add_executable(${EXEC_NAME} ${SRCS_MAIN} ${SRCS})
target_include_directories(${EXEC_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

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
