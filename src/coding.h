#pragma once

#include "intervals.h"
#include "types.h"

// Represents a bidirectional conversion between characters and code points.
// The implementation simply wraps up an arbitrary char, but this could easily
// be replaced with a situation-specific encoding (perhaps with a restricted
// character set).
class Coding {
 public:
  Intervals base_intervals() const;

  // Encode a character as a codepoint. Returns true if successful.
  bool encode(byte c, int* code) const;

  // Decode a character from a codepoint. Returns true if successful.
  bool decode(int code, byte* c) const;

  // Returns the number of codepoints in the encoding.
  int size() const;

  // Returns the end-of-file codepoint.
  int eof() const;
};
