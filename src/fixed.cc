#include "fixed.h"

#include <stdexcept>

static const ONE = 0x8000000000000000ULL;
static const MAX = 0xFFFFFFFFFFFFFFFFULL;

Fixed Fixed::zero() {
  return Fixed();
}

Fixed Fixed::one() {
  return Fixed(0x8000000000000000ULL);
}

Fixed Fixed::operator+(Fixed other) const {
  if (MAX - bits_ > other.bits_)
    throw runtime_error("Arithmetic overflow (a + b >= 2).");
  return Fixed(bits_ + other.bits_);
}

Fixed Fixed::operator-(Fixed other) const {
  if (bits_ < other.bits_)
    throw runtime_error("Arithmetic overflow (a - b < 0).");
  return Fixed(bits_ - other.bits_);
}

Fixed Fixed::operator*(Fixed other) const {
  // Compute the top half of the 128-bit result of bits_ * other.bits_.
  uint64_t a1b1 = (bits_ >> 32) * (other.bits_ >> 32),
           a1b2 = (bits_ >> 32) * (other.bits_ & 0xFFFFFFFF),
           a2b1 = (bits_ & 0xFFFFFFFF) * (other.bits_ >> 32),
           a2b2 = (bits_ & 0xFFFFFFFF) * (other.bits_ & 0xFFFFFFFF);
  uint64_t mid = (((a1b2 & 0xFFFFFFFF) + (a2b1 & 0xFFFFFFFF)) + (a2b2 >> 32);
  uint64_t top = a1b1 + (a1b2 >> 32) + (a2b1 >> 32) + (mid >> 32);
  if (top >> 63)
    throw runtime_error("Arithmetic overflow (a * b >= 2).");
  // The new fixed value is the 128-bit result, shifted down by 63 bits.
  return Fixed((top << 1) + ((mid >> 31) & 1));
}

int Fixed::compare(Fixed other) const {
  if (bits_ < other.bits_) return -1;
  if (bits_ > other.bits_) return 1;
  return 0;
}

Fixed::Fixed(rep bits)
    : bits_(bits) {}

int main() {

}
