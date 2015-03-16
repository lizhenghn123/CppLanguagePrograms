@echo off
echo ----------------------------------------------------------------
echo *                      Visual C++                              *
echo *          清理Visual Studio工程中不需要的文件                 *
echo *          保存成.bat文件放置在工程目录中                      *
echo *          会清理以下格式文件：                                *
echo *          *.aps *.idb *.ncp *.obj *.pch *.sbr *.tmp           *
echo *          *.pdb *.bsc *.ilk *.res *.ncb *.opt *.suo           *
echo *          *.manifest *.dep *.sdf *.tlog *.log *.ipch          *
echo *          *.lastbuildstate  *.user *.suo                      *
echo ----------------------------------------------------------------
echo 按任意键开始清理
pause

del /F /Q /S *.aps *.idb *.ncp *.obj *.pch *.sbr *.tmp *.pdb *.bsc *.ilk *.res *rc *cache
del /F /Q /S *.ncb *.opt *.suo *.manifest *.dep *.sdf *.tlog *.log *.ipch *.lastbuildstate
del /F /Q /S *.user *.suo

for /r %%a in (.) do (
set list=%%a
setlocal enabledelayedexpansion
if "!list:~-8,6!"=="-Debug" rd /q /s "!list:~0,-2!"
if "!list:~-10,8!"=="-Release" rd /q /s "!list:~0,-2!"
endlocal
)

rd /q /s "build/!bin"
rd /q /s "build/!tmp"
rd /q /s "build/debug"
rd /q /s "build/release"
rd /q /s ipch

echo 文件清理完毕！！！
pause