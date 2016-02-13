#pragma once

#include "bit_input.h"
#include "intervals.h"
#include "types.h"

#include <iostream>

class ArithmeticDecoder {
 public:
  ArithmeticDecoder(std::istream& in);

  // Read a code from the input and return it.
  int Decode(const Intervals& intervals);

  void DebugPrint() const;

 private:
  // Discard all common leading bits by repeatedly expanding the ranges.
  void Renormalize();

  // Discard the leading bit, shift up by one, and expand the ranges.
  void expand();

  // Fetch as many bits as possible and put them in value_.
  void read_bits();

  BitInput in_;

  // The position in value_ of the least significant bit that has been set.
  bound lsb_ = Intervals::ONE;
  bound value_ = 0;

  bound lower_ = 0, upper_ = Intervals::ONE;
};
