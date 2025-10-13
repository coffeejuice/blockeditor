cmake_minimum_required(VERSION 3.16)

project(blockeditor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Core)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appblockeditor
    main.cpp
)

qt_add_qml_module(appblockeditor
    URI blockeditor
    VERSION 1.0
    QML_FILES
        Main.qml
        BlocksListView.qml
        BlockDelegateLayout.qml
        BlockDelegate_Header.qml
        BlockDelegate_Document.qml
        BlockDelegate_Block.qml
        BlockDelegate_Heat.qml
        BlockDelegate_Upset.qml
        BlockDelegate_Draw.qml
        BlockDelegate_Unknown.qml
        BlockDelegate_Footer.qml
    RESOURCES
        assets/image0.jpg
        assets/image1.jpg
        assets/image2.jpg
        assets/image3.jpg
        assets/image4.jpg
        assets/image5.jpg
        assets/image6.jpg
        assets/image7.jpg
        assets/image8.jpg
        assets/image9.jpg
        assets/blocktype_unknown_yellow_inkspace.svg
        assets/fire.svg
        assets/red_danger.svg
        assets/check_list_bw.svg
    SOURCES
        blockslistmodel.h
        blockslistmodel.cpp
        blocks.h
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appblockeditor PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appblockeditor
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appblockeditor
    PRIVATE Qt6::Quick
)
target_link_libraries(appblockeditor PRIVATE Qt6::Core)

include(GNUInstallDirs)
install(TARGETS appblockeditor
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
