cmake_minimum_required(VERSION 3.19)
project(VirtualPartners VERSION 1.8 LANGUAGES CXX)
# 查找Qt6及其组件
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Multimedia LinguistTools)
# 标准项目设置
qt_standard_project_setup()

qt_add_executable(${PROJECT_NAME}
    WIN32 MACOSX_BUNDLE
    main.cpp
    PetWindow.cpp
    SettingsWindow.cpp
    PetWindow.h
    SettingsWindow.h
    config.h
)

# 链接Qt库
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Multimedia
)



# Linux特定的设置
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # 查找Vulkan和XKB
    find_package(Vulkan REQUIRED)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(XKBCOMMON REQUIRED xkbcommon)

    # 链接Vulkan和XKB库
    target_link_libraries(${PROJECT_NAME} PRIVATE ${Vulkan_LIBRARIES} ${XKBCOMMON_LIBRARIES})

    # 包含Vulkan和XKB的头文件目录
    target_include_directories(${PROJECT_NAME} PRIVATE ${Vulkan_INCLUDE_DIRS} ${XKBCOMMON_INCLUDE_DIRS})

    include(GNUInstallDirs)

    install(TARGETS ${PROJECT_NAME}
        BUNDLE  DESTINATION .
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
endif()

# Windows特定的设置（如果需要）
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"  # 或者你希望的目录
    )
endif()
