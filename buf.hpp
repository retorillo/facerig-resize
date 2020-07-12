#pragma once

#include <stdexcept>
using namespace std;

template <class T>
class buf {
  T* _m;
  int _l;
  inline int L(int l) noexcept {
    return sizeof(T) * l;
  }
public:
  buf(int l) {
    _m = reinterpret_cast<T*>(malloc(L(l)));
    if (!_m)
      throw runtime_error("Memory insufficient, allocation failed");
  }
  ~buf() {
    dispose();
  }
  T* get_ptr() noexcept {
    return _m;
  }
  void dispose() noexcept {
    if (!_m) return;
    free(_m);
    _m = 0;
    _l = 0;
  }
  void expand(int l) {
    if (l <= _l) return;
    auto M = reinterpret_cast<T*>(realloc(_m, L(l)));
    if (!M) {
      dispose();
      throw runtime_error("Memory insufficient, reallocation failed");
    }
    _m = M;
  }
};
