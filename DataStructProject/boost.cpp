#include <boost/algorithm/algorithm.hpp>
#include <boost/array.hpp>
#include <iostream>


boost::array<int, 5> arr = { 1, 2, 3, 4, 5 };

class MyClass
{
public:
	MyClass(boost::array<int,5>);
	~MyClass();

	void add(int value) {
		for (size_t i = 0; i < arr.size(); ++i) {
			arr[i] += value;
		}
		std::cout << "Added " << value << " to each element." << std::endl;
	}

	void deleteElement(size_t index) {
		if (index < arr.size()) {
			arr[index] = 0; // Set the element to 0
			std::cout << "Deleted element at index " << index << "." << std::endl;
		} else {
			std::cout << "Index out of range." << std::endl;
		}
	}

private:
	boost::array<int, 5> arr;
};

MyClass::MyClass(boost::array<int ,5> arr)
{
	this->arr = arr;
	std::cout << "MyClass constructor called." << std::endl;
	std::cout << "Array elements: ";
	for (const auto& elem : arr) {
		std::cout << elem << " ";
	}
	std::cout << std::endl;
}

MyClass::~MyClass()
{
}


std::unique_ptr<MyClass> myClassPtr = std::make_unique<MyClass>(arr);

int main() {
	std::cout << "Boost Array Example" << std::endl;
	// Add 10 to each element
	myClassPtr->add(10);
	// Delete element at index 2
	myClassPtr->deleteElement(2);
	// Print the modified array
	std::cout << "Modified array elements: ";
	for (const auto& elem : arr) {
		std::cout << elem << " ";
	}
	std::cout << std::endl;
	return 0;
}