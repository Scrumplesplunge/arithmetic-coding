#pragma once

#include "intervals.h"
#include "output.h"
#include "types.h"

#include <iostream>

class ArithmeticEncoder {
 public:
  ArithmeticEncoder(std::ostream& out);
  ~ArithmeticEncoder();

  // Write the given code to the output, using the given interval table.
  void Encode(const Intervals& intervals, int code);

  void DebugPrint() const;

 private:
  // Output any common leading bits, and expand the ranges.
  void Renormalize();

  // Discard the leading bit, shift up by one, and expand the ranges.
  void expand();

  // Ensure that the output falls within the correct final interval.
  void finish();

  Output out_;
  bound lower_ = 0, upper_ = Intervals::ONE;
};
