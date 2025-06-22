#include <iostream>
#include <stack>
#include <string>
#include <queue>
#include <utility>
#include <iterator>

// 全局变量（停车场最大容量）
unsigned int max_capacity = 0;

// 车辆类定义(及实现)
class Car {
private:
	std::string car_num;
	bool is_parked;
public:
	Car() = default;
	auto getCarNum() { return car_num; }
	bool getIsParked() { return is_parked; }
	void input();
	void output();
	void setParked() { is_parked = true; }
};
void Car::input() {
	using namespace std;
	cout << "Please enter the car number:";
	cin >> car_num;
}
void Car::output() {
	std::cout << car_num;
}

// 函数实现

// 辅助输出函数
void wait() {
	std::cout << "输入空格继续。";
	std::cin.ignore();
	std::cin.get();
}

// 添加到等待队列
void addToTheWaitingLine(std::queue<Car>& waiting_line, Car car) {
	waiting_line.push(car);
}

// 获取等待队列中最顶端的车辆
auto getTheToppestWaitingCar(std::queue<Car> waiting_line) { return waiting_line.front(); }

// 停车函数
void parkCar(std::stack<Car>& park_lot, std::queue<Car>& waiting_line, Car car) {
	car.input();
	if (park_lot.size() > max_capacity) { addToTheWaitingLine(waiting_line, car); }
	else {
		park_lot.push(car);
	}
}
// 驶出停车场

void driveOutOfThePark(std::stack<Car>& park_lot, std::queue<Car>& waiting_line, std::string car_num) {
	auto temp1 = park_lot;
	unsigned int index = 0;
	for (; park_lot.size(); ) {
		Car temp_car = temp1.top();
		temp1.pop();
		if (car_num == temp_car.getCarNum()) { break; }
		index = index + 1;
	}

	auto temp2 = park_lot;
	temp2 = {};
	for (unsigned int i = 0; i < index; i++) {
		temp2.push(park_lot.top());
		park_lot.pop();
	}
	temp2.pop();
	for (; temp2.size();) {
		park_lot.push(temp2.top());
		temp2.pop();
	}

	park_lot.push(getTheToppestWaitingCar(waiting_line));
	waiting_line.pop();
}

int main() {
	std::cout << "Please enter the maximum parked car:";
	// 获取停车场最大容量
	std::cin >> max_capacity;
	std::cout << std::endl;
	// 初始化停车场和等待队列
	static std::stack<Car> ParkLot = {};
	static std::queue<Car> WaitingLine = {};
	// 临时变量
	Car temp1;
	std::string temp_car_num;
	while (true){
	system("cls");
	std::cout << "Welcome to the Parking Lot Management System!" << std::endl;
	std::cout << "1. Park a car" << std::endl;
	std::cout << "2. Drive out of the park" << std::endl;
	std::cout << "3. Check the number of cars in the parking lot" << std::endl;
	std::cout << "4. Check the number of cars in the waiting line" << std::endl;
	std::cout << "Please choose the operation:" << std::endl;
	short choice = 0;
	std::cin >> choice;
	switch (choice)
	{
	case 1:
		temp1 = {};
		parkCar(ParkLot, WaitingLine, temp1);
		break;
	case 2:
		std::cin >> temp_car_num;
		driveOutOfThePark(ParkLot, WaitingLine, temp_car_num);
		break;
	case 3:
		std::cout << ParkLot.size();
		std::cout << " cars in the parking lot." << std::endl;
		wait();
		break;
	case 4:
		std::cout << WaitingLine.size();
		std::cout << " cars in the waiting line." << std::endl;
		wait();
		break;
	default:
		std::cout << "Invalid choice. Please try again." << std::endl;
		wait();
		break;
	}
}
}