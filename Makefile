
all: test_set0 test_set1 test_set3

clean:
	rm -f test_set0 test_set1 test_set3
test_set0: test_set.cpp
	$(CXX) -O0 test_set.cpp -o test_set0
test_set1: test_set.cpp
	$(CXX) -O1 test_set.cpp -o test_set1
test_set3: test_set.cpp
	$(CXX) -O3 test_set.cpp -o test_set3
