#pragma once

#include "types.h"

#include <iostream>
#include <memory>

// Read individual bits from a stream (LSB first).
class BitInput {
 public:
  static const int BUFFER_SIZE;

  BitInput(std::istream& in);

  // Read a single bit.
  bool get(bit* out);

 private:
  std::istream& in_;
  
  // (byte, bit) index of the next bit to be read.
  int byte_index_ = 0, bit_index_ = 0;

  // Number of bytes currently buffered.
  int bytes_buffered_ = 0;
  std::unique_ptr<byte[]> buffer_;
};
