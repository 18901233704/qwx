project(kwx)
set(PROJECT_VERSION "1.0.0")

cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

# ECM setup
include(FeatureSummary)
find_package(ECM 5.24.0  NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://projects.kde.org/projects/kdesupport/extra-cmake-modules")
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)
set(CMAKE_MODULE_PATH "${ECM_MODULE_PATH}" "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

set(KF5_DEP_VERSION "5.24.0")
set(REQUIRED_QT_VERSION "5.7.0")

# Definitions
add_definitions(-fpermissive)

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Handle build type
if(CMAKE_BUILD_TYPE MATCHES [Dd]ebug)
    message(STATUS "Debug build")
    add_definitions(-DQWX_DEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
else()
    message(STATUS "Release build")
    add_definitions(-DNDEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
endif()

find_package(Qt5 ${REQUIRED_QT_VERSION} REQUIRED COMPONENTS Core Widgets Qml Quick Network Xml QuickWidgets DBus)
find_package(KF5 ${KF5_DEP_VERSION} REQUIRED COMPONENTS I18n DBusAddons XmlGui Declarative)
if(CMAKE_BUILD_TYPE MATCHES [Dd]ebug)
    find_package(Qt5Test ${REQUIRED_QT_VERSION} REQUIRED)
endif()

find_package(KF5Prison "1.2.2" EXACT QUIET CONFIG)
set_package_properties(KF5Prison PROPERTIES DESCRIPTION "The KF5 Prison libraries"
                       URL "http://projects.kde.org/prison"
                       TYPE REQUIRED
                       PURPOSE "prison is a barcode api currently offering a nice Qt api to produce QRCode barcodes and DataMatrix barcodes, and can easily be made support more."
                      )
if(KF5Prison_FOUND)
    add_definitions("-DKF5PRISON_FOUND")
endif()

find_package(OpenMP)
set_package_properties(OpenMP PROPERTIES DESCRIPTION "The OpenMP libraries"
                       URL "http://www.openmp.org"
                       TYPE OPTIONAL
                       PURPOSE "Only test for GCC 6.x"
                      )
if(OPENMP_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(OPENMP_LIBRARIES "-lgomp")
    add_definitions("-DOPENMP_FOUND")
endif()

# Add subdirectories
add_subdirectory(po)
add_subdirectory(src)
if(CMAKE_BUILD_TYPE MATCHES [Dd]ebug)
    add_subdirectory(tests)
endif()
