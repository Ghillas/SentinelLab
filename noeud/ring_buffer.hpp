#pragma once
#include <array>

template <typename T, size_t N>
class RingBuffer {
  static_assert(N > 0 && (N & (N - 1)) == 0, "N doit être une puissance de 2");
  std::array<T, N> buf_{};
  size_t head_ = 0, tail_ = 0;
public:
  bool push(const T& v) {
    if (full()) return false;              // pas d'écrasement silencieux
    buf_[head_++ & (N - 1)] = v;
    return true;
  }
  bool pop(T& out) {
    if (empty()) return false;
    out = buf_[tail_++ & (N - 1)];
    return true;
  }
  bool   empty() const { return head_ == tail_; }
  bool   full()  const { return head_ - tail_ == N; }
  size_t size()  const { return head_ - tail_; }
};