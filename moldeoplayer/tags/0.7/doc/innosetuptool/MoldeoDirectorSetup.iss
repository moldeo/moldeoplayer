; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Moldeo Director"
#define MyAppVerName "Moldeo Director 0.7"
#define MyAppPublisher "Moldeo"
#define MyAppURL "http://www.moldeo.com.ar/"
#define MyAppExeName "moldeodirector.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{929E6185-F93D-41C7-A074-FA0823D595CE}
AppName={#MyAppName}
AppVerName={#MyAppVerName}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\Moldeo
DefaultGroupName=Moldeo
LicenseFile=D:\_data\proyectos\SINESTESIA\OPENSVN\trunk\0.7\bin\mingw\moldeo license.txt
OutputDir=D:\_data\proyectos\SINESTESIA\OPENSVN\trunk\0.7\doc\installers
OutputBaseFilename=Moldeo Director 0.7 Setup
SetupIconFile=D:\_data\proyectos\SINESTESIA\OPENSVN\trunk\0.7\doc\icons\Moldeo32.ico
Compression=lzma
SolidCompression=yes

[Languages]
Name: english; MessagesFile: compiler:Default.isl
Name: basque; MessagesFile: compiler:Languages\Basque.isl
Name: brazilianportuguese; MessagesFile: compiler:Languages\BrazilianPortuguese.isl
Name: catalan; MessagesFile: compiler:Languages\Catalan.isl
Name: czech; MessagesFile: compiler:Languages\Czech.isl
Name: danish; MessagesFile: compiler:Languages\Danish.isl
Name: dutch; MessagesFile: compiler:Languages\Dutch.isl
Name: finnish; MessagesFile: compiler:Languages\Finnish.isl
Name: french; MessagesFile: compiler:Languages\French.isl
Name: german; MessagesFile: compiler:Languages\German.isl
Name: hebrew; MessagesFile: compiler:Languages\Hebrew.isl
Name: hungarian; MessagesFile: compiler:Languages\Hungarian.isl
Name: italian; MessagesFile: compiler:Languages\Italian.isl
Name: norwegian; MessagesFile: compiler:Languages\Norwegian.isl
Name: polish; MessagesFile: compiler:Languages\Polish.isl
Name: portuguese; MessagesFile: compiler:Languages\Portuguese.isl
Name: russian; MessagesFile: compiler:Languages\Russian.isl
Name: slovak; MessagesFile: compiler:Languages\Slovak.isl
Name: slovenian; MessagesFile: compiler:Languages\Slovenian.isl
Name: spanish; MessagesFile: compiler:Languages\Spanish.isl

[Tasks]
Name: desktopicon; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked
Name: quicklaunchicon; Description: {cm:CreateQuickLaunchIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked

[Files]
Source: ..\..\bin\mingw\moldeodirector.exe; DestDir: {app}\bin\mingw
Source: ..\..\bin\mingw\moldeo_mingw_sdl.exe; DestDir: {app}\bin\mingw
; NOTE: Don't use "Flags: ignoreversion" on any shared system files
Source: ..\..\bin\mingw\FreeImage.dll; DestDir: {app}\bin\mingw; Tasks: ; Languages: 
Source: ..\..\bin\mingw\moldeo license.txt; DestDir: {app}\bin\mingw
Source: ..\..\bin\mingw\moldeo_mingw_sdl.exe; DestDir: {app}\bin\mingw
Source: ..\..\bin\mingw\moldeodirector.exe; DestDir: {app}\bin\mingw
Source: ..\..\bin\mingw\muparser.dll; DestDir: {app}\bin\mingw
Source: ..\..\bin\mingw\SDL.dll; DestDir: {app}\bin\mingw
Source: ..\..\bin\mingw\wxbase28_gcc_custom.dll; DestDir: {app}\bin\mingw
Source: ..\..\bin\mingw\wxbase28_net_gcc_custom.dll; DestDir: {app}\bin\mingw
Source: ..\..\bin\mingw\wxbase28_xml_gcc_custom.dll; DestDir: {app}\bin\mingw
Source: ..\..\bin\mingw\wxmsw28_adv_gcc_custom.dll; DestDir: {app}\bin\mingw
Source: ..\..\bin\mingw\wxmsw28_aui_gcc_custom.dll; DestDir: {app}\bin\mingw
Source: ..\..\bin\mingw\wxmsw28_core_gcc_custom.dll; DestDir: {app}\bin\mingw
Source: ..\..\bin\mingw\wxmsw28_gl_gcc_custom.dll; DestDir: {app}\bin\mingw
Source: ..\..\bin\mingw\wxmsw28_html_gcc_custom.dll; DestDir: {app}\bin\mingw
Source: ..\..\bin\mingw\wxmsw28_qa_gcc_custom.dll; DestDir: {app}\bin\mingw
Source: ..\..\bin\mingw\wxmsw28_richtext_gcc_custom.dll; DestDir: {app}\bin\mingw
Source: ..\..\bin\mingw\wxmsw28_xrc_gcc_custom.dll; DestDir: {app}\bin\mingw
Source: ..\..\bin\mingw\FreeImage.dll; DestDir: {app}\bin\mingw
Source: ..\..\bin\mingw\glew_shared.dll; DestDir: {app}\bin\mingw
Source: ..\..\bin\mingw\libmoldeo.dll; DestDir: {app}\bin\mingw
Source: ..\..\bin\mingw\libpng13.dll; DestDir: {app}\bin\mingw
Source: ..\..\bin\mingw\mingwm10.dll; DestDir: {app}\bin\mingw
Source: ..\..\bin\mingw\plugins\effects\image.dll; DestDir: {app}\bin\mingw\plugins\effects
Source: ..\..\bin\mingw\plugins\effects\icon.dll; DestDir: {app}\bin\mingw\plugins\effects
Source: ..\..\bin\mingw\plugins\iodevices\mouse.dll; DestDir: {app}\bin\mingw\plugins\iodevices
Source: ..\..\bin\mingw\plugins\iodevices\keyboard.dll; DestDir: {app}\bin\mingw\plugins\iodevices
Source: ..\..\bin\mingw\plugins\iodevices\mixer.dll; DestDir: {app}\bin\mingw\plugins\iodevices
Source: ..\..\bin\mingw\plugins\mastereffects\presetpanel.dll; DestDir: {app}\bin\mingw\plugins\mastereffects
Source: ..\..\bin\mingw\plugins\mastereffects\channel.dll; DestDir: {app}\bin\mingw\plugins\mastereffects
Source: ..\..\bin\mingw\plugins\mastereffects\ligia.dll; DestDir: {app}\bin\mingw\plugins\mastereffects
Source: ..\..\bin\mingw\plugins\mastereffects\masterchannel.dll; DestDir: {app}\bin\mingw\plugins\mastereffects
Source: ..\..\bin\mingw\plugins\mastereffects\panel.dll; DestDir: {app}\bin\mingw\plugins\mastereffects
Source: ..\..\bin\mingw\plugins\preeffects\erase.dll; DestDir: {app}\bin\mingw\plugins\preeffects
Source: ..\..\bin\mingw\plugins\posteffects\debug.dll; DestDir: {app}\bin\mingw\plugins\posteffects
Source: ..\..\data\test\objetos\iconos3d\moldeologo.3ds; DestDir: {app}\data\test\objetos\iconos3d
Source: ..\..\data\test\iconos\estrellas\shine.tga; DestDir: {app}\data\test\iconos\estrellas
Source: ..\..\data\test\iconos\estrellas\star.tga; DestDir: {app}\data\test\iconos\estrellas
Source: ..\..\data\test\iconos\estrellas\spark.tga; DestDir: {app}\data\test\iconos\estrellas
Source: ..\icons\Moldeo32.ico; DestDir: {app}\doc\icons
Source: ..\..\data\test\videomanager.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\channel0.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\icon.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\image.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\keyboard.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\ligia.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\masterchannel.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\mixer.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\mouse.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\panel.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\presetpanel.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\protconect.par; DestDir: {app}\data\test
Source: ..\..\data\test\shadermanager.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\soundmanager.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\test.mol; DestDir: {app}\data\test
Source: ..\..\data\test\shaders\Edges.cfg; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\Edges.glsl; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\Posterize.cfg; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\Posterize.glsl; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\fuentes\texto00.png; DestDir: {app}\data\test\fuentes
Source: ..\..\bin\mingw\plugins\effects\plane.dll; DestDir: {app}\bin\mingw\plugins\effects
Source: ..\..\bin\mingw\plugins\effects\tunel.dll; DestDir: {app}\bin\mingw\plugins\effects
Source: ..\..\bin\mingw\plugins\effects\cubemap.dll; DestDir: {app}\bin\mingw\plugins\effects
Source: ..\..\bin\mingw\plugins\effects\democube.dll; DestDir: {app}\bin\mingw\plugins\effects
Source: ..\..\bin\mingw\plugins\effects\icon3d.dll; DestDir: {app}\bin\mingw\plugins\effects
Source: ..\..\bin\mingw\plugins\effects\icon.dll; DestDir: {app}\bin\mingw\plugins\effects
Source: ..\..\bin\mingw\plugins\effects\image.dll; DestDir: {app}\bin\mingw\plugins\effects
Source: ..\..\bin\mingw\plugins\effects\imageflow.dll; DestDir: {app}\bin\mingw\plugins\effects
Source: ..\..\bin\mingw\plugins\effects\livedrawing2.dll; DestDir: {app}\bin\mingw\plugins\effects
Source: ..\..\bin\mingw\plugins\effects\movie.dll; DestDir: {app}\bin\mingw\plugins\effects
Source: ..\..\bin\mingw\plugins\effects\painter.dll; DestDir: {app}\bin\mingw\plugins\effects
Source: ..\..\data\test\shaders\TexInterpolator.glsl; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\BilinearFilter.cfg; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\BilinearFilter.glsl; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\Blur.cfg; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\Blur.glsl; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\ColorMatrix.cfg; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\ColorMatrix.glsl; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\ColorPart.cfg; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\Copy.cfg; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\Copy.glsl; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\CopyRGB.cfg; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\CopyRGB.glsl; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\DynEmboss.cfg; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\DynEmboss.glsl; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\DynSaturate.cfg; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\DynSaturate.glsl; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\Edges.cfg; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\Edges.glsl; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\Edgesxml.cfg; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\GridEmboss.cfg; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\Max.glsl; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\Maxxml.cfg; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\Mixer.cfg; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\Mixer.glsl; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\MovePart.cfg; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\Pixelate.cfg; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\Pixelate.glsl; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\PixelateDyn2.glsl; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\PixelateDyn.glsl; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\Posterize.cfg; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\Posterize.glsl; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\PulseEmboss.cfg; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\PulseGrid.glsl; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\RenderAveGrad.cfg; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\RenderBrushes.cfg; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\RenderGrad2fp.cfg; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\Saturate.cfg; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\Saturate.glsl; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\Sharpen.cfg; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\Sharpen.glsl; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\StaticEmboss.glsl; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\shaders\TexInterpolator.cfg; DestDir: {app}\data\test\shaders
Source: ..\..\data\test\cubemap.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\channel0.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\debug.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\democube.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\erase.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\icon3d.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\icon.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\image.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\keyboard.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\ligia.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\livedrawing2.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\masterchannel.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\mirror.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\mirrorg.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\mixer.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\mouse.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\painter.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\panel.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\plane.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\presetpanel.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\protconect.par; DestDir: {app}\data\test
Source: ..\..\data\test\shadermanager.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\soundmanager.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\tablet.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\test.mol; DestDir: {app}\data\test
Source: ..\..\data\test\tunel.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\videomanager.cfg; DestDir: {app}\data\test
Source: ..\..\data\test\videowall.cfg; DestDir: {app}\data\test
Source: ..\..\bin\mingw\plugins\iodevices\tablet.dll; DestDir: {app}\bin\mingw\plugins\iodevices
Source: ..\..\data\test\shaders\livedrawing\VertexProcessing.glsl; DestDir: {app}\data\test\shaders\livedrawing
Source: ..\..\data\test\shaders\livedrawing\LineAntialising.cfg; DestDir: {app}\data\test\shaders\livedrawing
Source: ..\..\data\test\shaders\livedrawing\LineAntialising.glsl; DestDir: {app}\data\test\shaders\livedrawing
Source: ..\..\data\test\images\pajarito.png; DestDir: {app}\images
Source: ..\..\data\test\images\pececito.png; DestDir: {app}\images
Source: ..\..\data\test\images\pintura.jpg; DestDir: {app}\images
Source: ..\..\bin\mingw\plugins\posteffects\videowall.dll; DestDir: {app}\bin\mingw\plugins\posteffects
Source: ..\..\bin\mingw\plugins\preeffects\mirrorg.dll; DestDir: {app}\bin\mingw\plugins\preeffects
Source: ..\..\bin\mingw\plugins\preeffects\mirror.dll; DestDir: {app}\bin\mingw\plugins\preeffects
Source: ..\..\bin\mingw\cgGL.dll; DestDir: {app}\bin\mingw
Source: ..\..\bin\mingw\cg.dll; DestDir: {app}\bin\mingw
Source: ..\installers\gstreamer-0.10.17.exe; DestDir: {app}\doc\installers; Tasks: ; Languages: 
Source: ..\installers\gst-ffmpeg-0.10.2.exe; DestDir: {app}\doc\installers
Source: ..\installers\gst-plugins-bad-0.10.6.exe; DestDir: {app}\doc\installers
Source: ..\installers\gst-plugins-base-0.10.17.exe; DestDir: {app}\doc\installers
Source: ..\installers\gst-plugins-good-0.10.7.exe; DestDir: {app}\doc\installers
Source: ..\installers\gst-plugins-ugly-0.10.7.exe; DestDir: {app}\doc\installers
Source: ..\..\bin\mingw\glut32.dll; DestDir: {app}\bin\mingw
Source: ..\..\bin\mingw\OpenAL32.dll; DestDir: {app}\bin\mingw
Source: ..\installers\gtk-2.12.9-win32-2.exe; DestDir: {app}\doc\installers
Source: ..\..\data\test\video\video2.avi; DestDir: {app}\data\test\video
Source: ..\..\data\test\video\video1.avi; DestDir: {app}\data\test\video
Source: ..\..\bin\mingw\plugins\effects\democube.dll; DestDir: {app}\bin\mingw\plugins\effects
Source: ..\..\data\test\brushes\brush1.tga; DestDir: {app}\data\test\brushes
Source: ..\..\data\test\brushes\brush2.tga; DestDir: {app}\data\test\brushes
Source: ..\..\data\test\brushes\brush3.tga; DestDir: {app}\data\test\brushes
Source: ..\..\data\test\brushes\brush_normal.tga; DestDir: {app}\data\test\brushes
Source: ..\..\data\test\brushes\diagonal.tga; DestDir: {app}\data\test\brushes
Source: ..\..\data\test\brushes\diagonal_lines.tga; DestDir: {app}\data\test\brushes
Source: ..\..\data\test\brushes\dots.tga; DestDir: {app}\data\test\brushes
Source: ..\..\data\test\brushes\simple_round.tga; DestDir: {app}\data\test\brushes
Source: ..\Logo Moldeo.png; DestDir: {app}\doc
Source: ..\logomoldeo2.jpg; DestDir: {app}\doc
Source: ..\logomoldeo.jpg; DestDir: {app}\doc
Source: ..\start.html; DestDir: {app}\doc
Source: ..\icons\fullscreen32.png; DestDir: {app}\doc\icons
Source: ..\icons\pause32.png; DestDir: {app}\doc\icons
Source: ..\icons\play32.png; DestDir: {app}\doc\icons
Source: ..\icons\savesession32.png; DestDir: {app}\doc\icons
Source: ..\icons\savevideo32.png; DestDir: {app}\doc\icons
Source: ..\icons\screenshot32.png; DestDir: {app}\doc\icons
Source: ..\icons\imageempty.png; DestDir: {app}\doc\icons
Source: ..\icons\pickcolor.png; DestDir: {app}\doc\icons
Source: ..\icons\rotate.png; DestDir: {app}\doc\icons
Source: ..\icons\translate.png; DestDir: {app}\doc\icons
Source: ..\icons\colors.png; DestDir: {app}\doc\icons
Source: ..\icons\scale.png; DestDir: {app}\doc\icons
Source: ..\icons\functiontest.png; DestDir: {app}\doc\icons
Source: ..\icons\imageempty.png; DestDir: {app}\doc\icons
Source: ..\icons\import.png; DestDir: {app}\doc\icons
Source: ..\icons\inspect.png; DestDir: {app}\doc\icons
Source: ..\icons\motioncircular.png; DestDir: {app}\doc\icons
Source: ..\icons\motiondiagonal.png; DestDir: {app}\doc\icons
Source: ..\icons\motionhorizontal.png; DestDir: {app}\doc\icons
Source: ..\icons\motionrandom.png; DestDir: {app}\doc\icons
Source: ..\icons\motionvertical.png; DestDir: {app}\doc\icons
Source: ..\icons\select.png; DestDir: {app}\doc\icons
Source: ..\..\bin\mingw\globals.bat; DestDir: {app}\bin\mingw

[Icons]
Name: {group}\{#MyAppName}; Filename: {app}\bin\mingw\{#MyAppExeName}; WorkingDir: {app}\bin\mingw\; IconFilename: {app}\doc\icons\Moldeo32.ico; Comment: Moldeo Director 0.7 GUI for Realtime Animated Images Composition; IconIndex: 0
Name: {group}\{cm:ProgramOnTheWeb,{#MyAppName}}; Filename: {#MyAppURL}
Name: {group}\{cm:UninstallProgram,{#MyAppName}}; Filename: {uninstallexe}
Name: {commondesktop}\{#MyAppName}; Filename: {app}\bin\mingw\{#MyAppExeName}; Tasks: desktopicon; WorkingDir: {app}\bin\mingw\; IconFilename: {app}\doc\icons\Moldeo32.ico; IconIndex: 0; Comment: Moldeo Director 0.7 GUI for Realtime Animated Images Composition
Name: {userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}; Filename: {app}\bin\mingw\{#MyAppExeName}; Tasks: quicklaunchicon; WorkingDir: {app}\bin\mingw\; IconFilename: {app}\doc\icons\Moldeo32.ico; IconIndex: 0; Comment: Moldeo Director 0.7 GUI for Realtime Animated Images Composition

[Run]
Filename: {app}\bin\mingw\globals.bat; WorkingDir: {app}\bin\mingw
Filename: {app}\doc\installers\gtk-2.12.9-win32-2.exe; Flags: postinstall
Filename: {app}\doc\installers\gstreamer-0.10.17.exe; Flags: postinstall
Filename: {app}\doc\installers\gst-plugins-ugly-0.10.7.exe; Flags: postinstall
Filename: {app}\doc\installers\gst-plugins-good-0.10.7.exe; Flags: postinstall
Filename: {app}\doc\installers\gst-plugins-base-0.10.17.exe; Flags: postinstall
Filename: {app}\doc\installers\gst-plugins-bad-0.10.6.exe; Flags: postinstall
Filename: {app}\doc\installers\gst-ffmpeg-0.10.2.exe; Flags: postinstall
Filename: {app}\bin\mingw\{#MyAppExeName}; Description: {cm:LaunchProgram,{#MyAppName}}; Flags: nowait postinstall skipifsilent; Tasks: ; Languages: 

[Dirs]
Name: {app}\bin
Name: {app}\bin\mingw
Name: {app}\data
Name: {app}\bin\mingw\plugins
Name: {app}\bin\mingw\plugins\effects
Name: {app}\bin\mingw\plugins\preeffects
Name: {app}\bin\mingw\plugins\posteffects
Name: {app}\bin\mingw\plugins\mastereffects
Name: {app}\bin\mingw\plugins\iodevices
Name: {app}\bin\mingw\plugins\resources
Name: {app}\data\test\fuentes
Name: {app}\data\test
Name: {app}\data\test\objetos
Name: {app}\data\test\objetos\iconos3d
Name: {app}\data\test\iconos
Name: {app}\data\test\imagenes
Name: {app}\data\test\fuentes
Name: {app}\data\test\shaders
Name: {app}\data\test\video
Name: {app}\data\test\scripts
Name: {app}\data\test\iconos\estrellas
Name: {app}\doc
Name: {app}\doc\icons
Name: {app}\data\test\shaders\livedrawing
Name: {app}\images
Name: {app}\doc
Name: {app}\doc\installers
Name: {app}\data\test\brushes
[Registry]
Root: HKLM; Subkey: SOFTWARE\Microsoft\Windows\CurrentVersion\Moldeo\bin\mingw\moldeodirector.exe; ValueType: string; ValueName: ; ValueData: {app}\moldeodirector.exe; Flags: uninsdeletekey