[Setup]
; 应用程序的名称和版本
AppName=VirtualPartners
AppVersion=1.1

; 安装程序的输出文件名
OutputDir=./inno_build
OutputBaseFilename=VirtualPartners安装向导
; 安装程序的窗口设置
SetupIconFile=data\icon256.ico
WizardImageFile=data\icon256.bmp
WizardSmallImageFile=data\icon256.bmp
AppPublisher=于鹏飞
AppPublisherURL=https://gitee.com/yupengfei1074064684/yuanshen-desktoppet/tree/master

; 其他设置...
DefaultDirName={commonpf}\VirtualPartners
DisableProgramGroupPage=yes
Compression=lzma
SolidCompression=yes

[Languages]
Name: "zh_CN"; MessagesFile: "ChineseSimplified.isl"

[Files]
; 包含 PyInstaller 打包的应用程序目录
Source: "VirtualPartners.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "data\icon256.ico"; DestDir: "{app}"; Flags: ignoreversion
Source: "data\*"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
[Icons]
; 创建开始菜单的快捷方式;设置快捷方式的图标
Name: "{group}\VirtualPartners"; Filename: "{app}\VirtualPartners.exe";IconFilename: "{app}\icon256.ico"

[Code]
procedure CurPageChanged(CurPageID: Integer);
begin
  if CurPageID = wpWelcome then
  begin
    // 用户处于欢迎页面时显示消息框
    MsgBox('感谢您安装VirtualPartners。如需支持，请联系作者:于鹏飞，Email:mr_yupengfei@foxmail.com', mbInformation, MB_OK);
  end;
end;

[Run]
; 安装后运行应用程序
Filename: "{app}\VirtualPartners.exe"; Description: "{cm:LaunchProgram,VirtualPartners}"; Flags: nowait postinstall