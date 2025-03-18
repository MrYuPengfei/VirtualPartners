# VirtualPartners

## 介绍

VirtualPartners是一款基于C++ Qt 6开发的电脑桌面伙伴的开源软件。

### 项目亮点

#### 1. 跨平台兼容性

- 支持 Windows 和 Linux 系统，在不同操作系统上都能稳定运行。例如在 Linux 系统中，通过 CMake 进行特定的设置，查找并链接 Vulkan 和 XKB 等库，确保程序在 Linux 环境下正常编译和运行。在 Windows 系统中，也有相应的构建和部署方法，如使用 `windeployqt` 确保应用在无 Qt 环境的设备上可运行。

#### 2. 丰富的功能实现

- **动画展示**：通过 `PetWindow` 类中的 `updateAnimation` 函数，实现角色动画的定时更新，循环播放动画列表中的图像，给用户带来生动的视觉体验。

- **音频播放**：使用 `QMediaPlayer` 和 `QAudioOutput` 实现背景音乐和角色语音的播放，并可以通过系统托盘菜单控制音频的开关。

- **系统托盘菜单**：提供了丰富的系统托盘菜单功能，包括角色选择、背景音乐选择、音频开关控制、显示帮助文档和版本文档等。

#### 3. 良好的用户交互体验

- **窗口拖动**：重写 `mousePressEvent` 函数，使用 `windowHandle()->startSystemMove()` 实现窗口的拖动功能，跨平台性能好，不会出现光标和窗口脱离的情况。

- **菜单操作**：通过创建 `QMenu` 和 `QAction`，实现了了解、闲聊、隐藏、退出等操作，方便用户与程序进行交互。

#### 4. 模块化设计

- 代码结构清晰，将不同功能模块封装在不同的类中，如 `PetWindow` 类负责桌面伙伴窗口的主要功能，`SettingsWindow` 类负责设置窗口的界面和操作，提高了代码的可维护性和可扩展性。

#### 5. 易于部署和分发

- 提供了多种部署方式，如在 Windows 系统中使用 `windeployqt` 和 Inno 6 进行构建生成安装包；在 Linux 系统中可以将程序打包成 `.deb` 文件，还可以使用 Docker 进行容器化部署，确保应用在不同环境中具有一致性。

### 面试介绍思路

#### 1. 项目概述

- 简要介绍项目名称 `VirtualPartners`，它是一款基于 C++ 和 Qt 6 开发的电脑桌面伙伴开源软件，主要功能是在桌面显示原神主题的角色动画，支持音频播放和用户交互操作。

#### 2. 技术选型

- 强调选择 Qt 6 的原因，如跨平台特性、丰富的 GUI 库和多媒体模块，能够快速实现动画展示和音频播放功能。同时提及使用 CMake 进行项目构建，方便管理不同平台的编译配置。

#### 3. 核心功能实现

- **动画展示**：详细介绍 `updateAnimation` 函数的实现原理，如何通过定时更新图像实现角色动画的循环播放。
- **音频播放**：说明使用 `QMediaPlayer` 和 `QAudioOutput` 实现背景音乐和角色语音播放的过程，以及如何通过系统托盘菜单控制音频开关。
- **窗口交互**：解释重写 `mousePressEvent` 函数实现窗口拖动的方法，以及创建系统托盘菜单和相关操作的实现思路。

#### 4. 跨平台兼容性

- 阐述项目在 Windows 和 Linux 系统上的不同配置和部署方法，如在 Linux 系统中查找并链接 Vulkan 和 XKB 库，在 Windows 系统中使用 `windeployqt` 确保应用的可移植性。
  
  #### 5. 项目亮点和挑战

- 强调项目的跨平台兼容性、丰富的功能和良好的用户交互体验等亮点。同时提及在开发过程中遇到的挑战，如解决在无 Qt 环境的设备上运行问题、优化动画性能等，并说明自己是如何克服这些挑战的。

#### 6. 未来展望

- 对项目未来的改进和扩展计划，增加更多角色和动画、优化音频效果、支持更多系统等。

## 跨平台部署

### Windows

#### 使用方法

点击安装包安装到Windows10/11上后，请点击任务栏设置，其他系统托盘图标，将VirtualPartners.exe设置为开启状态。
Ctrl+q退出程序，
Ctrl+“+”放大程序，
Ctrl+“-”缩小程序。
通过任务栏可以设置音效是否开启，以及选择你喜爱的原神角色。

#### 代码调试

在QtCreater14.0.1中调试需要正确配置CMake，设置可执行程序的工作目录为项目的根目录（不是可执行文件所在的目录）。
在VS2022中调试需要修改launch.vs.json，同样需要设置工作目录。

```
{
  "version": "0.2.1",
  "defaults": {},
  "configurations": [
    {
      "type": "default", 
      "name": "我写的配置文件",
      "project": "CMakeLists.txt",
      "projectTarget": "VirturalPartners.exe",

      "cwd": "${workspaceRoot}",
      "currentDir": "${workspaceRoot}"
    }
  ]
}
```

#### 生成安装包

使用windeployqt和inno 6 进行构建

#### windeployqt

使用windeployqt是一个自动化的方式来确保你的Qt应用程序在没有安装Qt的系统上也能运行。以下是使用windeployqt的详细步骤：
步骤1：编译应用程序
确保你的应用程序已经以Release模式编译完成。
步骤2：打开Qt命令行工具
在Windows系统中，你可以通过以下方式打开Qt的命令行工具：
在开始菜单中找到Qt的文件夹。
在打开的子菜单中选择“Command Prompt”或“MinGW”等，这取决于你使用的编译器。
步骤3：定位到应用程序的目录
使用cd命令在命令行中切换到你的应用程序的Release或Debug目录。
步骤4：运行windeployqt
在命令行中输入windeployqt命令，后面跟上你的应用程序的可执行文件名。
```
D:\QTProjects\VirtualPartners\build\Desktop_Qt_6_8_2_MSVC2022_64bit-Release\release>windeployqt VirtualPartners.exe
```

#### inno 6

下载inno6安装包生成工具，打开setup.iss文件进行构建。

### Linux

支持Linux，操作过程与Windows类似。

#### 
