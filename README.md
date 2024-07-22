# VirtualPartners

### 介绍
VirtualPartners是一款基于C++ Qt 6.2.4开发的桌面电子宠物软件。
### 使用方法
点击安装包安装到Windows11上后，请点击任务栏设置，其他系统托盘图标，将VirtualPartners.exe设置为开启状态。
Ctrl+q退出程序，
Ctrl+“+”放大程序，
Ctrl+“-”缩小程序。
通过任务栏可以设置音效是否开启，以及选择你喜爱的原神角色。

### 代码调试
在QtCreater13.0.1中需要正确配置CMake，设置可执行程序的工作目录为项目的根目录（不是可执行文件所在的目录）。

### 生成安装包
使用windeployqt和inno 6 进行构建
#### windeployqt
使用windeployqt是一个自动化的方式来确保你的Qt应用程序在没有安装Qt的系统上也能运行。以下是使用windeployqt的详细步骤：

步骤1：编译应用程序
确保你的应用程序已经以Release模式编译完成。这通常在Qt Creator中完成，或者使用CMake等构建系统。

步骤2：打开Qt命令行工具
在Windows系统中，你可以通过以下方式打开Qt的命令行工具：

在开始菜单中找到Qt的文件夹。
选择对应的Qt版本，比如“Qt 6.5.0”。
在打开的子菜单中选择“Command Prompt”或“MinGW”等，这取决于你使用的编译器。
步骤3：定位到应用程序的目录
使用cd命令在命令行中切换到你的应用程序的Release或Debug目录。例如：

bash
cd C:\Path\To\Your\Application\Release
步骤4：运行windeployqt
在命令行中输入windeployqt命令，后面跟上你的应用程序的可执行文件名。
#### inno 6
下载inno6安装包生成工具，打开setup.iss文件进行构建。
