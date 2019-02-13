cmake_minimum_required(VERSION 3.13)
enable_testing()
configure_file(CMakeGoogleTest.txt googletest-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download" )
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download" )

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Add googletest directly to our build. This adds
# the following targets: gtest, gtest_main, gmock
# and gmock_main
add_subdirectory("${CMAKE_BINARY_DIR}/googletest-src"
                 "${CMAKE_BINARY_DIR}/googletest-build")
add_subdirectory(test)
add_subdirectory(libmatrix)


#Main here
add_executable(myApp
    main.cpp
)

# Specify here the libraries this program depends on
target_link_libraries(myApp
  libmatrix
)

# install(TARGETS myApp DESTINATION bin)
