#CMakeLists projet
project(projet)
cmake_minimum_required(VERSION 3.8.2)

set(RESOURCES_DIRECTORY "${PROJECT_SOURCE_DIR}/resources/")
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(INC_DIR "${PROJECT_SOURCE_DIR}/include/")

#Configure config.cpp.in
configure_file(
	${PROJECT_SOURCE_DIR}/src/config.cpp.in
  ${CMAKE_CURRENT_BINARY_DIR}/src/config.cpp @ONLY
)

# configure_file(
#     ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/config.hpp
#     ${CMAKE_CURRENT_BINARY_DIR}/include/${PROJECT_NAME}/config.hpp
#     COPYONLY
# )

include_directories(${CMAKE_CURRENT_BINARY_DIR}/include BEFORE)

set(SRC
	${CMAKE_CURRENT_BINARY_DIR}/src/config.cpp
	${SRC_DIR}/main.cpp
	${SRC_DIR}/window.cpp
	${SRC_DIR}/camera.cpp
	${SRC_DIR}/shader.cpp
	${SRC_DIR}/mesh.cpp
	${SRC_DIR}/app_ui.cpp
)

set(RESOURCE_FILES
#	${PROJECT_SOURCE_DIR}/resources/shaders/lighting.frag
#	${PROJECT_SOURCE_DIR}/resources/shaders/lighting.vert
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/resources" PREFIX "Resources Files" FILES ${RESOURCE_FILES})

add_executable(${PROJECT_NAME} ${SRC} ${HEADER} ${RESOURCE_FILES})
include_directories(${INC_DIR} AFTER)

#Copy resources
add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
)

install(
	TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
	DIRECTORY ${PROJECT_SOURCE_DIR}/resources
	DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME}
)

# Linking
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARY} stbimage glad glfw glm imgui glengine)
