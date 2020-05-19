cmake_minimum_required(VERSION 3.1)

project(sfgc
  LANGUAGES CXX
  VERSION 0.1
)

option(SFGC_BUILD_EXAMPLES "Build SFGC examples" OFF)
option(SFGC_FIND_SFML "Use find_package to find SFML" ON)

if (SFGC_FIND_SFML)
	if (NOT BUILD_SHARED_LIBS)
		set(SFML_STATIC_LIBRARIES ON)
	endif()
	find_package(SFML 2.5 COMPONENTS system window)

	if(NOT SFML_FOUND)
    message(FATAL_ERROR "SFML 2 directory not found. Set SFML_DIR to directory where SFML was built (or one which contains SFMLConfig.cmake)")
	endif()
endif()


add_library(sfgc
  "${CMAKE_CURRENT_LIST_DIR}/src/sfgc.cpp"
)

# Add pretty alias
add_library(sfgc::sfgc ALIAS sfgc)
target_link_libraries(sfgc PUBLIC sfml-window sfml-system)

target_include_directories(sfgc
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if(BUILD_SHARED_LIBS)
  target_compile_definitions(sfgc PRIVATE SFGC_SHARED_LIB)
  set_target_properties(sfgc PROPERTIES
    DEFINE_SYMBOL "SFGC_EXPORTS"
  )
endif()

set(SFGC_PUBLIC_HEADERS
  "${CMAKE_CURRENT_LIST_DIR}/include/sfgc.h"
)

if(SFGC_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

# installation rules
install(TARGETS sfgc
  EXPORT sfgc
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT sfgc
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/sfgc
  NAMESPACE sfgc::
  FILE sfgcConfig.cmake
)
