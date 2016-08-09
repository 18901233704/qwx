add_definitions(-DTRANSLATION_DOMAIN=\"kwx\")

qt5_add_resources(kwx_RC_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/../resources.qrc)
qt5_add_dbus_adaptor(kwx_DBUS_SRCS org.isoftlinux.kwx.xml ${CMAKE_CURRENT_SOURCE_DIR}/kwxapp.h KwxApp)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(kwx_SRCS 
    main.cpp
    kwxapp.cpp
    kwxwin.cpp
    httpget.cpp
    kwxuuid.cpp
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
    xiaodoubi.cpp
    chatlog.cpp
    emotion.cpp
    logout.cpp
    ipcity.cpp
    weather.cpp
    i18n.cpp
    clipboard.cpp
    uploadmedia.cpp
)
if(KF5Prison_FOUND)
    set(kwx_SRCS ${kwx_SRCS} qrcodequick.cpp)
endif()

add_executable(kwx 
    ${kwx_SRCS}
    ${kwx_RC_SRCS}
    ${kwx_DBUS_SRCS}
)
target_compile_definitions(kwx PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")

target_link_libraries(kwx
    Qt5::Widgets
    Qt5::Qml
    Qt5::Quick
    Qt5::Network
    Qt5::Xml
    Qt5::QuickWidgets
    KF5::I18n
    KF5::DBusAddons
    KF5::XmlGui
    KF5::Declarative
)
if(KF5Prison_FOUND)
    target_link_libraries(kwx KF5::Prison)
endif()

install(TARGETS kwx RUNTIME DESTINATION bin)
