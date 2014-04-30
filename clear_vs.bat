@echo off
echo ----------------------------------------------------------------
echo *                      Visual C++                              *
echo *          清理Visual Studio工程中不需要的文件                 *
echo *          保存成.bat文件放置在工程目录中                      *
echo *          执行前先关闭打开该工程的Visual Studio               *
echo *          会清理以下格式文件：                                *
echo *          *.aps *.idb *.ncp *.obj *.pch *.sbr *.tmp           *
echo *          *.pdb *.bsc *.ilk *.res *.ncb *.opt *.suo           *
echo *          *.manifest *.dep *.sdf *.tlog *.log *.ipch          *
echo *          *.lastbuildstate  *.user *.suo                      *
echo ----------------------------------------------------------------
echo 按任意键开始清理
pause

del /F /Q /S *.aps *.idb *.ncp *.obj *.pch *.sbr *.tmp *.pdb *.bsc *.ilk *.res 
del /F /Q /S *.ncb *.opt *.suo *.manifest *.dep *.sdf *.tlog *.log *.ipch *.lastbuildstate
del /F /Q /S *.user *.suo

echo 文件清理完毕！！！
pause