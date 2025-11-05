cmake_minimum_required(VERSION 3.16)

project(temp_qt_quick_nested_flow VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(apptemp_qt_quick_nested_flow
    main.cpp
)

qt_add_qml_module(apptemp_qt_quick_nested_flow
    URI temp_qt_quick_nested_flow
    QML_FILES
        Main.qml
        CardView.qml
        Draw.qml
        Cell.qml
        EditText.qml
    SOURCES
        cardmodel.h
        cardmodel.cpp
        SOURCES cellmodel.h cellmodel.cpp
        # basecard.h
        # basecard.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(apptemp_qt_quick_nested_flow PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.apptemp_qt_quick_nested_flow
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(apptemp_qt_quick_nested_flow
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS apptemp_qt_quick_nested_flow
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
