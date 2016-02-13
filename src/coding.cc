#include "coding.h"

// The encoding is:
// 0-255: Standard byte values.
// 256:   End-of-file.

Intervals Coding::base_intervals() const {
  return Intervals(size());
}

bool Coding::encode(byte c, int* code) const {
  *code = c;
  return true;
}

bool Coding::decode(int code, byte* c) const {
  if (code < 0) return false;
  *c = code;
  return true;
}

int Coding::size() const {
  return 257;
}

int Coding::eof() const {
  return 256;
}
