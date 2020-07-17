#define ADMB_VERSION "ADMB-12.2"
#define ADMB_VERSION2 "admb-12.2"

[Setup]
PrivilegesRequired=none
AppName={#ADMB_VERSION}
AppVerName={#ADMB_VERSION}
UsePreviousAppDir=no
UsePreviousGroup=no
DefaultDirName={sd}\{#ADMB_VERSION}
DefaultGroupName={#ADMB_VERSION}
UninstallDisplayIcon={app}\uninstall.exe
OutputDir=..\..\..\build
OutputBaseFilename={#ADMB_VERSION2}
AppPublisher=ADMB Project
AppPublisherURL=http://www.admb-project.org/
LicenseFile=..\..\..\LICENSE.txt
Compression=lzma2/ultra64

[Types]
Name: {#ADMB_VERSION2}; Description: {#ADMB_VERSION}

[Files]
Source: "..\..\..\build\admb\*"; DestDir: "{app}"; Flags: recursesubdirs
Source: "..\..\set-admb-command-prompt.bat"; DestDir: "{app}\bin"
Source: "C:\rtools40\mingw64\*"; DestDir: "{app}\utilities\mingw64"; Flags: recursesubdirs

[Icons]
Name: "{app}\{#ADMB_VERSION} Command Prompt"; Filename: "{cmd}"; Parameters: "/K ""{app}\bin\set-admb-command-prompt.bat"""; WorkingDir: "{app}"; Comment: "{#ADMB_VERSION} Command Prompt"
Name: "{group}\{#ADMB_VERSION} Command Prompt"; Filename: "{cmd}"; Parameters: "/K ""{app}\bin\set-admb-command-prompt.bat"""; WorkingDir: "{app}"; Comment: "{#ADMB_VERSION} Command Prompt"
Name: "{group}\Uninstall {#ADMB_VERSION}"; Filename: "{uninstallexe}"; Comment: "Uninstall {#ADMB_VERSION}"
