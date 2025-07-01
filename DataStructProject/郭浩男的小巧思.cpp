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
	int car_set;//ͣ��λ��
	int ParkSize;
}Park;


typedef struct Park_List
{
	string data;
	Park_List* next = nullptr;
}Qnode;
typedef struct Out_park
{
	Qnode* front;//��ͷ
	Qnode* rear;//��β
	int out_set;//���λ��
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
		//std::cout << "ͣ�����������뵽����ȴ�" << std::endl;  
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
	Init_park(Q);//��������ջ
	int i = findCarInTheParking(P, car);
	if (i == -1)
	{
		std::cout << "���޴˳�" << std::endl;
		return;
	}
	int m = P.top - P.base;//��ǰ��������Ŀ
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
	{ // �����ȴ��ĳ�����ͣ����
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
	std::cout << "��ǰ�ĳ���״̬���£�";
	int m = P.top - P.base;
	Park Q;
	Init_park(Q);
	std::cout << "ͣ��ͣ�����ڵģ�";
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
	std::cout << "ͣ�ڱ����";
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
	if (j == -2)std::cout << "���޴˳�" << std::endl;
	if (i != -1) std::cout << "�˳�λ��ͣ�����ڣ�" << "��" << i+1 << "��λ��" << std::endl;
	if (j != -1)std::cout << "�˳�λ�ڱ����" << "��" << j+1 << "��λ��" << std::endl;
}

void wait() {
	std::cout << "����ո������";
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
		system("cls"); // ����
	std::cout << "���еĹ������£�" << std::endl
		<< "1.ѡ��������ͣ����" << std::endl
		<< "2.ѡ�����뿪ͣ����" << std::endl
		<< "3.��ѯĳ������״̬" << std::endl
		<< "4.��ѯĿǰ��ͣ����״̬" << std::endl
		<< "ѡ������һ�����ܽ���ͣ��������" << std::endl
		<< "���롮0����������" << std::endl;


	std::cin >> m;
	if (m == 0)break;
	switch (m)
	{
	case 1:
		std::cout << "ѡ��Ҫ����ĳ�����(ͣ�������������Ϊ5����)��" << std::endl;
		std::cin >> num; std::cout << "�����������ĳ�����" << std::endl;
		for (int i = 1; i <= num; i++)
		{
			std::cin >> car;
			DrivingIntoParking(P, L, car);
			if (i == 5)std::cout << "ͣ�������������³����뵽����ȴ�" << std::endl;
			if (i > 5)std::cout << car << "  ";
		}	printf("\n");
		wait();
		break;
	case 2:
		int DriveOutNum;

		std::cout << "ѡ����Ҫʻ���ĳ���������" << std::endl;
		std::cin >> DriveOutNum;
		for (int i = 0; i < DriveOutNum; i++)
		{
			std::cout << "������Ҫʻ���ĳ���������ʻ������" << std::endl;
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