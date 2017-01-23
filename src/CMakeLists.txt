set(HEADER_DIR ${LIBW3D_ROOT}/include/libw3d)
set(PUBLIC_HEADERS  ${HEADER_DIR}/w3d.hpp)

file(GLOB_RECURSE SRC_FILES
    "*.hpp"
    "*.cpp"
)

add_library(libw3d STATIC   ${SRC_FILES}
							${PUBLIC_HEADERS})

set_property(TARGET libw3d PROPERTY CXX_STANDARD 14)
set_property(TARGET libw3d PROPERTY PREFIX "")
target_include_directories(libw3d PUBLIC ${LIBW3D_ROOT}/include)