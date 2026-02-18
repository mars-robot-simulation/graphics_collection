cmake_minimum_required(VERSION 3.10)
project(vsg_lines)
set(PROJECT_VERSION 1.0)
set(PROJECT_DESCRIPTION "Implementation to create lines and pathes.")
include(FindPkgConfig)

find_package(vsg)

vsg_setup_dir_vars()
vsg_setup_build_vars()

include_directories(src)

set(SOURCES 
	src/LinesFactory.cpp
	src/LinesP.cpp
)

set(HEADERS
	src/LinesFactory.hpp
	src/LinesP.hpp
	src/Lines.hpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "")
set_target_properties(${PROJECT_NAME} PROPERTIES RELWITHDEBUG_POSTFIX "")

target_link_libraries(${PROJECT_NAME}
          vsg::vsg
)

if(WIN32)
  set(LIB_INSTALL_DIR bin) # .dll are in PATH, like executables
else(WIN32)
  set(LIB_INSTALL_DIR lib)
endif(WIN32)


set(_INSTALL_DESTINATIONS
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION ${LIB_INSTALL_DIR}
	ARCHIVE DESTINATION lib
)


# Install the library into the lib folder
install(TARGETS ${PROJECT_NAME} ${_INSTALL_DESTINATIONS})

# Install headers into include directory
install(FILES ${HEADERS} DESTINATION include/${PROJECT_NAME})

# Prepare and install necessary files to support finding of the library 
# using pkg-config
configure_file(${PROJECT_NAME}.pc.in ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc DESTINATION lib/pkgconfig)


