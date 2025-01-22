# 项目配置
TEMPLATE = app
TARGET = VirtualPartners
VERSION = 1.8

# 源文件
SOURCES += \
    main.cpp \
    PetWindow.cpp \
    # SettingsWindow.cpp \
    config.h

HEADERS += \
    PetWindow.h \
    # SettingsWindow.h

# Qt 模块
QT = core widgets multimedia

# Linux 特定配置
unix {
    # 查找 Vulkan 和 XKB
    message("Checking for Vulkan and XKBCommon...")
    system(pkg-config --exists vulkan xkbcommon || exit 1)
    Vulkan_LIBS = $$system(pkg-config --libs vulkan)
    XKBCommon_LIBS = $$system(pkg-config --libs xkbcommon)
    Vulkan_INCLUDES = $$system(pkg-config --cflags vulkan)
    XKBCommon_INCLUDES = $$system(pkg-config --cflags xkbcommon)

    # 链接库
    LIBS += $$Vulkan_LIBS $$XKBCommon_LIBS
    INCLUDEPATH += $$Vulkan_INCLUDES $$XKBCommon_INCLUDES

    # 安装目标
    INSTALLS += target
    target.path = $$[QT_INSTALL_BINS]
}

# Windows 特定配置
win32 {
    # 设置输出目录
    DESTDIR = $${PWD}/bin/
    TARGET = $$DESTDIR/$${TARGET}
    CONFIG(debug, debug|release) {
        TARGET = $$DESTDIR/$${TARGET}_debug
    }
}
