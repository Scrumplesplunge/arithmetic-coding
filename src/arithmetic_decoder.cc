#include "arithmetic_decoder.h"

#include "intervals.h"
#include "util.h"

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
  // Find the least significant bit above (and including) which the lower and
  // upper bounds match.
  bound same_above = next_pow2(lower_ ^ upper_);
  if (same_above < Intervals::ONE) {
    bound mul = Intervals::ONE / same_above;
    const bound mask = Intervals::ONE - 1;

    lsb_ *= mul;
    if (lsb_ > Intervals::ONE)
      throw logic_error("Next bit position overflows.");
    value_ = (value_ * mul) & mask;
    lower_ = (lower_ * mul) & mask;
    upper_ = (upper_ * mul) & mask;
    if (upper_ == 0) upper_ = Intervals::ONE;
  }
}

// Fetch as many bits as possible and put them in value_.
void ArithmeticDecoder::read_bits() {
  bit x;
  while (lsb_ > 1 && in_.get(&x)) {
    lsb_ /= 2;
    value_ |= x * lsb_;
  }
}
