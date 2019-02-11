file(GLOB SRCS *.cpp)

ADD_EXECUTABLE(testmatrix ${SRCS})

TARGET_LINK_LIBRARIES(testmatrix
    libmatrix
    libgtest
    libgmock)

add_test(NAME testmatrix
COMMAND testmatrix)
