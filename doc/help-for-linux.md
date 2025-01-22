## 安装 Vulkan 
sudo apt update
sudo apt install libvulkan-dev vulkan-tools vulkan-validationlayers-dev

##  Linux Mint 上将 Qt 程序打包成`.deb`文件（即 Debian 安装包）
在 Linux Mint 上将 Qt 程序打包成`.deb`文件（即 Debian 安装包），可以通过以下步骤完成。这种方法适用于需要分发到其他 Linux 系统（尤其是基于 Debian 的系统）的 Qt 应用程序。


步骤 1：安装必要的工具
在 Linux Mint 上，你需要安装一些工具来帮助打包，例如`dpkg`、`fakeroot`和`lintian`。


```bash
sudo apt update
sudo apt install dpkg-dev fakeroot lintian
```



步骤 2：准备打包目录
创建一个目录结构来存放你的应用程序和打包所需的文件。通常，这个目录结构会包括以下部分：

• `DEBIAN`：存放控制文件（如`control`文件）。

• `usr/local/bin`：存放可执行文件。

• `usr/share/applications`：存放`.desktop`文件（用于启动器）。

• `usr/share/icons`：存放图标文件。

例如，假设你的应用程序名为`VirtualPartners`，可以创建以下目录结构：


```bash
mkdir -p ./mint_build/VirtualPartners/DEBIAN
mkdir -p ./mint_build/VirtualPartners/usr/local/bin
mkdir -p ./mint_build/VirtualPartners/usr/share/applications
mkdir -p ./mint_build/VirtualPartners/usr/share/icons
```



步骤 3：将应用程序文件复制到打包目录
将你的 Qt 应用程序的可执行文件和依赖库复制到相应的目录中。例如：


```bash
cp ./build/Desktop_Qt_6_8_1-Release/VirtualPartners ./mint_build/VirtualPartners/usr/local/bin/
```


如果你使用了`linuxdeployqt`来收集依赖库，可以将生成的文件夹内容复制到`VirtualPartners/usr/local/bin`中。


步骤 4：创建控制文件
在`DEBIAN`文件夹中创建一个`control`文件，它包含了包的元数据。例如：


```bash
vim ./mint_build/VirtualPartners/DEBIAN/control
```


然后，在`control`文件中填写以下内容：


```plaintext
Package: virtualpartners
Version: 1.8
Section: utils
Priority: optional
Architecture: amd64
Depends: libc6, libqt6gui6, libqt6core6
Maintainer: Your Name <your.email@example.com>
Description: A Qt-based application for virtual partners
```



• `Package`：包的名称。

• `Version`：版本号。

• `Architecture`：目标架构（`amd64`表示 64 位系统）。

• `Depends`：依赖的库（根据你的应用程序实际依赖的库填写）。

• `Maintainer`：维护者信息。

• `Description`：包的描述。


步骤 5：创建启动器文件
在`usr/share/applications`文件夹中创建一个`.desktop`文件，用于在菜单中启动应用程序。例如：


```bash
nano ./mint_build/VirtualPartners/usr/share/applications/virtualpartners.desktop
```


然后，在`.desktop`文件中填写以下内容：


```plaintext
[Desktop Entry]
Version=1.0
Type=Application
Name=Virtual Partners
Exec=/usr/local/bin/VirtualPartners
Icon=/usr/share/icons/virtualpartners.png
Comment=A Qt-based application for virtual partners
Categories=Utility;
```



• `Name`：应用程序的名称。

• `Exec`：可执行文件的路径。

• `Icon`：图标文件的路径。

• `Categories`：分类（如`Utility`、`Game`等）。


步骤 6：打包
进入包含打包目录的父目录，运行以下命令来打包：


```bash
cd path/to/VirtualPartners
fakeroot dpkg-deb --build VirtualPartners
```


这将生成一个`.deb`文件，例如`VirtualPartners.deb`。


步骤 7：测试安装包
将生成的`.deb`文件安装到你的系统上，测试是否正常运行：


```bash
sudo dpkg -i VirtualPartners.deb
```


如果安装过程中提示缺少依赖库，可以根据提示安装缺失的库，或者在`control`文件中添加正确的依赖关系。


步骤 8：分发
你可以将生成的`.deb`文件分发给其他用户。他们可以通过以下命令安装：


```bash
sudo dpkg -i VirtualPartners.deb
```



---



使用`linuxdeployqt`自动化依赖收集
如果你的 Qt 应用程序依赖于多个动态库，可以使用`linuxdeployqt`工具来自动化收集依赖库。以下是使用`linuxdeployqt`的步骤：


• 安装`linuxdeployqt`：

• 下载`linuxdeployqt`：

```bash
     wget https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage
     ```


• 设置可执行权限：

```bash
     chmod +x linuxdeployqt-continuous-x86_64.AppImage
     ```



• 运行`linuxdeployqt`：

• 进入你的应用程序的构建目录：

```bash
     cd path/to/your/build
     ```


• 运行`linuxdeployqt`：

```bash
     ./linuxdeployqt-continuous-x86_64.AppImage VirtualPartners -appimage
     ```


这将生成一个包含所有依赖库的 AppImage 文件，你可以将其分发给其他用户。


---



总结
通过上述步骤，你可以将 Qt 应用程序打包成`.deb`文件，或者使用`linuxdeployqt`生成 AppImage 文件，以便在 Linux 系统上分发和安装。如果还有其他问题，请随时告诉我！