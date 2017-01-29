find_package(OpenGL REQUIRED)

add_executable(w3dview main.cpp arcball.hpp viewer.cpp 
viewer.hpp flextGL.c flextGL.h model.hpp model.cpp
shader.cpp shader.hpp targa.hpp texture.cpp texture.hpp)

set_property(TARGET w3dview PROPERTY CXX_STANDARD 14)

target_link_libraries(w3dview libw3d glfw ${OPENGL_gl_LIBRARY})
target_include_directories(w3dview PRIVATE glfw ${OPENGL_INCLUDE_DIR} ${LIBW3D_ROOT}/deps/gli
                        ${LIBW3D_ROOT}/deps/cxxopts/include ${LIBW3D_ROOT}/deps/glm)