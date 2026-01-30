# 尝试使用系统已安装的 GLFW 库
find_package(GLFW 3.3.5 QUIET)

if(GLFW_FOUND)
    message(STATUS "Found GLFW")
else()
    message(STATUS "GLFW not found - will use local source")
    
    # 设置本地 GLFW 源码目录
    set(GLFW_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src/third-party/glfw")
    
    # 检查 GLFW 源码是否存在
    if (EXISTS "${GLFW_SOURCE_DIR}")
        # 使用本地 GLFW 源码
        add_subdirectory(${GLFW_SOURCE_DIR})
        set(GLFW_INCLUDE_DIR ${GLFW_SOURCE_DIR}/include)
        set(GLFW_LIBRARIES glfw)
        message(STATUS "Using local GLFW source")
    else()
        # 如果本地没有 GLFW 源码，则尝试使用 VCPKG 安装的 GLFW
        if (DEFINED ENV{VCPKG_ROOT})
            set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
            find_package(glfw3 REQUIRED)
            set(GLFW_LIBRARIES glfw)
            message(STATUS "Using VCPKG GLFW library")
        else()
            # 如果本地没有 GLFW 源码，也没有 VCPKG，则显示错误消息
            message(FATAL_ERROR "GLFW not found. Please install GLFW or download GLFW source to ${GLFW_SOURCE_DIR}")
        endif()
    endif()
endif()

set(GLFW_INCLUDE_DIR ${GLFW_INCLUDE_DIR} CACHE STRING "")
set(GLFW_LIBRARIES ${GLFW_LIBRARIES} CACHE STRING "")
