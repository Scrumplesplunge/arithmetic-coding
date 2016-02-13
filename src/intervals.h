#pragma once

#include "types.h"

#include <memory>

// Represents an interval partition of [0, 1) between a number of codes.
class Intervals {
 public:
  // Representation of 1 in the bound type. This must satisfy the constraint
  // that neither of:
  // 
  // ONE * <sum of all relative frequencies>
  // ONE^2
  //
  // overflow.
  static const bound ONE;

  // Returns the location of value when [0, ONE) is rescaled to [min, max).
  static bound within(bound min, bound max, bound value);

  // Construct a new instance with intervals for codes [0..num_codes).
  Intervals(int num_codes);

  // Fetch the (inclusive) lower bound for the given code.
  bound lower(int code) const;

  // Fetch the (exclusive) upper bound for the given code.
  bound upper(int code) const;

  // Given that the current range is [range_min, range_max), find the code
  // containing the interval [value_min, value_max), if there is one.
  // Returns true and populates the output only if such a code was found.
  bool CodeAt(bound range_min, bound range_max, bound value_min,
              bound value_max, int* code) const;

  // Set the relative frequency of the specified code to the given value.
  void SetFrequency(int code, count frequency);

  // Get the relative frequency of the specified code.
  count frequency(int code) const;

  void DebugPrint() const;

 private:
  // Check that the given code index is within the acceptable bounds for this
  // instance.
  void check_bounds(int code) const;

  // Compute a bound based upon the values in the frequency tree.
  bound CalculateBound(int code) const;

  // Calculate the cumulative frequency up (but not including) the given code.
  count CumulativeFrequency(int code) const;

  // Reduce all relative frequencies by a factor of 2, except those which are 1.
  void WeakenFrequencies();

  // Number of different codes that have intervals.
  int num_codes_;

  // Relative frequency of each code type. Used for computing the interval
  // bounds.
  std::unique_ptr<count[]> frequency_;

  // Cumulative frequency tree. This is a balanced tree (similar to the array
  // form of a heap). It is used to calculate the cumulative sum of all
  // frequencies up to a particular code.
  count total_ = 0;
  int size_;
  std::unique_ptr<count[]> cumulative_frequency_;
};
