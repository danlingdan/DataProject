// ͷ�ļ�
#include <iostream>
#include <string>
#include <vector>

// ȫ�ֱ���

#define FULL (-1)
typedef std::string string;
typedef size_t Status;
size_t max_car=0; // ����ͣ�������������
#define PARKED (5)
#define WAITING (6)

// ���ṹ��
typedef struct Car
{
	string car_num; // ���ƺ�
	bool is_parked; // �Ƿ���ͣ��

	void input() {
		std::cout << "�����복�ƺţ�";
		std::cin >> car_num;
	}
	void output() {
		std::cout << car_num;
	}
}Car;
// ջ(ͣ����)�ṹ��
typedef struct Stack
{
	Car* top; // ջ��ָ��
	Car* base; // ջ��ָ��
	std::vector<int> position; // ��λλ��
}Stack;
// ����(���)�ṹ��
typedef struct QNode
{
	Car car; // ������Ϣ
	std::vector<int> position; // �����ڱ���е�λ��
	QNode* next;

}QNode,*QueuePtr;
typedef struct LinkQueue
{
	QueuePtr front; // ��ͷָ��
	QueuePtr rear; // ��βָ��
	size_t size;
}LinkQueue;

// ջ����

Status InitStack(Stack& S) {
	S.base = new Car[max_car];
	if (!S.base) { return false; }
	S.top = S.base;
	for (int i=1;i <= max_car;i++) {
		S.position.push_back(i);
	}
	return true;
}
bool Empty(Stack S) {
	if (S.base == S.top) { return true; }
	else { return false; }
}
Status Push(Stack& S, Car car) {
	if (S.top - S.base >= max_car) { return FULL; }
	else {
		*S.top = car;
		S.top->is_parked = true;
		S.top++;
		return true;
	}
}
Status Pop(Stack& S) {
	if (Empty(S)) { return false; }
	else
	{
		S.top--;
		return true;
	}
}
auto Top(Stack S) {
	return *(S.top - 1);
}
size_t Size(Stack S) {
	return (S.top - S.base);
}

// ���к���

Status InitQueue(LinkQueue& Q) {
	Q.rear = new QNode;
	if (!Q.rear) { return false; }
	Q.front = Q.rear;
	Q.front->next = nullptr;
	Q.size = 0;
	return true;
}
bool Empty(LinkQueue Q) {
	if (Q.front == Q.rear) { return true; }
	else { return false; }
}
size_t Size(LinkQueue Q) {
	return (Q.size);
}
Status Push(LinkQueue& Q,Car car) {
	QNode* p = new QNode;
	if (!p) { return false; }
	p->car = car;
	p->next = nullptr;
	Q.rear->next = p;
	Q.rear = p;
	Q.rear->car.is_parked = false;
	Q.size++;

	Q.rear->position.clear();
	for (int i = 1; i <= Q.size; i++) { Q.rear->position.push_back(i); }
	return true;
}
Status Pop(LinkQueue& Q) {
	if (Empty(Q)) { return false; }
	else {
		QNode* p = Q.front->next; // ָ��Ҫɾ���Ľڵ�
		Q.front->next = p->next;
		if (Q.rear == p) { Q.rear = Q.front; }
		delete p;
		Q.size--;

		if (!Empty(Q)) {
			QueuePtr node = Q.front->next;
			int i = 1;
			while (node) {
				node->position.clear();
				node->position.push_back(i);
				node = node->next;
				i++;
			}
		}
		return true;
	}
}
auto Front(LinkQueue Q) {
	return Q.front->next->car;
}

// ���ܺ���

Status ParkCar(Stack& S, LinkQueue& Q, Car car) {
	car.input();
	if (Push(S, car) == FULL) {
		if (Push(Q, car) == true) { return WAITING; }
	}
	return PARKED;
}
Status DrivingOut(Stack& S, LinkQueue& Q, Car car) {
	car.input();
	bool found = false;
	Stack temp;
	InitStack(temp);
	for (size_t i = 0; i < Size(S); i++)
	{
		if (Top(S).car_num != car.car_num) {
			Push(temp, Top(S));
			Pop(S);
		}
		else { 
		found = true; 
		Pop(S); }
	}
	if (found == false) { std::cout << "δ�ҵ�����"; }
	for (size_t i = 0; i < Size(temp); i++) {
		Push(S, Top(temp));
		Pop(temp);
	}

	if (!Empty(Q)) {
		Push(S, Front(Q));
		Pop(Q);
	}
	return true;
}
Status ShowPark(Stack S) {
	std::cout << "��ǰ��" << Size(S) << "������ͣ������"<< std::endl;
	for (int i = 0; i<Size(S); i++) { 
		std::cout << "���ƺţ�";
		S.base[i].output();
		std::cout << "������" << S.position.at(i) << std::endl;
	}
	return true;
}
Status ShowQueue(LinkQueue Q) {
	std::cout << "��ǰ��" << Size(Q) << "�����ڱ����" << std::endl;
	QueuePtr node = Q.front->next;
	int i = 1;
	while (node) {
		std::cout << "���ƺţ�";
		node->car.output();
		for (size_t t = 0; t < Q.size; t++)
		{
			std::cout << "����ţ�" << Q.front->next->position.at(t);
		}
		std::cout << std::endl;
		node = node->next;
		i++;
	}
	return true;
}

void ClearBuff() {
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
void Wait() { std::cout << "���������������"; std::cin.get(); }
void Menu() {
	printf("���г���ͣ��ʱ���롰1��\n");
	printf("���г���ʻ��ʱ���롰2��\n");
	printf("�����ǰͣ����״̬��3��\n");
	printf("�����ǰ���״̬��4��\n");
	printf("����������0��\n");
}
void DestroyStack(Stack& S) {
	if (S.base) {
		delete[] S.base;
		S.base = nullptr;
		S.top = nullptr;
	}
}
void DestroyQueue(LinkQueue& Q) {
	while (!Empty(Q)) {
		Pop(Q);
	}
	if (Q.front) {
		delete Q.front;
		Q.front = Q.rear = nullptr;
	}
}

int main() {
	std::cout << "������ͣ���������������";
	std::cin >> max_car;
	ClearBuff();

	Stack S;
	InitStack(S);
	LinkQueue Q;
	InitQueue(Q);
	while (true)
	{
		system("cls");
		Menu();
		std::cout << "������ѡ��";
		int choice;
		std::cin >> choice;
		if (choice == 0) {
			std::cout << "���������ллʹ�ã�" << std::endl;
			DestroyStack(S);
			DestroyQueue(Q);
			break;
		}
		Car car;
		switch (choice)
		{
		case 1:
		{
			ParkCar(S, Q, car);
			ClearBuff();
			Wait();
			break;
		}
		case 2: {
			DrivingOut(S, Q, car);
			ClearBuff();
			Wait();
			break;
		}
		case 3: {
			ShowPark(S);
			ClearBuff();
			Wait();
			break;
		}
		case 4: {
			ShowQueue(Q);
			ClearBuff();
			Wait();
			break;
		}
		default:
			break;
		}
	}
}