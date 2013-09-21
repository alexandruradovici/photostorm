;PhotoStorm Desktop Win32-0.1.1 Install Script

[Setup]
AppName=PhotoStorm Desktop
AppVerName=PhotoStorm Desktop Win32-0.1.1
AppVersion=0.1.1
AppPublisher=Project Info
AppPublisherURL=http://www.photostorm.net
AppCopyright=Copyright (C) 2004 Project Info
AppSupportURL=http://www.photostorm.net
AppUpdatesURL=http://www.photostorm.net
DefaultDirName={pf}\PhotoStorm
DefaultGroupName=PhotoStorm Desktop
DisableStartupPrompt=no
WindowShowCaption=yes
WindowVisible=no
LicenseFile=gpl.txt
Compression=bzip/9
SourceDir=.
OutputDir=.\Kit
OutputBaseFilename=PhotoStorm_Win32-0.1.1
ChangesAssociations=yes
AdminPrivilegesRequired=yes
WizardImageFile=setup_image_win32.bmp
UninstallDisplayIcon=photos.ico

[Tasks]
Name: "programgroup"; Description: "Create Start Menu Group"; GroupDescription: "Additional icons:"
Name: "desktopicon"; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"

[Registry]
Root: HKLM; Subkey: "Software\Microsoft\Windows\CurrentVersion\App Paths\photostorm.exe"; Flags: uninsdeletekeyifempty
Root: HKLM; Subkey: "Software\Microsoft\Windows\CurrentVersion\App Paths\photostorm.exe"; ValueType: string; ValueData: "{app}\photostorm.exe"; Flags: uninsdeletevalue
Root: HKLM; Subkey: "Software\Microsoft\Windows\CurrentVersion\App Paths\photostorm.exe"; ValueType: string; ValueData: "{app}\photostorm.exe"; Flags: uninsdeletevalue
Root: HKLM; Subkey: "Software\Microsoft\Windows\CurrentVersion\App Paths\photostorm.exe"; ValueType: string; ValueName: "Path"; ValueData: "{app};{code:GetGtkPath}\bin"; Flags: uninsdeletevalue
Root: HKLM; Subkey: "Software\Microsoft\Windows\CurrentVersion\App Paths\ps_trayicon.exe"; Flags: uninsdeletekeyifempty
Root: HKLM; Subkey: "Software\Microsoft\Windows\CurrentVersion\App Paths\ps_trayicon.exe"; ValueType: string; ValueData: "{app}\ps_trayicon.exe"; Flags: uninsdeletevalue
Root: HKLM; Subkey: "Software\Microsoft\Windows\CurrentVersion\App Paths\ps_trayicon.exe"; ValueType: string; ValueData: "{app}\ps_trayicon.exe"; Flags: uninsdeletevalue
Root: HKLM; Subkey: "Software\Microsoft\Windows\CurrentVersion\App Paths\ps_trayicon.exe"; ValueType: string; ValueName: "Path"; ValueData: "{app};{code:GetGtkPath}\bin"; Flags: uninsdeletevalue
Root: HKLM; Subkey: "Software\PhotoStorm"; ValueName: "Libs"; ValueType: string; ValueData: "{app}"; Flags: uninsdeletevalue
Root: HKLM; Subkey: "Software\Microsoft\Windows\CurrentVersion\Run"; ValueType: string; ValueName: "PhotoStorm Desktop Tray Icon Server"; ValueData: "{app}\ps_trayicon.exe"; Flags: uninsdeletevalue
Root: HKCR; Subkey: ".psa"; ValueType: string; ValueData: "PhotoStorm.Album"; Flags: uninsdeletevalue
Root: HKCR; Subkey: ".psa"; ValueType: string; ValueName: "Content Type"; ValueData: "album/psa"; Flags: uninsdeletevalue
Root: HKCR; Subkey: ".psp"; ValueType: string; ValueData: "PhotoStorm.Photo"; Flags: uninsdeletevalue
Root: HKCR; Subkey: ".psp"; ValueType: string; ValueName: "Content Type"; ValueData: "photo/psp"; Flags: uninsdeletevalue
Root: HKCR; Subkey: "PhotoStorm.Album\DefaultIcon"; ValueType: string; ValueData: "{app}\photostorm.exe,0"; Flags: uninsdeletevalue
Root: HKCR; Subkey: "PhotoStorm.Album\shell"; ValueType: string; ValueData: "open"; Flags: uninsdeletevalue
Root: HKCR; Subkey: "PhotoStorm.Album\shell\open\command"; ValueType: string; ValueData: """{app}\photostorm.exe"" ""%1"""; Flags: uninsdeletevalue
Root: HKCR; Subkey: "PhotoStorm.Photo\DefaultIcon"; ValueType: string; ValueData: "{app}\photostorm.exe,0"; Flags: uninsdeletevalue
Root: HKCR; Subkey: "PhotoStorm.Photo\shell"; ValueType: string; ValueData: "open"; Flags: uninsdeletevalue
Root: HKCR; Subkey: "PhotoStorm.Photo\shell\open\command"; ValueType: string; ValueData: """{app}\photostorm.exe"" ""%1"""; Flags: uninsdeletevalue

