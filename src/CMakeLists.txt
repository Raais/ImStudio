set(TARGET ImStudio)

include(third-party)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/sources/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/utils/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/sources/*.h ${CMAKE_CURRENT_SOURCE_DIR}/utils/*.h)

add_executable(${TARGET} WIN32 MACOSX_BUNDLE
    ${SOURCES}
    ${HEADERS}
    backend_glfw_opengl3.cpp
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


if (APPLE)
    # TODO:
    #set_target_properties(${TARGET} PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "ImStudio")
    #set_source_files_properties(icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

    #set_target_properties(${GUI_ONLY_BINARIES} PROPERTIES
    #        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}
    #        MACOSX_BUNDLE_LONG_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}
    #        MACOSX_BUNDLE_ICON_FILE "icns"
    #        MACOSX_FRAMEWORK_IDENTIFIER "org.ImStudio"
    #        MACOSX_BUNDLE_INFO_PLIST "${GLFW_SOURCE_DIR}/cmake/Info.plist.in")
endif()

install(TARGETS ${TARGET} DESTINATION bin)
install(FILES ${CMAKE_SOURCE_DIR}/LICENSE DESTINATION share/${TARGET})
install(FILES ${CMAKE_SOURCE_DIR}/README.md DESTINATION share/${TARGET})
