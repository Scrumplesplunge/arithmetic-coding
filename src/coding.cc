#include "coding.h"

// The encoding is:
// 0-255: Standard byte values.
// 256:   End-of-file.

Intervals Coding::base_intervals() const {
  return Intervals(size());
}

bool Coding::encode(char c, int* code) const {
  *code = static_cast<byte>(c);
  return true;
}

bool Coding::decode(int code, char* c) const {
  if (code < 0) return false;
  *c = static_cast<char>(code);
  return true;
}

int Coding::size() const {
  return 257;
}

int Coding::eof() const {
  return 256;
}
