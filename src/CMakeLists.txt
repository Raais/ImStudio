set(TARGET ImStudio)

include(third-party)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/sources/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/utils/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/headers/*.h ${CMAKE_CURRENT_SOURCE_DIR}/utils/*.h)

add_executable(${TARGET}
    ${SOURCES}
    ${HEADERS}
    main.cpp
)

message(STATUS "IMGUI_INCLUDE_DIRS: ${IMGUI_INCLUDE_DIRS}")
message(STATUS "FMT_INCLUDE_DIRS: ${FMT_INCLUDE_DIRS}")

target_include_directories(${TARGET} PRIVATE SYSTEM ${IMGUI_INCLUDE_DIRS})
target_include_directories(${TARGET} PRIVATE SYSTEM ${FMT_INCLUDE_DIRS})
target_include_directories(${TARGET} PRIVATE SYSTEM ${GLFW_INCLUDE_DIR})

target_link_libraries(${TARGET} PRIVATE ${IMGUI_LIBRARIES})
target_link_libraries(${TARGET} PRIVATE ${GLFW_LIBRARIES})
target_link_libraries(${TARGET} PRIVATE ${FMT_LIBRARIES})
target_link_libraries(${TARGET} PRIVATE ${OPENGL_LIBRARIES})
target_link_libraries(${TARGET} PRIVATE ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${TARGET} PRIVATE ${CMAKE_DL_LIBS})

if(UNIX AND NOT APPLE)
    install(TARGETS ${TARGET} DESTINATION bin)
    install(FILES ${CMAKE_SOURCE_DIR}/LICENSE DESTINATION share/${TARGET})
    install(FILES ${CMAKE_SOURCE_DIR}/README.md DESTINATION share/${TARGET})
endif()
