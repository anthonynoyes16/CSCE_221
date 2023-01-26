#include "executable.h"
#include <vector>

TEST(at) {
    
        Vector<int> vec(4);

    using iter = Vector<int>::iterator;
    iter begin = vec.begin();       // 1

    for(iter it = begin; it != vec.end(); it++){
	std::cout << *it << " ";
    
    }
    std::cout << "vec end is" << vec.end()-begin << std::endl;
std::cout << std::endl;
for(int i = 0; i < 3; i++){
	vec.push_back(4); 
    }       // 2
std::cout << "vec end is" << vec.end()-begin << std::endl;
std::cout << "vec begin is " << vec.begin()-vec.end() << std::endl;
for(iter it = begin; it != vec.end(); it++){
	std::cout << *it << " "; // 3


    }
    std::cout << std::endl;
}
