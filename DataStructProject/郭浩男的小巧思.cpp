#include<stdio.h>
#include<iostream>
#define MAXSIZE 5
#define OK 1
#define ORRER 0
int Parking_num = 0;
int Waiting_num = 0;
typedef std::string string;


typedef struct Park_Stack
{
	string* base, * top;
	int car_set;//停车位置
	int ParkSize;
}Park;


typedef struct Park_List
{
	string data;
	Park_List* next = nullptr;
}Qnode;
typedef struct Out_park
{
	Qnode* front;//队头
	Qnode* rear;//队尾
	int out_set;//便道位置
}List;


void Init_park(Park& P)
{
	P.base = new string[5];
	if (!P.base)exit(ORRER);
	P.top = P.base;
	P.ParkSize = MAXSIZE;
}


void Init_List(List& L)
{
	L.front = new Qnode;
	if (!L.front)exit(ORRER);
	L.rear = L.front;
	L.front->next = nullptr;
}


void Get_Parking(const Park& P, string& car)
{
	if (P.base == P.top)exit(ORRER);
	car = *(P.top - 1);
	std::cout << car << "  ";
}


void Get_Waiting(const List& L, string& car)
{
	if (L.front == L.rear) exit(ORRER);
	Qnode* temp = L.front->next;
	car = temp->data;
	std::cout << car << "  ";
}


void DrivingIntoParking(Park& P, List& L, string car)
{
	string vv;
	if (Parking_num < MAXSIZE)
	{
		*P.top = car;
		P.car_set = ++Parking_num;
		P.top++;
	}
	else
	{
		//std::cout << "停车场已满，请到便道等待" << std::endl;  
		Qnode* q = new Qnode;
		if (!q) exit(ORRER);
		q->data = car;
		q->next = nullptr;
		L.rear->next = q;
		L.rear = q;
		Waiting_num++;
		L.out_set = Waiting_num;
	}
}
int findCarInTheParking(Park P, string car)
{
	int m = P.top - P.base;
	for (int i = 0; i <= m; i++)
	{
		if (car == P.base[i])return i;
	}
	return -1;
}
int findCarInTheList(List L, string car)
{
	while (L.front != L.rear)
	{
		if (L.front->data == car)return L.out_set;
	}
	return -1;
}
void DriveOutOffTheParking(Park& P, List& L, string& car)
{
	Park Q;
	Init_park(Q);//建立辅助栈
	int i = findCarInTheParking(P, car);
	if (i == -1)
	{
		std::cout << "查无此车" << std::endl;
		return;
	}
	int m = P.top - P.base;//当前车辆的数目
	for (int j = m - 1; j > i; --j)
	{
		Q.base[Q.top - Q.base] = *(--P.top);
		Q.top++;
	}
	--P.top;
	Parking_num--;
	while (Q.top != Q.base)
	{
		*P.top = *(--Q.top);
		P.top++;
	}
	if (L.front != L.rear)
	{ // 外便道等待的车辆进停车场
		Qnode* temp = L.front->next;
		*(P.top) = temp->data;
		P.top++;
		Parking_num++;
		L.front->next = temp->next;
		if (L.rear == temp) L.rear = L.front;
		delete temp;
		Waiting_num--;
	}
}
void PlayTheRealParking(Park P, List L)
{
	std::cout << "当前的车辆状态如下：";
	int m = P.top - P.base;
	Park Q;
	Init_park(Q);
	std::cout << "停在停车场内的：";
	for (int i = 1; i <= m; i++)
	{
		Q.base[Q.top - Q.base] = *(--P.top);
		Q.top++;

	}
	while (Q.top != Q.base)
	{
		string car;
		Get_Parking(Q, car);
		Q.top--;

	}
	std::cout << std::endl;
	std::cout << "停在便道：";
	if (L.rear != L.front)
	{
		for (int i = 1; i <= Waiting_num; i++)
		{
			string car;
			Get_Waiting(L, car);
			L.front = L.front->next;
		}
	}
	std::cout << std::endl;
}
void QueryTheCarSituation(Park P, List L, string car)
{
	int i;
	int j;
	i = findCarInTheParking(P, car);
	j = findCarInTheList(L, car);
	j = j + i;
	if (j == -2)std::cout << "查无此车" << std::endl;
	if (i != -1) std::cout << "此车位于停车场内：" << "第" << i+1 << "个位置" << std::endl;
	if (j != -1)std::cout << "此车位于便道：" << "第" << j+1 << "个位置" << std::endl;
}

void wait() {
	std::cout << "输入空格继续。";
	std::cin.ignore();
	std::cin.get();
}

int main()
{
	Park P;
	List L;
	Init_park(P);
	Init_List(L);
	string car;
	string a;
	string DriveOutCar;
	string QueryCar;
	int m;
	int num;

	while (true) {
		system("cls"); // 清屏
	std::cout << "现有的功能如下：" << std::endl
		<< "1.选择车辆进入停车场" << std::endl
		<< "2.选择车辆离开停车场" << std::endl
		<< "3.查询某辆车的状态" << std::endl
		<< "4.查询目前的停车场状态" << std::endl
		<< "选择以上一个功能进行停车场测试" << std::endl
		<< "输入‘0’结束操作" << std::endl;


	std::cin >> m;
	if (m == 0)break;
	switch (m)
	{
	case 1:
		std::cout << "选择要进入的车辆数(停车场内最大容量为5辆车)：" << std::endl;
		std::cin >> num; std::cout << "依次输入进入的车辆：" << std::endl;
		for (int i = 1; i <= num; i++)
		{
			std::cin >> car;
			DrivingIntoParking(P, L, car);
			if (i == 5)std::cout << "停车场已满，以下车辆请到便道等待" << std::endl;
			if (i > 5)std::cout << car << "  ";
		}	printf("\n");
		wait();
		break;
	case 2:
		int DriveOutNum;

		std::cout << "选择需要驶出的车辆个数：" << std::endl;
		std::cin >> DriveOutNum;
		for (int i = 0; i < DriveOutNum; i++)
		{
			std::cout << "输入需要驶出的车辆（依次驶出）：" << std::endl;
			std::cin.ignore();
			std::cin >> DriveOutCar;
			DriveOutOffTheParking(P, L, DriveOutCar);

		}PlayTheRealParking(P, L); 
		wait();
		break;
	case 3:
		std::cin >> QueryCar;
		QueryTheCarSituation(P, L, QueryCar);
		wait();
		break;
	case 4:
		PlayTheRealParking(P, L);
		wait();
		break;
	}
}
	return OK;
}

if (findcar(S, car) == (-1)) { std::cout << "there is already a same car!"; }
else
{
	park();
}