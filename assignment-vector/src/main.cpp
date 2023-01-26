#include "Datum.h"
#include "Vector.h"
#include <fstream>

#include <string>
#include <vector>

int main() {
    Vector<int> vec(4);

    using iter = Vector<int>::iterator;
    iter begin = vec.begin();       // 1

    for(iter it = begin; it != vec.end(); it++)
	std::cout << *it << " ";
    std::cout << std::endl;

for(int i = 0; i < 3; i++)
	vec.push_back(4);        // 2

for(iter it = begin; it != vec.end(); it++)
	std::cout << *it << " "; // 3
std::cout << std::endl;
}
