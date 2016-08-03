include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_BINARY_DIR}
)

if(POLICY CMP0028)
    cmake_policy(SET CMP0028 OLD)
endif()

macro(QWX_EXECUTABLE_TESTS)
    foreach(_testname ${ARGN})
        add_executable(${_testname} 
            ${_testname}.cpp 
            ${CMAKE_CURRENT_SOURCE_DIR}/../src/uploadmedia.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/../src/httpget.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/../src/cookie.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/../src/chatlog.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/../src/qwxuuid.cpp
        )
        target_link_libraries(${_testname}
            Qt5::Test
            Qt5::Network
            Qt5::Xml
        )
    endforeach()
endmacro(QWX_EXECUTABLE_TESTS)

QWX_EXECUTABLE_TESTS(
    uploadmediatest
    cookietest
    chatlogtest
    qwxuuidtest
)
