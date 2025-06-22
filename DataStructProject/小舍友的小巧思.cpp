// 停车场.cpp : 
#include<stdio.h>
#include <iostream>
#include<stdlib.h>
#include<string>
#include <stack>
#include <queue>

//定义宏常量
typedef int Status;
#define OK 1
#define ERROR 0

int count = 0;
int baidao = 0;

//定义分配空间初始量
#define STACK_INIT_SIZE 5


//定义ElemType类型
typedef struct {
	std::string carnumber;
	int seatnumber;
}ElemType;


//------------栈的顺序存储表示------------

//栈存储结构的定义
typedef struct Stack {
	ElemType* top;
	ElemType* base;
	int stacksize = STACK_INIT_SIZE;
}SqStack;
//构造空栈（建立停车场）
Status InitStack(SqStack& S) {
	S.base = new ElemType[5];
	if (!S.base) exit(ERROR);
	S.top = S.base;
	return OK;
}
//栈的压入（汽车进入停车场）
Status EnterS(SqStack& S, ElemType e) {
	auto temp = S.top - 1;
	if (count >= S.stacksize) {  
		printf("停车场已满，该车将暂时停放到便道上等候/n");
		return ERROR;
	}
	*S.top = e;
	S.top++;
	count++;
	return OK;
}



Status Pop(SqStack& S, ElemType& e) { // Fixed: e is now passed by reference
	if (S.top == S.base) return ERROR; // 栈空
	S.top--; // 栈顶指针后移
	e = *(S.top); // 获取栈顶元素
	count--;
	return OK;
}

void pop(SqStack& S, std::string car_num) {
	int current_count = S.top - S.base;
	bool found = false;
	std::stack<ElemType> temp;
	std::stack<ElemType> temp2;

	// 先把所有元素弹出到临时栈
	for (int i = 0; i < current_count; i++) {
		temp.push(S.base[i]);
	}

	for (int i = 0; i < current_count; i++) {
		temp2.push(temp.top());
		temp.pop();
	}

	// 重新入栈，跳过要删除的车辆
	S.top = S.base; // 重置栈顶
	while (!temp2.empty()) {
		ElemType e = temp2.top();
		temp2.pop();
		if (e.carnumber == car_num && !found) {
			found = true; // 跳过该元素
			count--;
			continue;
		}
		*S.top = e;
		S.top++;
	}

	if (!found) {
		std::cout << "未找到该车牌号车辆！" << std::endl;
	}
	else {
		std::cout << "车辆" << car_num << "已驶出。" << std::endl;
	}
}



//------------队列的存储表示------------

//队列存储结构的定义
typedef struct QNode {
	ElemType info;
	struct QNode* next;
}QNode, * QueuePtr;
typedef struct {
	QueuePtr front;
	QueuePtr rear;
}LinkQueue;
//构造空队列（建立便道）
Status InitQueue(LinkQueue& Q) {
	Q.front = Q.rear = new QNode;
	if (!Q.front) exit(OVERFLOW);
	Q.front->next = NULL;
	return OK;
}
//插入元素在队尾（新来的车辆在便道上排队）
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
//队列的输出（第一个车辆进入停车场）
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

void outputPark(SqStack& S) { // 参数改为引用
	int current_count = S.top - S.base;
	std::stack<ElemType> temp;
	ElemType e;

	// 保存栈元素到临时栈
	SqStack tempS = S;
	for (int i = 0; i < current_count; i++) {
		temp.push(tempS.base[i]);
	}

	// 输出并还原
	std::cout << "当前停车场车辆: ";
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

    std::cout << "当前变道车辆: ";
    while (!temp.empty()) {
        std::cout << temp.front().carnumber << " ";
        temp.pop();
    }
}

//------------  菜单  ------------
void mune() {
	printf("当有车辆停车时输入“1”\n");
	printf("当有车辆驶出时输入“2”\n");
	printf("输出当前停车场状态“3”\n");
	printf("输出当前便道状态“4”\n");
	printf("结束操作“0”\n");
}

//------------功能实现------------
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
			std::cout << "输入车辆的车牌号和次序" << std::endl;
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
			std::cout << "输入驶出车辆的车牌号：";
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
			std::cout << "操作完成。";
			return 0;
		}
		default:
			std::cout << "输入错误指令。";
			break;
		}
	}
	return 0;
}