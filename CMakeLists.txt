cmake_minimum_required(VERSION 3.16)

project(test_nested_lists_2025_10_31 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(apptest_nested_lists_2025_10_31
    main.cpp
)

qt_add_qml_module(apptest_nested_lists_2025_10_31
    URI test_nested_lists_2025_10_31
    VERSION 1.0
    QML_FILES
        Main.qml
        CardsView.qml
    SOURCES
        fieldlistmodel.h
        fieldlistmodel.cpp
        chunklistmodel.h
        chunklistmodel.cpp
        cardsmodel.h
        cardsmodel.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(apptest_nested_lists_2025_10_31 PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.apptest_nested_lists_2025_10_31
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(apptest_nested_lists_2025_10_31
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS apptest_nested_lists_2025_10_31
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
