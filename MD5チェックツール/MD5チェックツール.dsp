# Microsoft Developer Studio Project File - Name="MD5チェックツール" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MD5チェックツール - Win32 DebugW
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "MD5チェックツール.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "MD5チェックツール.mak" CFG="MD5チェックツール - Win32 DebugW"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "MD5チェックツール - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "MD5チェックツール - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE "MD5チェックツール - Win32 ReleaseW" ("Win32 (x86) Application" 用)
!MESSAGE "MD5チェックツール - Win32 DebugW" ("Win32 (x86) Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MD5チェックツール - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\shared\keccak1600-mmx.obj ..\shared\md5-586.obj ..\shared\sha1-586.obj ..\shared\sha256-586.obj ..\shared\sha512-586.obj ..\shared\x86cpuid.obj ..\shared\x86iTaskbarList3.obj ..\shared\x86iFileDialog.obj /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\shared\keccak1600-mmx.obj ..\shared\md5-586.obj ..\shared\sha1-586.obj ..\shared\sha256-586.obj ..\shared\sha512-586.obj ..\shared\x86cpuid.obj ..\shared\x86iTaskbarList3.obj ..\shared\x86iFileDialog.obj ..\shared\x86crc32c.obj /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
TargetPath=\Documents and Settings\Administrator\My Documents\Projects\Release\MD5チェックツール.exe
SOURCE="$(InputPath)"
PostBuild_Cmds=mt.exe -nologo -outputresource:"$(TargetPath)" -manifest "..\shared\Common-Controls.manifest" "..\shared\DeclareDPIAware.manifest" "..\shared\supportedOS.manifest" "..\shared\requestedExecutionLevel.manifest"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "MD5チェックツール - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\shared\keccak1600-mmx.obj ..\shared\md5-586.obj ..\shared\sha1-586.obj ..\shared\sha256-586.obj ..\shared\sha512-586.obj ..\shared\x86cpuid.obj ..\shared\x86iTaskbarList3.obj ..\shared\x86iFileDialog.obj /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\shared\keccak1600-mmx.obj ..\shared\md5-586.obj ..\shared\sha1-586.obj ..\shared\sha256-586.obj ..\shared\sha512-586.obj ..\shared\x86cpuid.obj ..\shared\x86iTaskbarList3.obj ..\shared\x86iFileDialog.obj /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
TargetPath=\Documents and Settings\Administrator\My Documents\Projects\Debug\MD5チェックツール.exe
SOURCE="$(InputPath)"
PostBuild_Cmds=mt.exe -nologo -outputresource:"$(TargetPath)" -manifest "..\shared\Common-Controls.manifest" "..\shared\DeclareDPIAware.manifest" "..\shared\supportedOS.manifest" "..\shared\requestedExecutionLevel.manifest"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "MD5チェックツール - Win32 ReleaseW"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\ReleaseW"
# PROP BASE Intermediate_Dir "ReleaseW"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\ReleaseW"
# PROP Intermediate_Dir "ReleaseW"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\shared\keccak1600-mmx.obj ..\shared\md5-586.obj ..\shared\sha1-586.obj ..\shared\sha256-586.obj ..\shared\sha512-586.obj ..\shared\x86cpuid.obj ..\shared\x86iTaskbarList3.obj ..\shared\x86iFileDialog.obj /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\shared\keccak1600-mmx.obj ..\shared\md5-586.obj ..\shared\sha1-586.obj ..\shared\sha256-586.obj ..\shared\sha512-586.obj ..\shared\x86cpuid.obj ..\shared\x86iTaskbarList3.obj ..\shared\x86iFileDialog.obj ..\shared\x86crc32c.obj /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /opt:nowin98
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
TargetPath=\Documents and Settings\Administrator\My Documents\Projects\ReleaseW\MD5チェックツール.exe
SOURCE="$(InputPath)"
PostBuild_Cmds=mt.exe -nologo -outputresource:"$(TargetPath)" -manifest "..\shared\Common-Controls.manifest" "..\shared\DeclareDPIAware.manifest" "..\shared\supportedOS.manifest" "..\shared\requestedExecutionLevel.manifest"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "MD5チェックツール - Win32 DebugW"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\DebugW"
# PROP BASE Intermediate_Dir "DebugW"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\DebugW"
# PROP Intermediate_Dir "DebugW"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\shared\keccak1600-mmx.obj ..\shared\md5-586.obj ..\shared\sha1-586.obj ..\shared\sha256-586.obj ..\shared\sha512-586.obj ..\shared\x86cpuid.obj ..\shared\x86TaskbarList3.obj ..\shared\x86iFileDialog.obj /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\shared\keccak1600-mmx.obj ..\shared\md5-586.obj ..\shared\sha1-586.obj ..\shared\sha256-586.obj ..\shared\sha512-586.obj ..\shared\x86cpuid.obj ..\shared\x86iTaskbarList3.obj ..\shared\x86iFileDialog.obj /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /pdbtype:sept /opt:nowin98
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
TargetPath=\Documents and Settings\Administrator\My Documents\Projects\DebugW\MD5チェックツール.exe
SOURCE="$(InputPath)"
PostBuild_Cmds=mt.exe -nologo -outputresource:"$(TargetPath)" -manifest "..\shared\Common-Controls.manifest" "..\shared\DeclareDPIAware.manifest" "..\shared\supportedOS.manifest" "..\shared\requestedExecutionLevel.manifest"
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "MD5チェックツール - Win32 Release"
# Name "MD5チェックツール - Win32 Debug"
# Name "MD5チェックツール - Win32 ReleaseW"
# Name "MD5チェックツール - Win32 DebugW"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AboutDlg.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\shared\BinaryView.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=..\shared\charlib.c
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=..\shared\charlib_base64.c
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=..\shared\charlib_sysdll.c
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=..\shared\charlib_url.c
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=..\shared\clipboard.c
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=..\shared\cpuid.c
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=..\shared\crc32.c
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=..\shared\crc32c.c
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\DebugDlg.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\shared\ecoqos.c
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\FileListDlg.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\shared\getopt.c
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\hashthread.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\shared\iFileDialog.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=..\shared\IniFilePath.c
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=..\shared\iTaskbarList3.cpp
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Main.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\shared\md2.c
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=..\shared\md4.c
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=..\shared\md5.c
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=..\shared\md5_noasm.c
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\OptionDlg.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\OptionDlgTab1.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\OptionDlgTab2.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\OptionDlgTab3.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\OptionDlgTab4.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\shared\os.c
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=..\shared\os_full.c
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=..\shared\sha1.c
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=..\shared\sha1_noasm.c
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=..\shared\sha256.c
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=..\shared\sha256_noasm.c
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=..\shared\sha3.c
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=..\shared\sha3_noasm.c
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=..\shared\sha512.c
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=..\shared\sha512_noasm.c
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\stdafx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\shared\ToolTip.c
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=..\shared\WindowSize.c
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=..\shared\xxhash.c
# SUBTRACT CPP /YX
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AboutDlg.h
# End Source File
# Begin Source File

SOURCE=..\shared\BinaryView.h
# End Source File
# Begin Source File

SOURCE=..\shared\charlib.h
# End Source File
# Begin Source File

SOURCE=..\shared\charlib_base64.h
# End Source File
# Begin Source File

SOURCE=..\shared\charlib_sysdll.h
# End Source File
# Begin Source File

SOURCE=..\shared\charlib_url.h
# End Source File
# Begin Source File

SOURCE=..\shared\clipboard.h
# End Source File
# Begin Source File

SOURCE=..\shared\cpuid.h
# End Source File
# Begin Source File

SOURCE=..\shared\crc32.h
# End Source File
# Begin Source File

SOURCE=..\shared\crc32c.h
# End Source File
# Begin Source File

SOURCE=.\DebugDlg.h
# End Source File
# Begin Source File

SOURCE=..\shared\ecoqos.h
# End Source File
# Begin Source File

SOURCE=.\FileListDlg.h
# End Source File
# Begin Source File

SOURCE=..\shared\getopt.h
# End Source File
# Begin Source File

SOURCE=.\hashthread.h
# End Source File
# Begin Source File

SOURCE=..\shared\iFileDialog.h
# End Source File
# Begin Source File

SOURCE=..\shared\IniFilePath.h
# End Source File
# Begin Source File

SOURCE=..\shared\iTaskbarList3.h
# End Source File
# Begin Source File

SOURCE=.\Main.h
# End Source File
# Begin Source File

SOURCE=..\shared\md2.h
# End Source File
# Begin Source File

SOURCE=..\shared\md32_ctx.h
# End Source File
# Begin Source File

SOURCE=..\shared\md4.h
# End Source File
# Begin Source File

SOURCE=..\shared\md5.h
# End Source File
# Begin Source File

SOURCE=..\shared\md5_noasm.h
# End Source File
# Begin Source File

SOURCE=..\shared\OptionDlg.h
# End Source File
# Begin Source File

SOURCE=.\OptionDlgTab1.h
# End Source File
# Begin Source File

SOURCE=.\OptionDlgTab2.h
# End Source File
# Begin Source File

SOURCE=.\OptionDlgTab3.h
# End Source File
# Begin Source File

SOURCE=.\OptionDlgTab4.h
# End Source File
# Begin Source File

SOURCE=..\shared\os.h
# End Source File
# Begin Source File

SOURCE=..\shared\os_full.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=..\shared\sha1.h
# End Source File
# Begin Source File

SOURCE=..\shared\sha1_noasm.h
# End Source File
# Begin Source File

SOURCE=..\shared\sha256.h
# End Source File
# Begin Source File

SOURCE=..\shared\sha256_noasm.h
# End Source File
# Begin Source File

SOURCE=..\shared\sha3.h
# End Source File
# Begin Source File

SOURCE=..\shared\sha3_ctx.h
# End Source File
# Begin Source File

SOURCE=..\shared\sha3_noasm.h
# End Source File
# Begin Source File

SOURCE=..\shared\sha512.h
# End Source File
# Begin Source File

SOURCE=..\shared\sha512_noasm.h
# End Source File
# Begin Source File

SOURCE=..\shared\sha_ctx.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# Begin Source File

SOURCE=..\shared\targetver.h
# End Source File
# Begin Source File

SOURCE=..\shared\ToolTip.h
# End Source File
# Begin Source File

SOURCE=..\shared\WindowSize.h
# End Source File
# Begin Source File

SOURCE="..\shared\xxhash-0.8.3.h"
# End Source File
# Begin Source File

SOURCE="..\shared\xxhash-ctx.h"
# End Source File
# Begin Source File

SOURCE=..\shared\xxhash.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\FILELIST_ICON1.ico
# End Source File
# Begin Source File

SOURCE=.\FILELIST_ICON2.ico
# End Source File
# Begin Source File

SOURCE=.\FILELIST_ICON3.ico
# End Source File
# Begin Source File

SOURCE=.\FILELIST_ICON4.ico
# End Source File
# Begin Source File

SOURCE=.\Main.ico
# End Source File
# Begin Source File

SOURCE=.\Main.rc
# End Source File
# Begin Source File

SOURCE=.\STATUS_ICON0.ico
# End Source File
# Begin Source File

SOURCE=.\STATUS_ICON1.ico
# End Source File
# Begin Source File

SOURCE=.\STATUS_ICON2.ico
# End Source File
# Begin Source File

SOURCE=.\STATUS_ICON3.ico
# End Source File
# Begin Source File

SOURCE=.\STATUS_ICON4.ico
# End Source File
# Begin Source File

SOURCE=.\STATUS_ICON5.ico
# End Source File
# Begin Source File

SOURCE=.\STATUS_ICON6.ico
# End Source File
# End Group
# End Target
# End Project
