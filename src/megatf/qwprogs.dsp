# Microsoft Developer Studio Project File - Name="qwprogs" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=qwprogs - Win32 TG Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "qwprogs.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "qwprogs.mak" CFG="qwprogs - Win32 TG Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "qwprogs - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "qwprogs - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "qwprogs - Win32 TG Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "qwprogs - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QWPROGS_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /Ob2 /I "include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QWPROGS_EXPORTS" /YX /FD /QaxK /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /map /machine:I386

!ELSEIF  "$(CFG)" == "qwprogs - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QWPROGS_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QWPROGS_EXPORTS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"D:\games\qwtf\fortress\qwprogs.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "qwprogs - Win32 TG Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "qwprogs___Win32_TG_Debug"
# PROP BASE Intermediate_Dir "qwprogs___Win32_TG_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "TG_Debug"
# PROP Intermediate_Dir "TG_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QWPROGS_EXPORTS" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QWPROGS_EXPORTS" /D "TEST" /FR /YX /FD /GZ /c
# SUBTRACT CPP /WX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"D:\games\qwtf\fortress\qwprogs.dll" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"D:\games\qwtf\fortress\bots_dbg.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "qwprogs - Win32 Release"
# Name "qwprogs - Win32 Debug"
# Name "qwprogs - Win32 TG Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Bot"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\src\bot\ai-misc.c"
# End Source File
# Begin Source File

SOURCE=.\src\bot\ai.c
# End Source File
# Begin Source File

SOURCE=.\src\bot\fighting.c
# End Source File
# Begin Source File

SOURCE=.\src\bot\move.c
# End Source File
# Begin Source File

SOURCE=.\src\bot\qw.c
# End Source File
# Begin Source File

SOURCE=.\src\bot\ui.c
# End Source File
# Begin Source File

SOURCE=.\src\bot\waypoint.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\actions.c
# End Source File
# Begin Source File

SOURCE=.\src\admin.c
# End Source File
# Begin Source File

SOURCE=.\src\admin_cmd.c
# End Source File
# Begin Source File

SOURCE=.\src\bg_lib.c
# End Source File
# Begin Source File

SOURCE=.\src\buttons.c
# End Source File
# Begin Source File

SOURCE=.\src\clan.c
# End Source File
# Begin Source File

SOURCE=.\src\client.c
# End Source File
# Begin Source File

SOURCE=.\src\combat.c
# End Source File
# Begin Source File

SOURCE=.\src\coop.c
# End Source File
# Begin Source File

SOURCE=.\src\crc32.c
# End Source File
# Begin Source File

SOURCE=.\src\ctf.c
# End Source File
# Begin Source File

SOURCE=.\src\debug.c
# End Source File
# Begin Source File

SOURCE=.\src\demoman.c
# End Source File
# Begin Source File

SOURCE=.\src\doors.c
# End Source File
# Begin Source File

SOURCE=.\src\engineer.c
# End Source File
# Begin Source File

SOURCE=.\src\flare.c
# End Source File
# Begin Source File

SOURCE=.\src\g_cmd.c
# End Source File
# Begin Source File

SOURCE=.\src\g_main.c
# End Source File
# Begin Source File

SOURCE=.\src\g_mem.c
# End Source File
# Begin Source File

SOURCE=.\src\g_mod_command.c
# End Source File
# Begin Source File

SOURCE=.\src\g_spawn.c
# End Source File
# Begin Source File

SOURCE=.\src\g_syscalls.c
# End Source File
# Begin Source File

SOURCE=.\src\g_tf_utils.c
# End Source File
# Begin Source File

SOURCE=.\src\g_utils.c
# End Source File
# Begin Source File

SOURCE=.\src\game.def
# End Source File
# Begin Source File

SOURCE=.\src\hook.c
# End Source File
# Begin Source File

SOURCE=.\src\items.c
# End Source File
# Begin Source File

SOURCE=.\src\mathlib.c
# End Source File
# Begin Source File

SOURCE=.\src\menu.c
# End Source File
# Begin Source File

SOURCE=.\src\misc.c
# End Source File
# Begin Source File

SOURCE=.\src\plats.c
# End Source File
# Begin Source File

SOURCE=.\src\player.c
# End Source File
# Begin Source File

SOURCE=.\src\pyro.c
# End Source File
# Begin Source File

SOURCE=.\src\q_shared.c
# End Source File
# Begin Source File

SOURCE=.\src\qmd5.c
# End Source File
# Begin Source File

SOURCE=.\src\scout.c
# End Source File
# Begin Source File

SOURCE=.\src\sentry.c
# End Source File
# Begin Source File

SOURCE=.\src\server.c
# End Source File
# Begin Source File

SOURCE=.\src\sniper.c
# End Source File
# Begin Source File

SOURCE=.\src\spectate.c
# End Source File
# Begin Source File

SOURCE=.\src\spy.c
# End Source File
# Begin Source File

SOURCE=.\src\status.c
# End Source File
# Begin Source File

SOURCE=.\src\subs.c
# End Source File
# Begin Source File

SOURCE=.\src\tfort.c
# End Source File
# Begin Source File

SOURCE=.\src\tforthlp.c
# End Source File
# Begin Source File

SOURCE=.\src\tfortmap.c
# End Source File
# Begin Source File

SOURCE=.\src\tforttm.c
# End Source File
# Begin Source File

SOURCE=.\src\tg.c
# End Source File
# Begin Source File

SOURCE=.\src\triggers.c
# End Source File
# Begin Source File

SOURCE=.\src\tsoldier.c
# End Source File
# Begin Source File

SOURCE=.\src\version.c

!IF  "$(CFG)" == "qwprogs - Win32 Release"

# ADD CPP /Ze /W3 /Gi-
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "qwprogs - Win32 Debug"

!ELSEIF  "$(CFG)" == "qwprogs - Win32 TG Debug"

# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vote.c
# End Source File
# Begin Source File

SOURCE=.\src\weapons.c
# End Source File
# Begin Source File

SOURCE=.\src\world.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\include\bg_lib.h
# End Source File
# Begin Source File

SOURCE=.\include\bot_waypoint.h
# End Source File
# Begin Source File

SOURCE=.\include\g_consts.h
# End Source File
# Begin Source File

SOURCE=.\include\g_local.h
# End Source File
# Begin Source File

SOURCE=.\include\g_public.h
# End Source File
# Begin Source File

SOURCE=.\include\g_syscalls.h
# End Source File
# Begin Source File

SOURCE=.\include\g_tf_consts.h
# End Source File
# Begin Source File

SOURCE=.\include\mathlib.h
# End Source File
# Begin Source File

SOURCE=.\include\player.h
# End Source File
# Begin Source File

SOURCE=.\include\progdefs.h
# End Source File
# Begin Source File

SOURCE=.\include\progs.h
# End Source File
# Begin Source File

SOURCE=.\include\q_shared.h
# End Source File
# Begin Source File

SOURCE=.\include\qmd5.h
# End Source File
# Begin Source File

SOURCE=.\include\spy.h
# End Source File
# Begin Source File

SOURCE=.\include\tfort.h
# End Source File
# Begin Source File

SOURCE=.\include\tg.h

!IF  "$(CFG)" == "qwprogs - Win32 Release"

!ELSEIF  "$(CFG)" == "qwprogs - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "qwprogs - Win32 TG Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
