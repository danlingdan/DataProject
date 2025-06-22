// ͣ����.cpp : 
#include<stdio.h>
#include <iostream>
#include<stdlib.h>
#include<string>
#include <stack>
#include <queue>

//����곣��
typedef int Status;
#define OK 1
#define ERROR 0

int count = 0;
int baidao = 0;

//�������ռ��ʼ��
#define STACK_INIT_SIZE 5


//����ElemType����
typedef struct {
	std::string carnumber;
	int seatnumber;
}ElemType;


//------------ջ��˳��洢��ʾ------------

//ջ�洢�ṹ�Ķ���
typedef struct Stack {
	ElemType* top;
	ElemType* base;
	int stacksize = STACK_INIT_SIZE;
}SqStack;
//�����ջ������ͣ������
Status InitStack(SqStack& S) {
	S.base = new ElemType[5];
	if (!S.base) exit(ERROR);
	S.top = S.base;
	return OK;
}
//ջ��ѹ�루��������ͣ������
Status EnterS(SqStack& S, ElemType e) {
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



Status Pop(SqStack& S, ElemType& e) { // Fixed: e is now passed by reference
	if (S.top == S.base) return ERROR; // ջ��
	S.top--; // ջ��ָ�����
	e = *(S.top); // ��ȡջ��Ԫ��
	count--;
	return OK;
}

void pop(SqStack& S, std::string car_num) {
	int current_count = S.top - S.base;
	bool found = false;
	std::stack<ElemType> temp;
	std::stack<ElemType> temp2;

	// �Ȱ�����Ԫ�ص�������ʱջ
	for (int i = 0; i < current_count; i++) {
		temp.push(S.base[i]);
	}

	for (int i = 0; i < current_count; i++) {
		temp2.push(temp.top());
		temp.pop();
	}

	// ������ջ������Ҫɾ���ĳ���
	S.top = S.base; // ����ջ��
	while (!temp2.empty()) {
		ElemType e = temp2.top();
		temp2.pop();
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



//------------���еĴ洢��ʾ------------

//���д洢�ṹ�Ķ���
typedef struct QNode {
	ElemType info;
	struct QNode* next;
}QNode, * QueuePtr;
typedef struct {
	QueuePtr front;
	QueuePtr rear;
}LinkQueue;
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
	std::cout << p->info.carnumber;
	p->next = NULL;
	Q.rear->next = p;
	Q.rear = p;
	return OK;
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
	free(p);
	return OK;
}

void outputPark(SqStack& S) { // ������Ϊ����
	int current_count = S.top - S.base;
	std::stack<ElemType> temp;
	ElemType e;

	// ����ջԪ�ص���ʱջ
	SqStack tempS = S;
	for (int i = 0; i < current_count; i++) {
		temp.push(tempS.base[i]);
	}

	// �������ԭ
	std::cout << "��ǰͣ��������: ";
	while (!temp.empty()) {
		std::cout << temp.top().carnumber << " ";
		temp.pop();
	}
	std::cout << std::endl;
}

void outputBianDao(LinkQueue& p, SqStack s) {
    std::queue<ElemType> temp;
    ElemType e;

    LinkQueue tempQ = p;
    QNode* current = tempQ.front->next; 
    for (int i = 0; i < baidao && current != nullptr; i++) {
        temp.push(current->info);
        current = current->next; 
    }

    std::cout << "��ǰ�������: ";
    while (!temp.empty()) {
        std::cout << temp.front().carnumber << " ";
        temp.pop();
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
			e.seatnumber = count+1;
			std::cout << "���복���ĳ��ƺźʹ���" << std::endl;
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
			pop(Park,cn);
			OutQ(Biandao, e);
			EnterS(Park, e);
			break;
		}
		case 3:
			outputPark(Park);
			break;
		case 4:
			outputBianDao(Biandao,Park);
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