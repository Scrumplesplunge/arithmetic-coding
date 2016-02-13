#include "input.h"

using namespace std;

Input::Input(istream& in)
    : in_(in) {}

// Read a single bit.
bit Input::get() {
  if (bits_buffered_ == 0) {
    bits_buffered_ = 8;
    buffered_ = static_cast<byte>(in_.get());
  }
  
  bits_buffered_--;
  bit out = (buffered_ >> bits_buffered_) & 1;
  return out;
}
