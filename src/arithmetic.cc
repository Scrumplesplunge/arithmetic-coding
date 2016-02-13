#include "arithmetic_decoder.h"
#include "arithmetic_encoder.h"
#include "coding.h"

#include <stdexcept>
#include <string>

using namespace std;

int main(int argc, char* args[]) {
  if (argc < 2) {
    cerr << "Usage:\n"
         << " arithmetic encode <plain_input >encoded_output\n"
         << " arithmetic decode <encoded_input >plain_output\n";
    return 1;
  }

  string mode = args[1];

  // Initialize the intervals from the encoding scheme.
  Coding coding;
  Intervals intervals = coding.base_intervals();

  if (mode == "encode") {
    ArithmeticEncoder encoder(cout);

    char c;
    while (cin.get(c)) {
      // Encode the character if it has an encoding. Otherwise, ignore it.
      int code;
      if (coding.encode(c, &code)) {
        encoder.Encode(intervals, code);
        // Update the interval table.
        intervals.SetFrequency(code, intervals.frequency(code) + 1);
      } else {
        cerr << "Shit. Fan.\n";
      }
    }

    // Signify the end of the stream.
    encoder.Encode(intervals, coding.eof());
  } else if (mode == "decode") {
    ArithmeticDecoder decoder(cin);

    int code = decoder.Decode(intervals);
    int i = 0;
    while (code != coding.eof()) {
      i++;
      char c;
      if (!coding.decode(code, &c)) {
        throw logic_error(
            "Invalid codepoint returned from decoder: " + to_string(code));
      }
      cout << c;
      // Update the interval table.
      intervals.SetFrequency(code, intervals.frequency(code) + 1);
      code = decoder.Decode(intervals);
    }
  } else {
    cerr << "Invalid mode. Run without arguments for usage instructions.\n";
    return 1;
  }
}
