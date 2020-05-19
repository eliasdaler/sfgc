project(sfgc_minimal_example
  LANGUAGES CXX
)

# CMake 3.11 and later prefer to choose GLVND, but we choose legacy OpenGL just because it's safer
# (unless the OpenGL_GL_PREFERENCE was explicitly set)
# See CMP0072 for more details (cmake --help-policy CMP0072)
if ((NOT ${CMAKE_VERSION} VERSION_LESS 3.11) AND (NOT OpenGL_GL_PREFERENCE))
    set(OpenGL_GL_PREFERENCE "LEGACY")
endif()

find_package(OpenGL REQUIRED)

add_executable(sfgc_minimal_example
  main.cpp
)

target_link_libraries(sfgc_minimal_example
  PRIVATE
    sfgc::sfgc
    sfml-graphics
    ${OPENGL_LIBRARIES}
)

target_link_libraries(sfgc_minimal_example PRIVATE sfgc::sfgc)
