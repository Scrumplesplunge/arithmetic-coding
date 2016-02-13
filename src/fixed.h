#pragma once

#include <stdint.h>

// Fixed precision fractional number. This can represent numbers in the range
// [0, 2) with a fixed degree of precision.
class Fixed {
 public:
  typedef uint64_t rep;

  Fixed() = default;

  static Fixed zero();
  static Fixed one();

  rep bits() const { return bits_; }

  Fixed operator+(Fixed other) const;
  Fixed operator-(Fixed other) const;
  Fixed operator*(Fixed other) const;

  bool operator==(Fixed other) const { return compare(other) == 0; }
  bool operator!=(Fixed other) const { return compare(other) != 0; }
  bool operator<=(Fixed other) const { return compare(other) <= 0; }
  bool operator>=(Fixed other) const { return compare(other) >= 0; }
  bool operator<(Fixed other) const  { return compare(other) <  0; }
  bool operator>(Fixed other) const  { return compare(other) >  0; }

 private:
  int compare(Fixed other) const;
  
  Fixed(rep bits);

  rep bits_ = 0;
};
