cmake_minimum_required(VERSION 3.10)
project(3DOpenGL)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_C_FLAGS_RELEASE "-O3")

set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(LOCAL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src/include")
set(LIB_DIR "${CMAKE_SOURCE_DIR}/lib")
file(GLOB IMGUI_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/include/imgui/*.cpp"
)
set(SOURCE_FILES
	${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/vendor/glad.c
	${CMAKE_SOURCE_DIR}/src/include/shaderprogram.cpp
	${CMAKE_SOURCE_DIR}/src/include/Renderer.cpp
	${CMAKE_SOURCE_DIR}/src/include/Texture.cpp
	${CMAKE_SOURCE_DIR}/src/include/Mesh.cpp
	${CMAKE_SOURCE_DIR}/src/include/objs/VertexArray.cpp
    ${CMAKE_SOURCE_DIR}/src/include/objs/VertexBuffer.cpp
	${CMAKE_SOURCE_DIR}/src/include/objs/IndexBuffer.cpp
	${CMAKE_SOURCE_DIR}/src/include/model.cpp
	${IMGUI_SOURCES}
)
find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} 
	PRIVATE
		${INCLUDE_DIR}
		${LOCAL_INCLUDE_DIR}
)

target_link_directories(${PROJECT_NAME} PRIVATE ${LIB_DIR}/GLFW ${LIB_DIR}/assimp)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        OpenGL::GL
        libglfw3.a
		assimp.dylib
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreFoundation"
)

