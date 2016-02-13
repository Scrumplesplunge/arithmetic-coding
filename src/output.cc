#include "output.h"

using namespace std;

Output::Output(ostream& out)
    : out_(out) {}

Output::~Output() {
  flush();
}

// Output a single bit.
Output& Output::operator<<(bit x) {
  put(x);
  return *this;
}

// Output a single bit.
void Output::put(bit x) {
  buffered_ = (buffered_ << 1) | x;
  bits_buffered_++;
  if (bits_buffered_ == 8) {
    out_.put(static_cast<char>(buffered_));
    bits_buffered_ = 0;
    buffered_ = 0;
  }
}

// Flush the currently buffered bits, filling the empty spaces with 0's.
void Output::flush() {
  if (bits_buffered_ == 0) return;

  buffered_ <<= (8 - bits_buffered_);
  out_.put(static_cast<char>(buffered_));
  bits_buffered_ = 0;
  buffered_ = 0;
}
