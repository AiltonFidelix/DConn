#define DConnName "DConn"
#define DConnVersion "1.0.0"
#define DConnExeName "DConn.exe"
#define DConnIcon "icons/icon.ico"

[Setup]
AppName={#DConnName}
AppVersion={#DConnVersion}
DefaultDirName="{autopf}\{#DConnName}"
OutputBaseFilename="{#DConnName}-{#DConnVersion}-Setup-amd64"
SetupIconFile={#DConnIcon}
SourceDir="."
ArchitecturesInstallIn64BitMode=x64
ArchitecturesAllowed=x64
AppPublisher=Ailton Fidelix
AppPublisherURL=https://github.com/AiltonFidelix/DConn
AppSupportURL=https://github.com/AiltonFidelix/DConn
AppUpdatesURL=https://github.com/AiltonFidelix/DConn

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: desktopicon; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons}
Name: quicklaunchicon; Description: {cm:CreateQuickLaunchIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked

[Files]
Source: build\release\{#DConnExeName}; DestDir: {app}; DestName: {#DConnExeName}; Flags: ignoreversion
Source: build\release\Qt6Core.dll; DestDir: {app}; Flags: ignoreversion
Source: build\release\Qt6Gui.dll; DestDir: {app}; Flags: ignoreversion
Source: build\release\Qt6Widgets.dll; DestDir: {app}; Flags: ignoreversion
Source: build\release\qwindows.dll; DestDir: {app}\platforms\; Flags: ignoreversion
Source: build\release\qmodernwindowsstyle.dll; DestDir: {app}\styles\; Flags: ignoreversion
Source: build\release\libgcc_s_seh-1.dll; DestDir: {app}; Flags: ignoreversion
Source: build\release\libstdc++-6.dll; DestDir: {app}; Flags: ignoreversion

[Icons]
Name: "{group}\{#DConnName}"; Filename: {app}\{#DConnExeName}
Name: {group}\{cm:UninstallProgram,{#DConnName}}; Filename: {uninstallexe}
Name: {commondesktop}\{#DConnName}; Filename: {app}\{#DConnExeName}; Tasks: desktopicon
Name: {userappdata}\Microsoft\Internet Explorer\Quick Launch\{#DConnName}; Filename: {app}\{#DConnExeName}; Tasks: quicklaunchicon

[Run]
Filename: "{app}\{#DConnExeName}"; Description: "{cm:LaunchProgram,{#DConnName}}"; Flags: nowait postinstall skipifsilent
