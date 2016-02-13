#pragma once

#include "types.h"

#include <iostream>

// Output individual bits to a stream (MSB first).
class Input {
 public:
  Input(std::istream& in);

  // Read a single bit.
  bit get();

 private:
  int bits_buffered_ = 0;
  byte buffered_ = 0;

  std::istream& in_;
};
