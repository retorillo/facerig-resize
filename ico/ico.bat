@echo off
if not exist cache mkdir cache
setlocal enabledelayedexpansion
for %%s in (256 128 64 48 32 16 8) do (
  set dest=cache/%%s.ico
  magick convert %%s.png !dest!
  set files=!files! !dest!
)
magick !files! app.ico
rmdir /S /Q cache
magick identify app.ico 
endlocal
