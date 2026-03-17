# Microsoft Developer Studio Project File - Name="ipl98library" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ipl98library - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ipl98library.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ipl98library.mak" CFG="ipl98library - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ipl98library - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ipl98library - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ipl98library - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "COMPILING_IPL98_LIB" /YX /FD /Zm500 /c
# ADD BASE RSC /l 0x406 /d "NDEBUG"
# ADD RSC /l 0x406 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../../lib/ipl98_visualc.lib"

!ELSEIF  "$(CFG)" == "ipl98library - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "COMPILING_IPL98_LIB" /YX /FD /GZ /Zm500 /c
# ADD BASE RSC /l 0x406 /d "_DEBUG"
# ADD RSC /l 0x406 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../../lib/ipl98_visualc_d.lib"

!ENDIF 

# Begin Target

# Name "ipl98library - Win32 Release"
# Name "ipl98library - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\byte_image.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\complex_image.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\coordinate_transform.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\corresponding_3d2d_points.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\equation_solver.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\feature_extraction.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\float_image.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\graph_plot2d.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\graphics.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\hough_transform\hough_base.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\hough_transform\hough_probrht_line.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\hough_transform\hough_rht_circle.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\hough_transform\hough_rht_line.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\hough_transform\hough_sht_line.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\hough_transform\hough_space2d.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\image.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\int_image.cpp
# End Source File
# Begin Source File

SOURCE=.\ipl98_explicit_template_inst.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\ipl98_general_functions.cpp
# End Source File
# Begin Source File

SOURCE=.\ipl98_kernel_c.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\local_operation.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\mask_operation.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\morphology.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\palette.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\perspective.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\pixelgroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\pixelgroups.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\segmentate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\hough_transform\sparse_hough_space2d.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\hough_transform\sparse_hough_space3d.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\spectral.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\statistic.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\std_image.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\text.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\time\time_date.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\topography.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\arrays\array2d.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\arrays\array2d_const_it.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\arrays\array2d_it.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\arrays\arrayos2d.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\arrays\arrayos2d_const_it.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\arrays\arrayos2d_it.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\byte_image.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\complex_image.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\coordinate_transform.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\corresponding_3d2d_points.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\equation_solver.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\error.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\feature_extraction.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\float_image.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\generic_image.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\graph_plot2d.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\graphics.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\hough_transform\hough_base.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\hough_transform\hough_probrht_line.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\hough_transform\hough_rht_circle.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\hough_transform\hough_rht_line.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\hough_transform\hough_sht_line.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\hough_transform\hough_space2d.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\image.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\image_column_it.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\image_column_it_1bp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\image_column_it_24bp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\image_const_column_it.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\image_const_column_it_1bp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\image_const_column_it_24bp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\image_const_row_it.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\image_const_row_it_1bp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\image_const_row_it_24bp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\image_row_it.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\image_row_it_1bp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\image_row_it_24bp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\int_image.h
# End Source File
# Begin Source File

SOURCE=.\ipl98_explicit_template_inst.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\ipl98_general_functions.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\ipl98_setup.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\ipl98_types.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\geometry\line2d.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\geometry\line3d.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\geometry\line_segment2d.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\local_operation.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\mask_operation.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\morphology.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\palette.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\hough_transform\parameter2d.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\perspective.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\pixelgroup.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\pixelgroup_const_it.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\pixelgroup_it.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\pixelgroups.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\geometry\plane.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\points\point2d.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\points\point3d.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\positiongroup2d.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\positiongroup2d_const_it.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\positiongroup2d_it.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\segmentate.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\hough_transform\sparse_hough_space2d.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\hough_transform\sparse_hough_space3d.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\spectral.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\statistic.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\std_image.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\hough_transform\str_list.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\text.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\time\time_date.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\algorithms\topography.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\vectors\vector2d.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\ipl98\cpp\vectors\vector3d.h
# End Source File
# End Group
# End Target
# End Project
