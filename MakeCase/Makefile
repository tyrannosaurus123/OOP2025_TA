all: gen clean

_input.out: input.cpp
	g++ -std=c++17 input.cpp -o _input.out

gen_input: _input.out
	./_input.out

_output.out: output.cpp
	g++ -std=c++17 output.cpp -o _output.out

gen_output: _output.out
	./_output.out

_input_validator.out: input_validator.cpp
	g++ -std=c++17 input_validator.cpp -o _input_validator.out

check: _input_validator.out
	./_input_validator.out

gen: gen_input check gen_output

clean:
	rm _input.out
	rm _output.out
	rm _input_validator.out
