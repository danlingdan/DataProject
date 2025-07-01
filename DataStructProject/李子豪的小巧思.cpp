// ͷ�ļ�
#include<stdio.h>
#include <iostream>
#include<stdlib.h>
#include<string>

// ����ȫ�ֱ���
// 
//����곣��
#define OK 1
#define ERROR 0

// �������
typedef int Status;
int count = 0; // ��¼ͣ�����г���������
int baidao = 0; // ��¼����г���������

//�������ռ��ʼ��
#define STACK_INIT_SIZE 1

//����ElemType����
typedef struct {
	std::string carnumber;
}ElemType;

//ջ�洢�ṹ�Ķ���
typedef struct Stack {
	ElemType* top;
	ElemType* base;
	int stacksize = STACK_INIT_SIZE;
}SqStack;

//���д洢�ṹ�Ķ���
typedef struct QNode {
	ElemType info;
	struct QNode* next;
}QNode, * QueuePtr;
typedef struct {
	QueuePtr front;
	QueuePtr rear;
}LinkQueue;

// ��������
Status InitStack(SqStack& S); // ��ʼ��ջ
Status SearchSq(SqStack& S, ElemType e); // ����ջ���Ƿ����ĳ��Ԫ��
Status EnterS(SqStack& S, ElemType e); // ѹ��ջ����������ͣ������
Status Pop(SqStack& S, ElemType& e); // ����ջ������ʻ��ͣ������
void pop(SqStack& S, std::string car_num); // ��ջ��ɾ��ָ������
Status InitQueue(LinkQueue& Q); // ��ʼ������
Status EnterQ(LinkQueue& Q, ElemType e); // ��ӣ������ĳ����ڱ�����Ŷӣ�
Status OutQ(LinkQueue& Q, ElemType& e); // ���ӣ���һ����������ͣ������
void outputPark(SqStack& S); // ���ͣ����״̬
void outputBianDao(LinkQueue& p, SqStack s); // ������״̬
auto top(SqStack S); // ��ȡջ��Ԫ��
bool empty(SqStack S); // �ж�ջ�Ƿ�Ϊ��
void mune(); // ��ʾ�˵�
bool empty(LinkQueue Q);

// ����ʵ��

//��ʼ����ջ������ͣ������
Status InitStack(SqStack& S) {
	S.base = new ElemType[5];
	if (!S.base) exit(ERROR);
	S.top = S.base;
	return OK;
}
//����
Status SearchSq(SqStack& S, ElemType e) {
	for (int i = 0; i < count; i++) {
		if (S.base[i].carnumber == e.carnumber) return 1;
	}
	return 0;
}
//ջ��ѹ�루��������ͣ������
Status EnterS(SqStack& S, ElemType e) {
	if (count > 1) {
		if (SearchSq(S, e))
		{
			std::cout << "���ƺ�������󣨸ó��ƺ��ѽ�ͣ������������δ������" << std::endl;
			return ERROR;
		}
	}
	auto temp = S.top - 1;
	if (count >= S.stacksize) {
		printf("ͣ�����������ó�����ʱͣ�ŵ�����ϵȺ�/n");
		return ERROR;
	}
	*S.top = e;
	S.top++;
	count++;
	return OK;
}
auto top(SqStack S) {
	auto e = *(S.top-1);
	return e;
}
bool empty(SqStack S) {
	if (S.base == S.top) { return true; }
	else { return false; }
}
//ѹ�뵽ջ��
Status Pop(SqStack& S, ElemType& e) {
	if (S.top == S.base) return ERROR;
	S.top--;
	e = *(S.top);
	count--;
	return OK;
}
void pop(SqStack& S, std::string car_num) {
	int current_count = S.top - S.base;
	bool found = false;
	SqStack temp_1;
	InitStack(temp_1);
	SqStack temp_2;
	InitStack(temp_2);
	ElemType useless;
	// �Ȱ�����Ԫ�ص�������ʱջ
	for (int i = 0; i < current_count; i++) {
		EnterS(temp_1, S.base[i]);
	}
	for (int i = 0; i < current_count; i++) {
		EnterS(temp_2, top(temp_1));
		Pop(temp_2, useless);
	}
	// ������ջ������Ҫɾ���ĳ���
	S.top = S.base; // ����ջ��
	while (!empty(temp_2)) {
		ElemType e = top(temp_2);
		Pop(temp_2, useless);
		if (e.carnumber == car_num && !found) {
			found = true; // ������Ԫ��
			count--;
			continue;
		}
		*S.top = e;
		S.top++;
	}

	if (!found) {
		std::cout << "δ�ҵ��ó��ƺų�����" << std::endl;
	}
	else {
		std::cout << "����" << car_num << "��ʻ����" << std::endl;
	}
}


