cmake_minimum_required(VERSION 3.5)

project(VirtualPartners VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
# find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)
# 查找 Qt
find_package(Qt6 COMPONENTS Core Widgets Multimedia REQUIRED)

# 包含 Qt 的头文件目录
include_directories(${Qt6_INCLUDE_DIRS})

# # 寻找 SQLite3 库
# find_package(SQLite3 REQUIRED)
# add_library(sqlite3 sqlite3.c)

# 包含 SQLite3 头文件目录
include_directories(${SQLite3_INCLUDE_DIRS})

# 定义源文件变量
set(SOURCE_FILES
    main.cpp
    PetWindow.cpp
)

# 定义头文件变量（如果有额外的头文件）
set(HEADER_FILES
    PetWindow.h config.h
)

# 启用 Qt 的自动 moc、uic 和 rcc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 定义应用程序
# qt_add_resources(MY_RESOURCE_FILES PetWindow.qrc)

# add_executable(${PROJECT_NAME} ${MY_RESOURCE_FILES} ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES})

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES}
    VERSION.md
    HELP.md)

# 链接 Qt 模块
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets Qt6::Multimedia)

# 链接 SQLite3 库
# target_link_libraries(MyExecutable ${SQLite3_LIBRARIES})

# 后缀处理，确保资源文件被正确处理
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

# 包含 Qt 的模块化工具集
target_compile_definitions(${PROJECT_NAME} PRIVATE $<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>, QT_QML_DEBUG>)

target_link_libraries(VirtualPartners PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)



if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.VirtualPartners)
endif()
set_target_properties(VirtualPartners PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# include(GNUInstallDirs)
# install(TARGETS VirtualPartners
#     BUNDLE DESTINATION .
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# )

# if)
# if(QT_VERSION_MAJOR EQUAL 6)
#     qt_finalize_executable(VirtualPartners)
# endif()

# endif()


# 如果是 Android 平台，添加 Android 特定的源文件和库
if(ANDROID)
    # 添加 Android 特定的源文件
    # list(APPEND SOURCE_FILES
    #     android_specific_file.cpp
    # )

    # 链接 Android 特定的库
    find_library(LOG_LIB log)
    find_library(ANDROID_LIB android)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${LOG_LIB} ${ANDROID_LIB})

    # 设置应用的包名和版本信息
    set_target_properties(${PROJECT_NAME} PROPERTIES
                # ANDROID_PACKAGE_NAME "com.example.virtualpartners"
        ANDROID_PACKAGE_NAME "virtualpartners"
 #        ANDROID_VERSION_CODE 1
   #      ANDROID_VERSION_NAME "0.1"
    )
endif()

# 其他平台的设置...
