#include "arithmetic_decoder.h"

#include <stdio.h>

using namespace std;

ArithmeticDecoder::ArithmeticDecoder(istream& in)
    : in_(in) {}

// Write the given code to the output, using the given interval table.
int ArithmeticDecoder::Decode(const Intervals& intervals) {
  read_bits();

  int code;
  if (!intervals.CodeAt(lower_, upper_, value_, value_ + 1, &code))
    throw runtime_error("No code match found but precision limit reached.");

  // Retrieve the bounds q3 the given code.
  bound code_lower = intervals.lower(code);
  bound code_upper = intervals.upper(code);

  // Calculate this range inside the current range.
  bound new_lower = Intervals::within(lower_, upper_, code_lower);
  bound new_upper = Intervals::within(lower_, upper_, code_upper);

  lower_ = new_lower;
  upper_ = new_upper;

  Renormalize();

  return code;
}

void ArithmeticDecoder::DebugPrint() const {
  fprintf(stderr, "lower = 0x%08lx = %2.10lf, upper = 0x%08lx = %2.10lf\n",
          lower_, static_cast<double>(lower_) / Intervals::ONE,
          upper_, static_cast<double>(upper_) / Intervals::ONE);
}

// Output any common leading bits, and expand the ranges.
void ArithmeticDecoder::Renormalize() {
  const bound HALF = Intervals::ONE / 2;
  bit lower_msb, upper_msb;
 
  lower_msb = lower_ / HALF;
  upper_msb = upper_ / HALF;
  while (lower_msb == upper_msb) {
    // The MSB matches. Expand the ranges.
    expand();
 
    // Recompute the MSBs.
    lower_msb = lower_ / HALF;
    upper_msb = upper_ / HALF;
  }
}

// Discard the leading bit, shift up by one, and expand the ranges.
void ArithmeticDecoder::expand() {
  lsb_ *= 2;
  if (lsb_ > Intervals::ONE)
    throw logic_error("Next bit position overflows.");

  const int mask = Intervals::ONE - 1;
  value_ = (value_ << 1) & mask;
  lower_ = (lower_ << 1) & mask;
  upper_ = (upper_ << 1) & mask;
}

// Fetch as many bits as possible and put them in value_.
void ArithmeticDecoder::read_bits() {
  bit x;
  while (lsb_ > 1 && in_.get(&x)) {
    lsb_ /= 2;
    value_ |= x * lsb_;
  }
}
