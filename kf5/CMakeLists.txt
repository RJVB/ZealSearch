add_definitions(-DTRANSLATION_DOMAIN=\"ktexteditor_zealsearch\")
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

set(ktexteditor_zealsearch_SRCS zealsearch_config.cpp zealsearchplugin.cpp zealsearchview.cpp)
# resource for ui file and stuff
qt5_add_resources(ktexteditor_zealsearch_SRCS plugin.qrc)

ecm_qt_declare_logging_category(ktexteditor_zealsearch_SRCS
    HEADER zealsearch_debug.h
    IDENTIFIER KTEZEAL
    CATEGORY_NAME "katezealsearchplugin"
)

add_library(ktexteditor_zealsearch MODULE ${ktexteditor_zealsearch_SRCS})

get_filename_component(json ktexteditor_zealsearch.json REALPATH)
set_property(TARGET ktexteditor_zealsearch APPEND PROPERTY AUTOGEN_TARGET_DEPENDS ${json})

target_link_libraries(ktexteditor_zealsearch Qt5::Gui KF5::TextEditor KF5::XmlGui KF5::I18n)

install(TARGETS ktexteditor_zealsearch DESTINATION ${PLUGIN_INSTALL_DIR}/ktexteditor)
