#pragma once

#define UNICODE
#define _UNICODE

#include <windows.h>
#include <tchar.h>
#include <conio.h>

#ifdef UNICODE
  #define tcout wcout
  #define tcerr wcerr
  #define tcin  wcin
#endif

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

#include "buf.hpp"
#define BUFT buf<TCHAR>

#define MUTEX_NAME TEXT("facerig_resize")
#include "mutex.hpp"

#include "usrdef.h"

#define SPINNER_COUNT 6
#define SLEEP_TIME 500
#define SET_STYLE (WS_VISIBLE | WS_CLIPSIBLINGS | WS_CAPTION | WS_SYSMENU | WS_GROUP | WS_TABSTOP)

enum RESIZE_RESULT {
  SUCCESS = 0,
  FAILURE = -1,
  NOTFOUND = 1,
};

wstring rc(UINT);
void bksp(int);
RESIZE_RESULT resize(int*, HANDLE*);

#define APP_TITLE             rc(1)
#define SPLASH_SCREEN         rc(2)
#define FAILED_GET_PLACEMENT  rc(3)
#define FAILED_GET_DPI        rc(4)
#define FAILED_GET_STYLE      rc(5)
#define FAILED_SET_STYLE      rc(6)
#define FAILED_GET_EXSTYLE    rc(7)
#define FAILED_OVERLAPPED     rc(8)
#define FAILED_SET_WINDOWRECT rc(9)
#define FAILED_SET_PLACEMENT  rc(10)
#define INFO_WAITING          rc(11)
#define INFO_ADJUSTED         rc(12)
#define INFO_PRESSANYKEY      rc(13)

#pragma comment(lib, "user32.lib")
