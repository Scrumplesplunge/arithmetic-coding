#include "byte_input.h"

using namespace std;

const int ByteInput::BUFFER_SIZE = 1 << 16;  // 64K

ByteInput::ByteInput(istream& in)
    : in_(in), buffer_(new byte[BUFFER_SIZE]) {}

// Read a byte.
bool ByteInput::get(byte* out) {
  if (index_ == bytes_buffered_) {
    // Buffer as much input as possible.
    in_.read(reinterpret_cast<char*>(buffer_.get()), BUFFER_SIZE);
    bytes_buffered_ = in_.gcount();
    index_ = 0;
  }
  if (bytes_buffered_ == 0) return false;
  *out = buffer_[index_++];
  return true;
}
