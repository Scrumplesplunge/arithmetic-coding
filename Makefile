CXXFLAGS = -std=c++14 -Wall -Ofast -flto -s

.PHONY: old new clean

arithmetic: src/arithmetic.cc src/arithmetic_decoder.cc           \
            src/arithmetic_encoder.cc src/coding.cc src/input.cc  \
            src/intervals.cc src/output.cc
	${CXX} ${CXXFLAGS} $^ -o $@

old: src/intervals.h src/intervals.cc src/intervals.h.old src/intervals.cc.old
	mv src/intervals.h src/intervals.h.new
	mv src/intervals.cc src/intervals.cc.new
	mv src/intervals.h.old src/intervals.h
	mv src/intervals.cc.old src/intervals.cc

new: src/intervals.h src/intervals.cc src/intervals.h.new src/intervals.cc.new
	mv src/intervals.h src/intervals.h.old
	mv src/intervals.cc src/intervals.cc.old
	mv src/intervals.h.new src/intervals.h
	mv src/intervals.cc.new src/intervals.cc

clean:
	rm -rf arithmetic
