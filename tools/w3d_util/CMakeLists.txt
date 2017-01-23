add_executable(w3dutil main.cpp)
target_link_libraries(w3dutil libw3d)
set_property(TARGET w3dutil PROPERTY CXX_STANDARD 14)
add_custom_command(TARGET w3dutil POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_CURRENT_SOURCE_DIR}/files $<TARGET_FILE_DIR:w3dutil>)