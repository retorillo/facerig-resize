#include "app.h"

wstring rc(UINT id) {
  WCHAR* ptr;
  int len = LoadString(GetModuleHandle(NULL), id, reinterpret_cast<LPWSTR>(&ptr), 0);
  WCHAR* str = new WCHAR[len + 1];
  memcpy(str, ptr, sizeof(WCHAR) * len);
  str[len] = 0;
  wstring r(str);
  delete[] str;
  return r;
}

RESIZE_RESULT resize(int* spin, HWND* hwnd) {
  BUFT b(MAX_PATH);
  RESIZE_RESULT e = NOTFOUND;
  void* bag[] = { &b, &e, spin, hwnd};
  EnumWindows([](HWND h, LPARAM l) -> BOOL {
    void** bag = reinterpret_cast<void**>(l);
    BUFT* b = reinterpret_cast<BUFT*>(bag[0]);
    RESIZE_RESULT* e = reinterpret_cast<RESIZE_RESULT*>(bag[1]);
    try {
      int L;
      b->expand(L = (GetWindowTextLength(h) + 1));
      GetWindowText(h, b->get_ptr(), L);
      if (_tcscmp(b->get_ptr(), TITLE))
        return TRUE;
     
      bksp(*reinterpret_cast<int*>(bag[2])); 
      *reinterpret_cast<int*>(bag[2]) = 0;
      *reinterpret_cast<HWND*>(bag[3]) = h;

      tcout << endl;
      tcout << TEXT("HANDLE  : 0x") << hex << h << dec << endl;    

      WINDOWPLACEMENT wpl;
      wpl.length = sizeof(WINDOWPLACEMENT);
      if (!GetWindowPlacement(h, &wpl))
        throw FAILED_GET_PLACEMENT;
      
      RECT r, nr;
      DWORD stl, estl;
      UINT dpi;
      if (!(dpi = GetDpiForWindow(h)))
        throw FAILED_GET_DPI;
      tcout << TEXT("DPI     : ") << dpi << endl;    
      
      r = nr = wpl.rcNormalPosition;
      nr.right = nr.left + WIDTH;
      nr.bottom = nr.top + HEIGHT;
      if (!(stl = GetWindowLong(h, GWL_STYLE)))
        throw FAILED_GET_STYLE;
      tcout << TEXT("STYLE   : 0x") << hex << stl << dec;

      if (stl != SET_STYLE) {
        if (!SetWindowLong(h, GWL_STYLE, SET_STYLE))
          throw FAILED_SET_STYLE;
        tcout << TEXT(" => 0x") << hex << SET_STYLE << dec;
      }
      tcout << endl;

      if (!(estl = GetWindowLong(h, GWL_EXSTYLE)))
        throw FAILED_GET_EXSTYLE;
      tcout << TEXT("EXSTYLE : 0x") << hex << estl << dec << endl;
      if (!AdjustWindowRectExForDpi(&nr, SET_STYLE, !!GetMenu(h), estl, dpi))
        throw FAILED_SET_WINDOWRECT;

      tcout << TEXT("WIDTH   : ") << r.right - r.left << TEXT(" => ") << nr.right - nr.left << endl;    
      tcout << TEXT("HEIGHT  : ") << r.bottom - r.top << TEXT(" => ") << nr.bottom - nr.top << endl; 
      wpl.rcNormalPosition = nr;
      wpl.showCmd = SW_SHOWNORMAL;
      if (!SetWindowPlacement(h, &wpl))
        throw FAILED_SET_PLACEMENT;

      *e = SUCCESS; 
      return FALSE;
    }
    catch(const wstring& err) {
      bksp(*reinterpret_cast<int*>(bag[2])); 
      *reinterpret_cast<int*>(bag[2]) = 0;
      DWORD le = GetLastError();
      tcerr << endl << err << endl;
      tcerr << TEXT("LASTERROR : ") << hex << le << endl;    
      *e = FAILURE;
      return FALSE;
    }
  }, reinterpret_cast<LPARAM>(bag));
  return e; 
}

void bksp(int N) {
  for (int c = 0; c < N; c++)
    tcout << TEXT('\b');
  for (int c = 0; c < N; c++)
    tcout << TEXT(' ');
  for (int c = 0; c < N; c++)
    tcout << TEXT('\b');
}

int _tmain(int argc, TCHAR* argv[]) {
  wstring title = APP_TITLE.c_str();
  mutex mu;
  if (!mu.take()) {
    BUFT b(MAX_PATH);
    void* bag[] = { &b, &title };
    EnumWindows([](HWND h, LPARAM l) -> BOOL {
      void** bag = reinterpret_cast<void**>(l);
      BUFT* b = reinterpret_cast<BUFT*>(bag[0]);
      wstring* title = reinterpret_cast<wstring*>(bag[1]);
      int L;
      b->expand(L = (GetWindowTextLength(h) + 1));
      GetWindowText(h, b->get_ptr(), L);
      if (_tcscmp(b->get_ptr(), title->c_str()))
        return TRUE;
      SetForegroundWindow(h);
      FLASHWINFO fi;
      memset(&fi, 0, sizeof(FLASHWINFO));
      fi.cbSize = sizeof(FLASHWINFO);
      fi.hwnd = h;
      fi.dwFlags = FLASHW_ALL;
      fi.uCount = 3;
      FlashWindowEx(&fi);
      return FALSE;
    }, reinterpret_cast<LPARAM>(&bag));
    return 1;
  }

  SetProcessDPIAware();
  SetConsoleTitle(title.c_str());
  CONSOLE_SCREEN_BUFFER_INFOEX cx;
  memset(&cx, 0, sizeof(CONSOLE_SCREEN_BUFFER_INFOEX));
  cx.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
  GetConsoleScreenBufferInfoEx(GetStdHandle(STD_OUTPUT_HANDLE), &cx);
  cx.dwSize = {78, 20};
  SetConsoleScreenBufferInfoEx(GetStdHandle(STD_OUTPUT_HANDLE), &cx);
  setlocale(LC_CTYPE, "");
  RESIZE_RESULT r = NOTFOUND;
  tcout << SPLASH_SCREEN;

  HWND hwnd;
  bool restart = true;
  int spin = 0;
  int failure = 0;
  while (r != FAILURE) {
    if (restart) {
      Sleep(SLEEP_TIME);
      if (spin > 0) bksp(spin);
      spin = 0;
      tcout << endl << INFO_WAITING;
      r = NOTFOUND;
      hwnd = NULL;
      restart = false;
    }
    if (spin > 0 && spin % SPINNER_COUNT == 0) {
      bksp(SPINNER_COUNT);
      spin = 0;
    }
    else {
      tcout << TEXT(".");
      spin++; 
    }
    if (r == NOTFOUND) {
      r = resize(&spin, &hwnd);
      switch(r) {
        case FAILURE:
          tcout << INFO_PRESSANYKEY;
          while (!_kbhit()) continue;
          return 1;
        case SUCCESS:
          tcout << INFO_ADJUSTED;
          break;
      }
    }
    else if(!IsWindow(hwnd)) {
      restart = true;
    }
    Sleep(SLEEP_TIME);
  }
}
