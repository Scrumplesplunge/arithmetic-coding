#include "arithmetic_encoder.h"

#include <stdio.h>

using namespace std;

ArithmeticEncoder::ArithmeticEncoder(ostream& out)
    : out_(out) {}

ArithmeticEncoder::~ArithmeticEncoder() {
  finish();
}

// Write the given code to the output, using the given interval table.
void ArithmeticEncoder::Encode(const Intervals& intervals, int code) {
  // Retrieve the bounds for the given code.
  bound code_lower = intervals.lower(code);
  bound code_upper = intervals.upper(code);

  // Calculate this range inside the current range.
  bound new_lower = Intervals::within(lower_, upper_, code_lower);
  bound new_upper = Intervals::within(lower_, upper_, code_upper);

  lower_ = new_lower;
  upper_ = new_upper;

  Renormalize();
}

void ArithmeticEncoder::DebugPrint() const {
  fprintf(stderr, "lower = 0x%08lx = %2.10lf, upper = 0x%08lx = %2.10lf\n",
          lower_, static_cast<double>(lower_) / Intervals::ONE,
          upper_, static_cast<double>(upper_) / Intervals::ONE);
}

// Output any common leading bits, and expand the ranges.
void ArithmeticEncoder::Renormalize() {
  const bound HALF = Intervals::ONE / 2;
  bit lower_msb, upper_msb;
 
  lower_msb = lower_ / HALF;
  upper_msb = upper_ / HALF;
  while (lower_msb == upper_msb) {
    // The MSB matches. Output it and expand the ranges.
    out_.put(lower_msb);
    expand();
 
    // Recompute the MSBs.
    lower_msb = lower_ / HALF;
    upper_msb = upper_ / HALF;
  }
}

// Discard the leading bit, shift up by one, and expand the ranges.
void ArithmeticEncoder::expand() {
  lower_ = ((2 * lower_) & (Intervals::ONE - 1));
  upper_ = ((2 * upper_) & (Intervals::ONE - 1));
  if (upper_ == 0) upper_ = Intervals::ONE;
}

// Ensure that the output falls within the correct final interval.
void ArithmeticEncoder::finish() {
  const bound HALF = Intervals::ONE / 2;
  // We know the initial MSBs differ, so we only consider subsequent digits.
  int length = 0;
  while (true) {
    expand();
    length++;
    bit lower_msb = lower_ / HALF,
        upper_msb = upper_ / HALF;
    if (lower_msb == 0) {
      // 0111...111 is definitely in the range.
      out_.put(0);
      for (int i = 0; i < length; i++) out_.put(1);
      return;
    } else if (upper_msb == 1) {
      // 1000...000 is definitely in the range.
      out_.put(1);
      for (int i = 0; i < length; i++) out_.put(0);
      return;
    }
  }
}
