#include <iostream>

#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define INFEASIBLE (-1)
#define OVERFLOW (-2)

#define LIST_INIT_SIZE 100
#define LISTINCREMENT 10

typedef int Status;
typedef int ElemType;

typedef struct LNode {
	ElemType data;
	struct LNode* next;
} LNode, * LinkList;

//函数原型
Status InitList(LinkList& L);
Status ListEmpty(LinkList L);
Status ListLength(LinkList L);
Status GetElem(LinkList L, int i, ElemType& e);
Status ListInsert(LinkList& L, int i, ElemType e);
Status ListDelete(LinkList& L, int i, ElemType& e);
Status ListTraverse(LinkList L, void (*visit)(ElemType));
Status ClearList(LinkList& L);
Status DestroyList(LinkList& L);
Status ReverseList(LinkList& L);
Status MergeLists1(LinkList La, LinkList Lb,LinkList& Lc);
Status MergeLists2(LinkList La, LinkList Lb,LinkList& Lc);



void visit(ElemType e) {
	std::cout << e << " ";
}

Status InitList(LinkList& L) {
	L = (LinkList)malloc(sizeof(LNode));
	if (!L) {
		return OVERFLOW;
	}
	L->next = NULL;
	return OK;
}

Status ListEmpty(LinkList L) {
	return L->next == NULL ? TRUE : FALSE;
}

Status ListLength(LinkList L) {
	int length = 0;
	LNode* p = L->next;
	while (p) {
		length++;
		p = p->next;
	}
	return length;
}

Status GetElem(LinkList L, int i, ElemType& e) {
	if (i < 1) {
		return ERROR;
	}
	LNode* p = L->next;
	int j = 1;
	while (p && j < i) {
		p = p->next;
		j++;
	}
	if (!p || j > i) {
		return ERROR;
	}
	e = p->data;
	return OK;
}

Status ListInsert(LinkList& L, int i, ElemType e) {
	if (i < 1) {
		return ERROR;
	}
	LNode* p = L;
	int j = 0;
	while (p && j < i - 1) {
		p = p->next;
		j++;
	}
	if (!p || j > i - 1) {
		return ERROR;
	}
	LNode* s = (LNode*)malloc(sizeof(LNode));
	if (!s) {
		return OVERFLOW;
	}
	s->data = e;
	s->next = p->next;
	p->next = s;
	return OK;
}

Status ListDelete(LinkList& L, int i, ElemType& e) {
	if (i < 1) {
		return ERROR;
	}
	LNode* p = L;
	int j = 0;
	while (p->next && j < i - 1) {
		p = p->next;
		j++;
	}
	if (!p->next || j > i - 1) {
		return ERROR;
	}
	LNode* q = p->next;
	e = q->data;
	p->next = q->next;
	free(q);
	return OK;
}

Status ListTraverse(LinkList L, void (*visit)(ElemType)) {
	LNode* p = L->next;
	while (p) {
		visit(p->data);
		p = p->next;
	}
	std::cout << std::endl;
	return OK;
}

Status ClearList(LinkList& L) {
	LNode* p = L->next;
	while (p) {
		LNode* q = p;
		p = p->next;
		free(q);
	}
	L->next = NULL;
	return OK;
}

Status DestroyList(LinkList& L) {
	ClearList(L);
	free(L);
	L = NULL;
	return OK;
}

//就地逆置
Status ReverseList(LinkList& L) {
	LNode* prev = NULL;
	LNode* curr = L->next;
	LNode* next = NULL;
	while (curr) {
		next = curr->next; //保存下一个节点
		curr->next = prev; //反转当前节点的指针
		prev = curr; //移动prev到当前节点
		curr = next; //移动curr到下一个节点
	}
	L->next = prev; //更新头指针
	return OK;
}

//两个非递减合并为一个非递减链表
Status MergeLists1(LinkList La, LinkList Lb,LinkList& Lc) {
	LNode* pa = La->next;
	LNode* pb = Lb->next;
	LNode* pc = Lc; //合并后的链表头指针
	pc->next = NULL; //初始化合并链表为空
	while (pa && pb) {
		if (pa->data <= pb->data) {
			pc->next = pa;
			pc = pc->next;
			pa = pa->next;
		}
		else {
			pc->next = pb;
			pc = pc->next;
			pb = pb->next;
		}
	}
	if (pa) {
		pc->next = pa;
	}
	else {
		pc->next = pb;
	}
	Lb->next = NULL; //清空Lb
	return OK;
}


//两个非递减合并为一个非递增链表
Status MergeLists2(LinkList La, LinkList Lb, LinkList& Lc) {
	LNode* pa = La->next;
	LNode* pb = Lb->next;
	LNode* pc = Lc; //合并后的链表头指针
	pc->next = NULL; //初始化合并链表为空
	while (pa && pb) {
		if (pa->data >= pb->data) {
			pc->next = pa;
			pc = pc->next;
			pa = pa->next;
		}
		else {
			pc->next = pb;
			pc = pc->next;
			pb = pb->next;
		}
	}
	if (pa) {
		pc->next = pa;
	}
	else {
		pc->next = pb;
	}
	Lb->next = NULL; //清空Lb
	return OK;
}


int main() {
	LinkList L1, L2, L3, L4, L5;
	InitList(L1);//递增
	InitList(L2);//递减
	InitList(L3);//递增
	InitList(L4);
	InitList(L5);
	std::cout << "L1和L2的初始状态为：";
	for (int i = 1; i <= 5; i++) {
		ListInsert(L1, i, i);
	}
	for (int i = 1; i <= 5; i++) {
		ListInsert(L2, i, 6 - i);
	}
	for (int i = 1; i <= 10; i++) {
		ListInsert(L3, i, i);
	}
	ListTraverse(L1, visit);
	ListTraverse(L2, visit);
	std::cout << "L2就地逆置后为：";
	ReverseList(L2);
	ListTraverse(L2, visit);
	std::cout << "L2再次就地逆置后为：";
	ReverseList(L2);
	ListTraverse(L2, visit);
	std::cout << "L1和L3合并为L4（两个非递减合为非递减）：";
	MergeLists1(L1, L3, L4);
	ListTraverse(L4, visit);
	std::cout << "L1和L3合并为L5（两个非递减合为非递增）：";
	MergeLists2(L1, L3,L5);
	ListTraverse(L5, visit);
}