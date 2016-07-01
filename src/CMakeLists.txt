qt5_add_resources(qwx_RC_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/../resources.qrc)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(qwx 
    main.cpp
	qtsingleapplication/qtsingleapplication.cpp
    qtsingleapplication/qtlocalpeer.cpp
    httpget.cpp
    qwxuuid.cpp
    scan.cpp
    cookie.cpp
    httppost.cpp
    statreport.cpp
    init.cpp
    userobject.h
    sync.cpp
    contact.cpp
    headimg.cpp
    download.cpp
    statusnotify.cpp
    sendmsg.cpp
    monitor.cpp
    getmsg.cpp
    circleimage.cpp
    process.cpp
    xiaodoubi.cpp
    chatlog.cpp
    emotion.cpp
    logout.cpp
    ipcity.cpp
    weather.cpp
    i18n.cpp
    clipboard.cpp
    ${qwx_RC_SRCS}
)

target_link_libraries(qwx
    Qt5::Widgets
    Qt5::Qml
    Qt5::Quick
    Qt5::Network
    Qt5::Xml
)

add_dependencies(qwx translation)

install(TARGETS qwx RUNTIME DESTINATION bin)
