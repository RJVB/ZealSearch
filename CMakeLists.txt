cmake_minimum_required(VERSION 2.8.12)

project(ktexteditor_zealsearch)

option(USE_KDE4 "legacy build for KDE4" ON)

include(FeatureSummary)

if(USE_KDE4)

    find_package(KDE4 REQUIRED)
    include(KDE4Defaults)
    include_directories(${KDE4_INCLUDES} ${QT_INCLUDES})
    add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

    set(ktexteditor_zealsearch_SRCS zealsearch_config.cpp zealsearchplugin.cpp)

    kde4_add_plugin(ktexteditor_zealsearch ${ktexteditor_zealsearch_SRCS})

    target_link_libraries(ktexteditor_zealsearch ${KDE4_KDEUI_LIBS} ${KDE4_KTEXTEDITOR_LIBS})

    install(TARGETS ktexteditor_zealsearch DESTINATION ${PLUGIN_INSTALL_DIR})

    install(FILES zealsearchui.rc DESTINATION ${DATA_INSTALL_DIR}/ktexteditor_zealsearch)

    install(FILES ktexteditor_zealsearch.desktop ktexteditor_zealsearch_config.desktop DESTINATION ${SERVICES_INSTALL_DIR})

else()

    message(STATUS "Building for KF5")

    find_package(ECM 1.7.0 REQUIRED NO_MODULE)

    set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

    set(REQUIRED_QT_VERSION "5.2")
    find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Core Gui Widgets)
    include(KDEInstallDirs)
    include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
    include(KDECMakeSettings)

    include(ECMAddTests)

    set(KF5_DEP_VERSION "5.29.0")

    find_package(KF5 ${KF5_DEP_VERSION} REQUIRED COMPONENTS CoreAddons I18n TextEditor XmlGui)

    add_subdirectory(kf5)
endif()

feature_summary(WHAT ALL   FATAL_ON_MISSING_REQUIRED_PACKAGES)
