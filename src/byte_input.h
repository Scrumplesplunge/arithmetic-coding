#pragma once

#include "types.h"

#include <iostream>
#include <memory>

// Buffer input and read bytes from it.
class ByteInput {
 public:
  static const int BUFFER_SIZE;

  ByteInput(std::istream& in);

  // Read a single bit.
  bool get(byte* out);

 private:
  std::istream& in_;
  
  // Index of the next byte to be read.
  int index_ = 0;

  // Number of bytes currently buffered.
  int bytes_buffered_ = 0;
  std::unique_ptr<byte[]> buffer_;
};
