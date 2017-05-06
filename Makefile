all: metarndtest runtime

metarndtest: metarndtest.cc
	g++ -I metarnd -Wall -Wextra -std=c++11 metarndtest.cc -o metarndtest

runtime: runtime.cc
	g++ -Wall -Wextra -std=c++11 runtime.cc -o runtime
