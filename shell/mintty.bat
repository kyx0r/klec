@echo off
set "WD=%__CD__%"
set "LOGINSHELL=usr\bin\bash"
start "%WD%" "%WD%%LOGINSHELL%" -l -c "/mintty.exe"
