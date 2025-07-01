// 头文件
#include<stdio.h>
#include <iostream>
#include<stdlib.h>
#include<string>

// 定义全局变量
// 
//定义宏常量
#define OK 1
#define ERROR 0

// 定义变量
typedef int Status;
int count = 0; // 记录停车场中车辆的数量
int baidao = 0; // 记录便道中车辆的数量

//定义分配空间初始量
#define STACK_INIT_SIZE 1

//定义ElemType类型
typedef struct {
	std::string carnumber;
}ElemType;

//栈存储结构的定义
typedef struct Stack {
	ElemType* top;
	ElemType* base;
	int stacksize = STACK_INIT_SIZE;
}SqStack;

//队列存储结构的定义
typedef struct QNode {
	ElemType info;
	struct QNode* next;
}QNode, * QueuePtr;
typedef struct {
	QueuePtr front;
	QueuePtr rear;
}LinkQueue;

// 函数声明
Status InitStack(SqStack& S); // 初始化栈
Status SearchSq(SqStack& S, ElemType e); // 查找栈中是否存在某个元素
Status EnterS(SqStack& S, ElemType e); // 压入栈（汽车进入停车场）
Status Pop(SqStack& S, ElemType& e); // 弹出栈（汽车驶出停车场）
void pop(SqStack& S, std::string car_num); // 从栈中删除指定车辆
Status InitQueue(LinkQueue& Q); // 初始化队列
Status EnterQ(LinkQueue& Q, ElemType e); // 入队（新来的车辆在便道上排队）
Status OutQ(LinkQueue& Q, ElemType& e); // 出队（第一个车辆进入停车场）
void outputPark(SqStack& S); // 输出停车场状态
void outputBianDao(LinkQueue& p, SqStack s); // 输出便道状态
auto top(SqStack S); // 获取栈顶元素
bool empty(SqStack S); // 判断栈是否为空
void mune(); // 显示菜单
bool empty(LinkQueue Q);

// 函数实现

//初始化空栈（建立停车场）
Status InitStack(SqStack& S) {
	S.base = new ElemType[5];
	if (!S.base) exit(ERROR);
	S.top = S.base;
	return OK;
}
//查找
Status SearchSq(SqStack& S, ElemType e) {
	for (int i = 0; i < count; i++) {
		if (S.base[i].carnumber == e.carnumber) return 1;
	}
	return 0;
}
//栈的压入（汽车进入停车场）
Status EnterS(SqStack& S, ElemType e) {
	if (count > 1) {
		if (SearchSq(S, e))
		{
			std::cout << "车牌号输入错误（该车牌号已进停车场或便道，且未出来）" << std::endl;
			return ERROR;
		}
	}
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
auto top(SqStack S) {
	auto e = *(S.top-1);
	return e;
}
bool empty(SqStack S) {
	if (S.base == S.top) { return true; }
	else { return false; }
}
//压入到栈顶
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
	// 先把所有元素弹出到临时栈
	for (int i = 0; i < current_count; i++) {
		EnterS(temp_1, S.base[i]);
	}
	for (int i = 0; i < current_count; i++) {
		EnterS(temp_2, top(temp_1));
		Pop(temp_2, useless);
	}
	// 重新入栈，跳过要删除的车辆
	S.top = S.base; // 重置栈顶
	while (!empty(temp_2)) {
		ElemType e = top(temp_2);
		Pop(temp_2, useless);
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
	delete p;
	return OK;
}
void outputPark(SqStack& S) { // 参数改为引用
	int current_count = S.top - S.base;
	LinkQueue temp_1;
	InitQueue(temp_1);
	ElemType e;
	ElemType useless;

	// 保存栈元素到临时栈
	SqStack tempS = S;
	for (int i = 0; i < current_count; i++) {
		EnterQ(temp_1, tempS.base[i]);
	}

	// 输出并还原
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

	std::cout << "当前变道车辆: ";
	while (!empty(temp_1)) {
		std::cout << top(temp_1).carnumber << " ";
		OutQ(temp_1,useless);
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
			std::cout << "输入车辆的车牌号" << std::endl;
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
			pop(Park, cn);
			OutQ(Biandao, e);
			EnterS(Park, e);
			break;
		}
		case 3:
			std::cout << "当前停车场车辆: ";
			outputPark(Park);
			break;
		case 4:
			outputBianDao(Biandao, Park);
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