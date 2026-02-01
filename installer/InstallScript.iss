; TCR compliant Sample install script.
; Basically put this file in the root for what you want included in the installer,
;   then put everything into the subdirectories as listed in the installer below.
;   Remember to change the AppId
;   Thanks to Dan Weiss (dweiss@digipen.edu) for the original version.

[Setup]
; !!NOTE!!: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click "Tools" | "Create GUID" inside the Visual Studio IDE.)
AppId={{79E42D11-8735-42FB-9023-92E3B334F323}


; Standard app data stuff
; !!NOTE!!: Replace all instances of "Desktop-Companion" with your game name.
AppName=Desktop-Companion
AppVerName=Desktop-Companion Version 1
AppPublisher=jspoh
AppPublisherURL=https://jspoh.dev/
AppSupportURL=https://jspoh.dev/

; Default path to the file storage directory.
; {pf} is the default program files directory set by Windows
DefaultDirName={pf}\jspoh\Desktop-Companion

; Start menu directory
DefaultGroupName=jspoh\Desktop-Companion

; Output directory for the installer.
OutputDir=.\INSTALLER

; Setup executable installer
OutputBaseFilename=Desktop-Companion_Setup

; Path to the EULA (Needed to pass TCRs)
LicenseFile=INSTALLERFILES\jspoh_EULA.txt

; Path to the icon for the installer (TCR check requires custom icon)
SetupIconFile=.\INSTALLERFILES\favicon.ico

; Compression scheme for the installer. Check Inno Setup help files for more options.
Compression=lzma
SolidCompression=yes

; This allows the installer to run without admin privileges, which means you can't install
; in the Program Files, or change the registry. This is done to allow installation on Sandbox
; or other intermediate directory
PrivilegesRequired=none

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
; Creates an installer option to allow/disallow desktop shortcut
; Checked by default
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"
Name: "startup"; Description: "Run Desktop-Companion at Windows startup"; GroupDescription: "Additional tasks:"

[Registry]
; Add to Windows startup if the user checked the option
Root: HKCU; Subkey: "Software\Microsoft\Windows\CurrentVersion\Run"; ValueType: string; ValueName: "Desktop-Companion"; ValueData: """{app}\desktop-companion.exe"""; Flags: uninsdeletevalue; Tasks: startup

; delete cofig/data file in ~/Documents
[UninstallDelete]
Type: filesanddirsrecursively; Name: "{userdocs}\jspoh desktop companion\jdc"

;
; !!NOTE!!: Your game may not require the VC or DX redistributable installers.
;	Update the [Files] and [Run] lists (below) as necessary for your game.
;

; This is the list of files that the installer should grab and install.
;
; Destination Directory Notes
;   {app} is the root directory that you will be installing to.
;   {temp} is a temporary directory that Windows sets that gets deleted after the
;      installer quits.
;   {userdocs} is the directory for My Documents/Documents on Windows XP, Vista, and 7.
;
; For more information on default paths to install to, check the "Constants" article
;   in the Inno Setup 5 documentation.
;
; I recommend placing any installers for required stuff (DirectX, PhysX, etc)
;   in the general structure below to keep things neat for you.
[Files]
; The game directory is exactly what you want your install directory in program files to look like
Source: .\GAMEDIRECTORY\*; DestDir: {app}; Flags: ignoreversion recursesubdirs createallsubdirs
; Include the redistributable programs and install them to the temp directory
; Source: .\REDIST\vcredist_2008_x86.exe; DestDir: {tmp}; Flags: ignoreversion
Source: .\REDIST\VC_redist.x64.exe; DestDir: {tmp}; Flags: ignoreversion
; Source: .\REDIST\dxwebsetup.exe; DestDir: {tmp}; Flags: ignoreversion

; This is the list of shortcuts that the installer will setup for you.
; Of note, this will create the uninstaller automatically.
;
; Directory Notes
;   {group} is the start menu location that the game will install shortcuts to.
;   {commondesktop} is your Windows desktop directory.
[Icons]
Name: {group}\Desktop-Companion; Filename: {app}\desktop-companion.exe; WorkingDir: {app}; IconFilename: "{app}\assets\favicon.ico"
Name: {group}\{cm:UninstallProgram,Desktop-Companion}; Filename: {uninstallexe}
Name: {group}\{cm:UninstallProgram,Desktop-Companion}; Filename: {uninstallexe};  IconFilename: "{app}\assets\favicon.ico"
Name: {commondesktop}\Desktop-Companion; Filename: {app}\desktop-companion.exe; Tasks: desktopicon; WorkingDir: {app}; IconFilename: "{app}\assets\favicon.ico"

; List of items to execute in the installer.
; Note that this will run all executables in their silent versions as required by the TCRs.
;
; The last item being run is the installer option to automatically launch the game after
;   the installer exits as required by the TCRs.
[Run]
;Filename: {tmp}\vc_redist.x86.exe; Parameters: /q; StatusMsg: Installing Visual C++ 2015 Redistributable...
Filename: {tmp}\VC_redist.x64.exe; Parameters: /q; StatusMsg: Installing Visual C++ 2022 Redistributable...
;Filename: {tmp}\dxwebsetup.exe; Parameters: /q; StatusMsg: Installing DirectX...
Filename: {app}\desktop-companion.exe; Description: {cm:LaunchProgram,Desktop-Companion}; Flags: nowait postinstall skipifsilent


;To be able to run and generate the Desktop-Companion_Setup.exe you need to download and install "Inno Setup" application, then open this file with the application.