[Files]
Source: "photostorm.exe"; DestDir: "{app}"; DestName: "photostorm.exe"; CopyMode: normal
Source: "ps_trayicon.exe"; DestDir: "{app}"; DestName: "ps_trayicon.exe"; CopyMode: normal
Source: "bgd.dll"; DestDir: "{app}"; DestName: "bgd.dll"; CopyMode: normal
Source: "png2bmp.exe"; DestDir: "{app}"; DestName: "png2bmp.exe"; CopyMode: normal
Source: "ps_uninstall.exe"; DestDir: "{app}"; DestName: "ps_uninstall.exe"; CopyMode: normal
Source: "Albums\gnome.psa"; DestDir: "{app}\Albums"; DestName: "gnome.psa"; CopyMode: normal
Source: "Quotes\quotes1.psq"; DestDir: "{app}\Quotes"; DestName: "quotes1.psq"; CopyMode: normal
Source: "pics\char\add_photo.png"; DestDir: "{app}\pics\char"; DestName: "add_photo.png"; CopyMode: normal
Source: "pics\logos\photostorm.jpg"; DestDir: "{app}\pics\logos"; DestName: "photostorm.jpg"; CopyMode: normal
Source: "pics\logos\splash_screen_win32.jpg"; DestDir: "{app}\pics\logos"; DestName: "splash_screen.jpg"; CopyMode: normal
Source: "pics\logos\wizard_1.png"; DestDir: "{app}\pics\logos"; DestName: "wizard_1.png"; CopyMode: normal
Source: "pics\picts\album.png"; DestDir: "{app}\pics\picts"; DestName: "album.png"; CopyMode: normal
Source: "pics\picts\collection.png"; DestDir: "{app}\pics\picts"; DestName: "collection.png"; CopyMode: normal
Source: "pics\picts\photos.png"; DestDir: "{app}\pics\picts"; DestName: "photos.png"; CopyMode: normal
Source: "pics\picts\photos_applet.png"; DestDir: "{app}\pics\picts"; DestName: "photos_applet.png"; CopyMode: normal
Source: "pics\picts\photostorm.png"; DestDir: "{app}\pics\picts"; DestName: "photostorm.png"; CopyMode: normal
Source: "pics\picts\properties.png"; DestDir: "{app}\pics\picts"; DestName: "properties.png"; CopyMode: normal
Source: "pics\picts\wallpaper.png"; DestDir: "{app}\pics\picts"; DestName: "wallpaper.png"; CopyMode: normal
Source: "pics\picts\website.png"; DestDir: "{app}\pics\picts"; DestName: "website.png"; CopyMode: normal
Source: "pics\picts\website24.png"; DestDir: "{app}\pics\picts"; DestName: "website24.png"; CopyMode: normal
Source: "fonts\arial.ttf"; DestDir: "{app}\fonts"; DestName: "arial.ttf"; CopyMode: normal
Source: "fonts\ariali.ttf"; DestDir: "{app}\fonts"; DestName: "ariali.ttf"; CopyMode: normal
Source: "fonts\comic.ttf"; DestDir: "{app}\fonts"; DestName: "comic.ttf"; CopyMode: normal
Source: "fonts\monosb.ttf"; DestDir: "{app}\fonts"; DestName: "monosb.ttf"; CopyMode: normal
Source: "fonts\signature.ttf"; DestDir: "{app}\fonts"; DestName: "signature.ttf"; CopyMode: normal
Source: "fonts\tahoma.ttf"; DestDir: "{app}\fonts"; DestName: "tahoma.ttf"; CopyMode: normal
Source: "languages\ro.psl"; DestDir: "{app}\languages"; DestName: "ro.psl"; CopyMode: normal
Source: "Docs\index.html"; DestDir: "{app}\Docs"; DestName: "index.html"; CopyMode: normal
Source: "Docs\welcome.html"; DestDir: "{app}\Docs"; DestName: "welcome.html"; CopyMode: normal
Source: "Docs\contents.html"; DestDir: "{app}\Docs"; DestName: "contents.html"; CopyMode: normal
Source: "Docs\menu_logo.html"; DestDir: "{app}\Docs"; DestName: "menu_logo.html"; CopyMode: normal
Source: "Docs\menu.html"; DestDir: "{app}\Docs"; DestName: "menu.html"; CopyMode: normal
Source: "Docs\about.html"; DestDir: "{app}\Docs"; DestName: "about.html"; CopyMode: normal
Source: "Docs\images\ps_help.jpg"; DestDir: "{app}\Docs\images"; DestName: "ps_help.jpg"; CopyMode: normal
Source: "Docs\css\ps_help.css"; DestDir: "{app}\Docs\css"; DestName: "ps_help.css"; CopyMode: normal
Source: "Gtk-Wimp\libwimp.dll"; DestDir: "{code:GetGtkPath}\lib\gtk-2.0\2.4.0\engines"; DestName: "libwimp.dll"; CopyMode: alwaysoverwrite
Source: "Gtk-Wimp\gtkrc"; DestDir: "{code:GetGtkPath}\share\themes\Default\gtk-2.0"; DestName: "gtkrc"; CopyMode: alwaysoverwrite

