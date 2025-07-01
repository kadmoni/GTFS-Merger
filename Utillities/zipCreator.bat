@echo off
set PATH=%PATH%;C:\Program Files\7-Zip\
7z a thisnew.zip .\Combined\*
move .\*.zip .\Combined_Zip