; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#include "scripts\products.iss"
#include "scripts\products\dotnetfx35.iss"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{06F25DC8-71E2-44E2-805A-F15E15B51C74}
;AppId=Remove Empty Directories
AppName=Remove Empty Directories
;AppVersion=2.2
AppVersion=2.2 (Admin Editon)
AppPublisher=Jonas John
AppCopyright=Remove Empty Directories ? Jonas John 2006-2011
AppPublisherURL=http://www.jonasjohn.de/
AppSupportURL=http://www.jonasjohn.de/
AppUpdatesURL=http://www.jonasjohn.de/
DefaultDirName={pf}\Remove Empty Directories
DefaultGroupName=Remove Empty Directories
AllowNoIcons=true
LicenseFile=license.txt
;OutputBaseFilename=red-v2.2-setup
OutputBaseFilename=red-v2.2-setup-admin
SourceDir=.
OutputDir=output\
;SetupIconFile=red.ico
Compression=lzma
SolidCompression=true
UninstallDisplayIcon={app}\RED2.exe

;MinVersion=4.1,5.0
;PrivilegesRequired=admin
;ArchitecturesAllowed=x86 x64 ia64
;ArchitecturesInstallIn64BitMode=x64 ia64

[Languages]
Name: en; MessagesFile: compiler:Default.isl
;Name: "de"; MessagesFile: "compiler:Languages\German.isl"

[Tasks]
Name: desktopicon; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked
; Name: quicklaunchicon; Description: {cm:CreateQuickLaunchIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked; OnlyBelowVersion: 0,6.1
;Name: common; Description: "&All users"; GroupDescription: "Install For:"; Flags: exclusive unchecked
;Name: local;  Description: "Just &me"; GroupDescription: "Install For:"; Flags: exclusive

[Files]
Source: RED2.exe; DestDir: {app}; Flags: ignoreversion
Source: license.txt; DestDir: {app}; Flags: ignoreversion
Source: credits.txt; DestDir: {app}; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[InstallDelete]
; Delete old files from RED 2.1
Name: {group}\Readme file.lnk; Type: files
Name: {group}\Uninstall.lnk; Type: files
Name: {app}\readme.txt; Type: files
Name: {app}\lgpl.txt; Type: files
Name: {app}\RED2.exe.manifest; Type: files
Name: {app}\uninst.exe; Type: files

[UninstallDelete]
; cleanup - delete config dir
Name: {localappdata}\Remove_Empty_Directories; Type: filesandordirs; Tasks: ; Languages: 
; Delete application directory if empty
Name: {app}; Type: dirifempty

[Icons]
Name: {group}\Remove Empty Directories; Filename: {app}\RED2.exe
Name: {group}\{cm:UninstallProgram,Remove Empty Directories}; Filename: {uninstallexe}
Name: {commondesktop}\Remove Empty Directories; Filename: {app}\RED2.exe; Tasks: desktopicon
;Name: {userappdata}\Microsoft\Internet Explorer\Quick Launch\Remove Empty Directories; Filename: {app}\RED2.exe; Tasks: quicklaunchicon

[Registry]
; remove explorer context entry if exists
Root: HKCR; Subkey: Folder\shell\Remove empty dirs; ValueType: string; Flags: uninsdeletekey deletekey dontcreatekey; Tasks: ; Languages: 

[Run]
Filename: {app}\RED2.exe; Description: {cm:LaunchProgram,Remove Empty Directories}; Flags: nowait postinstall skipifsilent shellexec; Tasks: ; Languages: 

[Code]

function InitializeSetup(): Boolean;
var
    UninstallerPath: String;
    ErrorCode: Integer;
begin
    //install .net 3.5 if possible
    dotnetfx35();

    // Check whether application is already installed
    if RegKeyExists(HKEY_LOCAL_MACHINE,  'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Remove Empty Directories') then
    begin
        if MsgBox('The setup has detected that RED is already installed on your computer. Do you wish to uninstall the previous version and continue with a fresh installation?', mbConfirmation, MB_YESNO) = IDYES then
        begin

            RegQueryStringValue(HKEY_LOCAL_MACHINE, 'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Remove Empty Directories', 'UninstallString', UninstallerPath);

            // ShellExec('runas', uninstaller, '/SILENT', '', SW_HIDE, ewWaitUntilTerminated, ErrorCode);
            // use above statement if extra level security is required usually it is not req
            //ShellExec('open', UninstallerPath, '/S', '', SW_SHOW, ewWaitUntilTerminated, ErrorCode);
            ShellExec('runas', UninstallerPath, '/S', '', SW_SHOW, ewWaitUntilTerminated, ErrorCode);
            //Exec(UninstallerPath, '/S', '', SW_SHOWNORMAL, ewWaitUntilTerminated, ErrorCode);
        end;
    end;

    Result := true;
end;