[Icons]
Name: "{group}\Photostorm Desktop"; Filename: "{app}\photostorm.exe"; Tasks: programgroup; Comment: "Manage your albums, add, modify or delete photos, select wallpapers.";
Name: "{group}\System Tray Icon"; Filename: "{app}\ps_trayicon.exe"; Tasks: programgroup; Comment: "Displays the PhotoStorm System Tray Icon and takes care to update the wallpaper.";
Name: "{group}\Uninstall"; Filename: "{uninstallexe}"; Tasks: programgroup; Comment: "Uninstall PhotoStorm";
Name: "{commondesktop}\PhotoStorm Desktop"; Filename: "{app}\photostorm.exe"; Tasks: desktopicon; Comment: "Manage your albums, add, modify or delete photos, select wallpapers.";

[Run]
Filename: "ps_trayicon.exe"; Parameters: "--about"; Description: "PhotoStorm System Tray Icon"; Flags: postinstall nowait shellexec
Filename: "photostorm.exe"; Parameters: "--new_wallpaper"; Description: "Make a new wallpaper"; Flags: postinstall nowait shellexec

[UninstallDelete]
Type: filesandordirs; Name: "{app}\.photostorm";
Type: filesandordirs; Name: "{app}\languages"
Type: filesandordirs; Name: "{userappdata}\.photostorm\photostorm.cfg"

[UninstallRun]
Filename: "{app}\ps_uninstall.exe"; StatusMsg: "Stopping PhotoStorm Tray Icon Server";

[Code]

var
  Exists: Boolean;
  GtkPath: String;

function GetGtkInstalled (): Boolean;
begin
  Exists := RegQueryStringValue (HKLM, 'Software\GTK\2.0', 'Path', GtkPath);
  if not Exists then begin
    Exists := RegQueryStringValue (HKCU, 'Software\GTK\2.0', 'Path', GtkPath);
  end;
   Result := Exists
end;

procedure writeGtkPath ();
begin
  GetGtkInstalled ();
  RegWriteStringValue (HKLM, 'Software\Microsoft\Windows\CurrentVersion\App Paths\photostorm.exe', 'Path', WizardDirValue+';'+GtkPath+'\bin');
  RegWriteStringValue (HKLM, 'Software\Microsoft\Windows\CurrentVersion\App Paths\ps_trayicon.exe', 'Path', WizardDirValue+';'+GtkPath+'\bin');
end;

function GetGtkPath (S: String): String;
begin
    Result := GtkPath;
end;

function InitializeSetup(): Boolean;
begin
  Result := GetGtkInstalled ();
  if not Result then begin
    MsgBox ('The GTK 2.4.3 Libraries are not installed. Please get the gtk-2.4.3 libraries installer from http://www.photostorm.net or http://www.dropline.net. You may also download the photostorm install with the gtk-2.4.3 libraries from http://www.photostorm.net.', mbError, MB_OK);
  end;
end;

procedure DeinitializeSetup ();
begin
  writeGtkPath ();
end;


