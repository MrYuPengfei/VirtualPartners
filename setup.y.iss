#define MyAppName "原神桌面宠物"
#define MyAppVersion "V1.6"
[Setup]
; 应用程序的名称和版本
; 安装程序的输出文件名
OutputDir=./inno_build
AppName={#MyAppName}
AppVersion={#MyAppVersion}
OutputBaseFilename={#MyAppName}{#MyAppVersion}安装向导
; 安装程序的窗口设置
SetupIconFile=data\icon256.ico
WizardImageFile=data\icon256.bmp
WizardSmallImageFile=data\icon256.bmp
AppPublisher=于鹏飞
AppPublisherURL=https://gitee.com/yupengfei1074064684/virtual-partners4-pc.git

; 其他设置...

DefaultDirName={commonpf}\{#MyAppName}
DisableProgramGroupPage=yes
//Compression=None
Compression=lzma
SolidCompression=yes

[Languages]
Name: "zh_CN"; MessagesFile: "ChineseSimplified.isl"

[Files]
; 包含 PyInstaller 打包的应用程序目录
Source: "build\Desktop_Qt_6_7_2_MSVC2019_64bit-MinSizeRel\VirtualPartners.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "build\Desktop_Qt_6_7_2_MSVC2019_64bit-MinSizeRel\*.dll"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "data\icon256.ico"; DestDir: "{app}"; Flags: ignoreversion
Source: "data\*"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
//Source: "data.zip"; DestDir: "{tmp}"; Flags: ignoreversion 
//Source: "unzip.exe"; DestDir: "{tmp}"; Flags: ignoreversion
//Source: "data.zip"; DestDir: "{tmp}"; Flags: deleteafterinstall
[Icons]
; 创建开始菜单的快捷方式;设置快捷方式的图标
Name: "{group}\原神桌面宠物"; Filename: "{app}\VirtualPartners.exe";IconFilename: "{app}\icon256.ico"

[Code]
procedure CurPageChanged(CurPageID: Integer);
begin
  if CurPageID = wpWelcome then
  begin
    // 用户处于欢迎页面时显示消息框
    MsgBox('感谢您安装原神桌面宠物。如需支持，请联系作者:于鹏飞，Email:mr_yupengfei@foxmail.com', mbInformation, MB_OK);
  end;
end;
[Run]
; 安装后运行应用程序
//Filename: "{tmp}\unzip.exe"; Parameters: "x ""{tmp}\data.zip"" -d ""{app}"""; Flags: nowait postinstall skipifsilent
Filename: "{app}\VirtualPartners.exe"; Description: "{cm:LaunchProgram,原神桌面宠物}"; Flags: nowait postinstall
