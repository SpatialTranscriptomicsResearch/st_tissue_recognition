# Make it possible to #include <git_config.hpp> for out-of-source builds
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

CONFIGURE_FILE(git_config.hpp.in git_config.hpp)

ADD_LIBRARY(tissue-recognition-opencv SHARED
    tissue-recognition-opencv.cpp
    util.cpp)
TARGET_LINK_LIBRARIES(tissue-recognition-opencv
    ${OpenCV_LIBS})

ADD_LIBRARY(tissue-recognition SHARED
    tissue-recognition.cpp
    util.cpp)
TARGET_LINK_LIBRARIES(tissue-recognition
    tissue-recognition-opencv
    ${OpenCV_LIBS})

ADD_EXECUTABLE(recognize-tissue main.cpp)
TARGET_LINK_LIBRARIES(recognize-tissue tissue-recognition-opencv)

INSTALL(TARGETS tissue-recognition-opencv DESTINATION lib)
INSTALL(TARGETS tissue-recognition DESTINATION lib)
INSTALL(TARGETS recognize-tissue DESTINATION bin)
