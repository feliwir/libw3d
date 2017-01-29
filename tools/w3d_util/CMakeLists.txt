add_executable(w3dutil main.cpp)
target_link_libraries(w3dutil libw3d)
set_property(TARGET w3dutil PROPERTY CXX_STANDARD 14)
