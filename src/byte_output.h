#pragma once

#include "types.h"

#include <iostream>
#include <memory>

// Buffered byte output.
class ByteOutput {
 public:
  static const int BUFFER_SIZE;

  ByteOutput(std::ostream& out);
  ~ByteOutput();

  // Output a byte.
  void put(byte x);

  // Flush the currently buffered bytes.
  void flush();

 private:
  std::ostream& out_;

  // (byte, bit) index of the next bit to be read.
  int index_ = 0;
  std::unique_ptr<byte[]> buffer_;
};
