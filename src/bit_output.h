#pragma once

#include "types.h"

#include <iostream>
#include <memory>

// Output individual bits to a stream (LSB first).
class BitOutput {
 public:
  static const int BUFFER_SIZE;

  BitOutput(std::ostream& out);
  ~BitOutput();

  // Output a single bit.
  void put(bit x);

  // Flush the currently buffered bits, filling the empty spaces in the last
  // byte with 0's.
  void flush();

 private:
  std::ostream& out_;

  // (byte, bit) index of the next bit to be read.
  int byte_index_ = 0, bit_index_ = 0;
  std::unique_ptr<byte[]> buffer_;
};
