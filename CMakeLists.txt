cmake_minimum_required(VERSION 2.8)

project(myProject)

enable_testing()

add_subdirectory(test)
add_subdirectory(libmatrix)
set(Main_SRCS
    main.cpp
)

add_executable(myApp
    ${Main_SRCS}
)

# Specify here the libraries this program depends on
target_link_libraries(myApp
  libmatrix
)

install(TARGETS myApp DESTINATION bin)
