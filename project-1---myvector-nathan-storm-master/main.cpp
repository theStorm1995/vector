
#include <iostream>

#include "MyVector.hpp"

using std::cout;
using std::endl;

/**
 * You don't have to modify this source file, but can if you want.
 * This will not be used for grading, so you can use it to debug your
 * vector class.
 * This is the program that runs with "make run"
 */

int main()
{
	MyVector<int> vec(101);
	size_t size_loc = 0;
	for(size_t i; i < 101; i++) {
		vec.push_back(i);
	}
	
	vec.erase(23);

	cout << "\nVec" << endl;
	cout << vec[23] << endl;
	cout << size_loc << endl;

	return 0;
}


