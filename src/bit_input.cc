#include "bit_input.h"

using namespace std;

const int BitInput::BUFFER_SIZE = 1 << 16;  // 64K

BitInput::BitInput(istream& in)
    : in_(in), buffer_(new byte[BUFFER_SIZE]) {}

// Read a single bit.
bool BitInput::get(bit* out) {
  if (byte_index_ == bytes_buffered_) {
    // Buffer as much input as possible.
    in_.read(reinterpret_cast<char*>(buffer_.get()), BUFFER_SIZE);
    bytes_buffered_ = in_.gcount();
    bit_index_ = 0;
    byte_index_ = 0;
  }
  if (byte_index_ == bytes_buffered_) return false;

  *out = (buffer_[byte_index_] >> bit_index_) & 1;

  // Advance the index.
  bit_index_++;
  if (bit_index_ == 8) {
    byte_index_++;
    bit_index_ = 0;
  }
  return true;
}