//����ն��У����������
Status InitQueue(LinkQueue& Q) {
	Q.front = Q.rear = new QNode;
	if (!Q.front) exit(OVERFLOW);
	Q.front->next = NULL;
	return OK;
}
//����Ԫ���ڶ�β�������ĳ����ڱ�����Ŷӣ�
Status EnterQ(LinkQueue& Q, ElemType e) {
	QueuePtr p = new QNode;
	if (!p)exit(OVERFLOW);
	p->info = e;
	p->next = NULL;
	Q.rear->next = p;
	Q.rear = p;
	return OK;
}
bool empty(LinkQueue Q) {
	if (Q.front == Q.rear) { return true; }
	else { return false; }
}
auto top(LinkQueue Q) {
	return Q.front->info;
}
//���е��������һ����������ͣ������
Status OutQ(LinkQueue& Q, ElemType& e) {
	if (Q.front == Q.rear) return ERROR;
	QueuePtr p = new QNode;
	p = Q.front->next;
	e = p->info;
	Q.front->next = p->next;
	if (Q.rear == p) {
		Q.rear = Q.front;
	}
	delete p;
	return OK;
}
void outputPark(SqStack& S) { // ������Ϊ����
	int current_count = S.top - S.base;
	LinkQueue temp_1;
	InitQueue(temp_1);
	ElemType e;
	ElemType useless;

	// ����ջԪ�ص���ʱջ
	SqStack tempS = S;
	for (int i = 0; i < current_count; i++) {
		EnterQ(temp_1, tempS.base[i]);
	}

	// �������ԭ
	while (!empty(temp_1)) {
		std::cout << top(temp_1).carnumber << " ";
		OutQ(temp_1, useless);
	}
	std::cout << std::endl;
}
void outputBianDao(LinkQueue& p, SqStack s) {
	LinkQueue temp_1;
	InitQueue(temp_1);
	ElemType e;
	ElemType useless;
	LinkQueue tempQ = p;
	QNode* current = tempQ.front->next;
	for (int i = 0; i < baidao && current != nullptr; i++) {
		EnterQ(temp_1,current->info);
		current = current->next;
	}

	std::cout << "��ǰ�������: ";
	while (!empty(temp_1)) {
		std::cout << top(temp_1).carnumber << " ";
		OutQ(temp_1,useless);
	}
}

//------------  �˵�  ------------
void mune() {
	printf("���г���ͣ��ʱ���롰1��\n");
	printf("���г���ʻ��ʱ���롰2��\n");
	printf("�����ǰͣ����״̬��3��\n");
	printf("�����ǰ���״̬��4��\n");
	printf("����������0��\n");
}

//------------����ʵ��------------
int main() {
	SqStack Park;
	LinkQueue Biandao;
	InitStack(Park);
	InitQueue(Biandao);
	int choice;
	while (1) {
		mune();
		scanf_s("%d", &choice);
		switch (choice)
		{
		case 1:
		{
			ElemType e;
			std::cout << "���복���ĳ��ƺ�" << std::endl;
			std::cin >> e.carnumber;

			if (!EnterS(Park, e)) {
				EnterQ(Biandao, e);
				++baidao;
			}

			break;
		}
		case 2:
		{
			ElemType e;
			std::string cn;
			int sn;
			std::cout << "����ʻ�������ĳ��ƺţ�";
			std::cin >> cn;
			pop(Park, cn);
			OutQ(Biandao, e);
			EnterS(Park, e);
			break;
		}
		case 3:
			std::cout << "��ǰͣ��������: ";
			outputPark(Park);
			break;
		case 4:
			outputBianDao(Biandao, Park);
			break;
		case 0:
		{
			std::cout << "������ɡ�";
			return 0;
		}
		default:
			std::cout << "�������ָ�";
			break;
		}
	}
	return 0;
}