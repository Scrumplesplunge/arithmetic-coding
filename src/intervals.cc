#include "intervals.h"

#include <iostream>
#include <stdexcept>
#include <stdio.h>

using namespace std;

const bound Intervals::ONE = 0x0000000080000000ULL;

static int next_pow2(int x) {
  x--;
  x |= x >> 1;
  x |= x >> 2;
  x |= x >> 4;
  x |= x >> 8;
  x |= x >> 16;
  return x + 1;
}

bound Intervals::within(bound min, bound max, bound value) {
  bound size = max - min;
  return min + size * value / Intervals::ONE;
}

// Construct a new instance with intervals for codes [0..num_codes).
Intervals::Intervals(int num_codes)
    : num_codes_(num_codes), frequency_(new count[num_codes]),
      size_(next_pow2(num_codes) - 1),
      cumulative_frequency_(new count[size_]) {
  for (int i = 0; i < num_codes; i++) frequency_[i] = 0;
  for (int i = 0; i < size_; i++) cumulative_frequency_[i] = 0;
  for (int i = 0; i < num_codes; i++) SetFrequency(i, 1);
}

// Fetch the (inclusive) lower bound for the given code.
bound Intervals::lower(int code) const {
  check_bounds(code);
  return CalculateBound(code);
}

// Fetch the (inclusive) upper bound for the given code.
bound Intervals::upper(int code) const {
  check_bounds(code);
  return CalculateBound(code + 1);
}

// Given that the current range is [range_min, range_max], find the code
// containing the interval [value_min, value_max], if there is one.
bool Intervals::CodeAt(bound range_min, bound range_max, bound value_min,
                       bound value_max, int* code) const {
  // Binary search (by the lower bound) for an interval which contains the one
  // given.
  int i = 0, j = num_codes_;
  while (i + 1 < j) {
    int mid = (i + j) / 2;
    if (within(range_min, range_max, lower(mid)) <= value_min) {
      // The lower bound of mid comes before the lower bound of the given
      // interval, so it (as well as possibly larger indices) is a valid
      // candidate.
      i = mid;
    } else {
      // mid comes after the lower bound, so any valid candidates must comes
      // strictly before this one.
      j = mid;
    }
  }
  // At this point, i is the largest code for which the lower bound is less than
  // or equal to the lower bound of the provided interval. Thus, we only need to
  // check the upper bound.
  if (within(range_min, range_max, upper(i)) >= value_max) {
    *code = i;
    return true;
  } else {
    // The upper bound for this code does not encapsulate the interval.
    // Therefore, no such code exists.
    return false;
  }
}

// Set the relative frequency of the specified code to the given value.
void Intervals::SetFrequency(int code, count frequency) {
  check_bounds(code);

  // Update the frequency value.
  count frequency_difference = frequency - frequency_[code];
  total_ += frequency_difference;
  frequency_[code] += frequency_difference;

  // Update the cumulative frequency tree.
  int i = size_ + code;
  do {
    int parent = (i - 1) / 2;

    if (i % 2) {
      // i is the left child of its parent. Thus, the parent count includes this
      // count.
      cumulative_frequency_[parent] += frequency_difference;
    }
    
    i = parent;
  } while (i != 0);

  // If the frequency totals exceed the bound, reduce all the frequencies. This
  // recurses, so it will repeat as many times as necessary.
  if (total_ > ONE) return WeakenFrequencies();
}

// Get the relative frequency of the specified code.
count Intervals::frequency(int code) const {
  check_bounds(code);
  return frequency_[code];
}

// Print out the intervals.
void Intervals::DebugPrint() const {
  for (int i = 0; i < num_codes_; i++) {
    double lower_bound = static_cast<double>(lower(i)) / ONE;
    double upper_bound = static_cast<double>(upper(i)) / ONE;
    fprintf(stderr, "%3d : [%1.10lf, %1.10lf)\n", i, lower_bound, upper_bound);
  }
}

// Check that the given code index is within the acceptable bounds for this
// instance.
void Intervals::check_bounds(int code) const {
  if (code < 0 || num_codes_ <= code)
    throw runtime_error("Symbol code out of bounds.");
}

// Compute a bound based upon the values in the frequency tree.
bound Intervals::CalculateBound(int code) const {
  return ONE * CumulativeFrequency(code) / total_;
}

// Calculate the cumulative frequency up (but not including) the given code.
count Intervals::CumulativeFrequency(int code) const {
  if (code == 0) return 0;
  if (code == num_codes_) return total_;

  count cumulative = 0;
  int i = size_ + code;
  do {
    int parent = (i - 1) / 2;

    if ((i % 2) == 0) {
      // i is the right child of its parent. Thus, the parent is a component of
      // the cumulative frequency.
      cumulative += cumulative_frequency_[parent];
    }

    i = parent;
  } while (i != 0);
  return cumulative;
}

// Reduce all relative frequencies by a factor of 2, except those which are 1.
void Intervals::WeakenFrequencies() {
  // Decrease each frequency count by half.
  for (int i = 0; i < num_codes_; i++) {
    if (frequency_[i] > 1) SetFrequency(i, frequency_[i] / 2);
  }
  cerr << "WeakenFrequencies()\n";
}
