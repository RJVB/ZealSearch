add_definitions(-DTRANSLATION_DOMAIN=\"ktexteditor_zealsearch\")
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

set(ktexteditor_zealsearch_SRCS zealsearch_config.cpp zealsearchplugin.cpp zealsearchview.cpp)
# resource for ui file and stuff
qt5_add_resources(ktexteditor_zealsearch_SRCS plugin.qrc)

add_library(ktexteditor_zealsearch MODULE ${ktexteditor_zealsearch_SRCS})
kcoreaddons_desktop_to_json(ktexteditor_zealsearch ktexteditor_zealsearch.desktop)
# kcoreaddons_desktop_to_json(ktexteditor_zealsearch ktexteditor_zealsearch_config.desktop)

target_link_libraries(ktexteditor_zealsearch Qt5::Gui KF5::TextEditor KF5::XmlGui KF5::I18n)

install(TARGETS ktexteditor_zealsearch DESTINATION ${PLUGIN_INSTALL_DIR}/ktexteditor)

# install(FILES zealsearchui.rc DESTINATION ${DATA_INSTALL_DIR}/ktexteditor_zealsearch)

# install(FILES ktexteditor_zealsearch.desktop ktexteditor_zealsearch_config.desktop DESTINATION ${SERVICES_INSTALL_DIR})
