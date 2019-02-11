
set(libmatrix_SRCS matrix.hpp)

# Declare the library
add_library(libmatrix STATIC
    ${libmatrix_SRCS}
)
set_target_properties(libmatrix PROPERTIES LINKER_LANGUAGE CXX)
# Specify here the include directories exported
# by this library
target_include_directories(libmatrix PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR})
