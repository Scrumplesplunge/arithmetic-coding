#pragma once

#include "intervals.h"
#include "input.h"
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

  // Fetch a bit and write it to value_.
  void read_bit();

  Input in_;

  // The position in value_ of the least significant bit that has been set.
  bound lsb_ = Intervals::ONE;
  bound value_ = 0;

  bound lower_ = 0, upper_ = Intervals::ONE;
};
