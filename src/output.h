#pragma once

#include "types.h"

#include <iostream>

// Output individual bits to a stream (MSB first).
class Output {
 public:
  Output(std::ostream& out);
  ~Output();

  // Output a single bit.
  Output& operator<<(bit x);

  // Output a single bit.
  void put(bit x);

  // Flush the currently buffered bits, filling the empty spaces with 0's.
  void flush();

 private:
  int bits_buffered_ = 0;
  byte buffered_ = 0;

  std::ostream& out_;
};
