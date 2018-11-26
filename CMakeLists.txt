cmake_minimum_required(VERSION 2.8.12)

project(ktexteditor_zealsearch)

option(USE_KDE4 "legacy build for KDE4" ON)

include(FeatureSummary)

if(USE_KDE4)

    message(STATUS "Building for KDE4")

    find_package(KDE4 REQUIRED)
    include(KDE4Defaults)

    add_subdirectory(kde4)

else()

    message(STATUS "Building for KF5")

    find_package(ECM 1.7.0 REQUIRED NO_MODULE)

    set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

    set(REQUIRED_QT_VERSION "5.2")
    find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Core Gui Widgets)

    include(KDEInstallDirs)
    include(KDECompilerSettings NO_POLICY_SCOPE)
    include(KDECMakeSettings)

    include(ECMAddTests)

    set(KF5_DEP_VERSION "5.29.0")

    find_package(KF5 ${KF5_DEP_VERSION} REQUIRED COMPONENTS CoreAddons I18n TextEditor XmlGui)

    add_subdirectory(kf5)

endif()

feature_summary(WHAT ALL   FATAL_ON_MISSING_REQUIRED_PACKAGES)
