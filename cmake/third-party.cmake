add_library(imgui
        #ImGui Core
        ${CMAKE_SOURCE_DIR}/src/third-party/imgui/imgui.cpp
        ${CMAKE_SOURCE_DIR}/src/third-party/imgui/imgui_demo.cpp
        ${CMAKE_SOURCE_DIR}/src/third-party/imgui/imgui_draw.cpp
        ${CMAKE_SOURCE_DIR}/src/third-party/imgui/imgui_tables.cpp
        ${CMAKE_SOURCE_DIR}/src/third-party/imgui/imgui_widgets.cpp

        #ImGui Backends
        ${CMAKE_SOURCE_DIR}/src/third-party/imgui/backends/imgui_impl_glfw.cpp
        ${CMAKE_SOURCE_DIR}/src/third-party/imgui/backends/imgui_impl_opengl3.cpp

        #ImGui Extras
        ${CMAKE_SOURCE_DIR}/src/third-party/imgui/misc/cpp/imgui_stdlib.cpp
        )

add_dependencies(imgui glfw)

list(APPEND IMGUI_INCLUDE_DIRS
        ${CMAKE_SOURCE_DIR}/src/third-party/imgui
        ${CMAKE_SOURCE_DIR}/src/third-party/imgui/backends)
list(APPEND IMGUI_LIBRARIES imgui)

target_include_directories(imgui PRIVATE SYSTEM ${IMGUI_INCLUDE_DIRS})
target_include_directories(imgui PRIVATE SYSTEM ${GLFW_INCLUDE_DIR})
target_link_libraries(imgui PRIVATE ${GLFW_LIBRARIES})
target_compile_definitions(imgui PUBLIC -DIMGUI_IMPL_OPENGL_LOADER_GLAD)

add_library(fmt
        ${CMAKE_SOURCE_DIR}/src/third-party/fmt/src/format.cc
        )
list(APPEND FMT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/src/third-party/fmt/include)
list(APPEND FMT_LIBRARIES fmt)

target_include_directories(fmt PRIVATE SYSTEM ${FMT_INCLUDE_DIRS})
