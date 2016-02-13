#include "bit_output.h"

using namespace std;

const int BitOutput::BUFFER_SIZE = 1 << 16;  // 64K

BitOutput::BitOutput(ostream& out)
    : out_(out), buffer_(new byte[BUFFER_SIZE]) {}

BitOutput::~BitOutput() {
  flush();
}

// Output a single bit.
void BitOutput::put(bit x) {
  if (byte_index_ == BUFFER_SIZE) {
    // Output the contents of the buffer.
    out_.write(reinterpret_cast<char*>(buffer_.get()), BUFFER_SIZE);
    byte_index_ = 0;
    bit_index_ = 0;
  }

  byte b = (bit_index_ == 0) ? 0 : buffer_[byte_index_];
  b |= x << bit_index_;
  buffer_[byte_index_] = b;

  // Advance the index.
  bit_index_++;
  if (bit_index_ == 8) {
    byte_index_++;
    bit_index_ = 0;
  }
}

// Flush the currently buffered bits, filling the empty spaces with 0's.
void BitOutput::flush() {
  if (byte_index_ == 0 && bit_index_ == 0) return;

  // Output the buffered bytes. If the bit_index_ is 0, there is nothing in
  // buffer_[byte_index_] to be output.
  out_.write(reinterpret_cast<char*>(buffer_.get()),
             (bit_index_ == 0) ? byte_index_ : byte_index_ + 1);

  byte_index_ = 0;
  bit_index_ = 0;
}
