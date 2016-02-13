CXXFLAGS = -std=c++14 -Wall -Ofast -g

.PHONY: old new clean

arithmetic: src/arithmetic.cc src/arithmetic_decoder.cc               \
            src/arithmetic_encoder.cc src/coding.cc src/bit_input.cc  \
            src/intervals.cc src/bit_output.cc src/byte_input.cc      \
						src/byte_output.cc
	${CXX} ${CXXFLAGS} $^ -o $@

clean:
	rm -rf arithmetic
