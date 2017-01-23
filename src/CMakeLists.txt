set(HEADER_DIR ${LIBW3D_ROOT}/include/libw3d)

file(GLOB_RECURSE SRC_FILES
    "*.hpp"
    "*.cpp"
)

file(GLOB_RECURSE PUBLIC_HEADERS "../include/libw3d/*.hpp")

add_library(libw3d STATIC   ${SRC_FILES}
							${PUBLIC_HEADERS})

set_property(TARGET libw3d PROPERTY CXX_STANDARD 14)
set_property(TARGET libw3d PROPERTY PREFIX "")
target_include_directories(libw3d PUBLIC ${LIBW3D_ROOT}/include)