#include "byte_output.h"

using namespace std;

const int ByteOutput::BUFFER_SIZE = 1 << 16;  // 64K

ByteOutput::ByteOutput(ostream& out)
    : out_(out), buffer_(new byte[BUFFER_SIZE]) {}

ByteOutput::~ByteOutput() {
  flush();
}

// Output a byte.
void ByteOutput::put(byte x) {
  if (index_ == BUFFER_SIZE) {
    // Output the contents of the buffer.
    out_.write(reinterpret_cast<char*>(buffer_.get()), BUFFER_SIZE);
    index_ = 0;
  }

  buffer_[index_++] = x;
}

// Flush the currently buffered bits, filling the empty spaces with 0's.
void ByteOutput::flush() {
  if (index_ == 0) return;

  // Output the buffered bytes.
  out_.write(reinterpret_cast<char*>(buffer_.get()), index_);
  
  index_ = 0;
}
