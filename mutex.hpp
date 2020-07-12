class mutex {
  HANDLE handle;
  public:
    bool take() {
      handle = CreateMutex(NULL, true, MUTEX_NAME);
      return GetLastError() != ERROR_ALREADY_EXISTS;
    }
    mutex() {
      handle = NULL;
    }
    ~mutex() {
      if (handle == NULL) return;
      ReleaseMutex(handle);
      CloseHandle(handle);
    }
};
