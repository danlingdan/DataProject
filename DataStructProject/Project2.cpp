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

//����ԭ��
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

//�͵�����
Status ReverseList(LinkList& L) {
	LNode* prev = NULL;
	LNode* curr = L->next;
	LNode* next = NULL;
	while (curr) {
		next = curr->next; //������һ���ڵ�
		curr->next = prev; //��ת��ǰ�ڵ��ָ��
		prev = curr; //�ƶ�prev����ǰ�ڵ�
		curr = next; //�ƶ�curr����һ���ڵ�
	}
	L->next = prev; //����ͷָ��
	return OK;
}

//�����ǵݼ��ϲ�Ϊһ���ǵݼ�����
Status MergeLists1(LinkList La, LinkList Lb,LinkList& Lc) {
	LNode* pa = La->next;
	LNode* pb = Lb->next;
	LNode* pc = Lc; //�ϲ��������ͷָ��
	pc->next = NULL; //��ʼ���ϲ�����Ϊ��
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
	Lb->next = NULL; //���Lb
	return OK;
}


//�����ǵݼ��ϲ�Ϊһ���ǵ�������
Status MergeLists2(LinkList La, LinkList Lb, LinkList& Lc) {
	LNode* pa = La->next;
	LNode* pb = Lb->next;
	LNode* pc = Lc; //�ϲ��������ͷָ��
	pc->next = NULL; //��ʼ���ϲ�����Ϊ��
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
	Lb->next = NULL; //���Lb
	return OK;
}


int main() {
	LinkList L1, L2, L3, L4, L5;
	InitList(L1);//����
	InitList(L2);//�ݼ�
	InitList(L3);//����
	InitList(L4);
	InitList(L5);
	std::cout << "L1��L2�ĳ�ʼ״̬Ϊ��";
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
	std::cout << "L2�͵����ú�Ϊ��";
	ReverseList(L2);
	ListTraverse(L2, visit);
	std::cout << "L2�ٴξ͵����ú�Ϊ��";
	ReverseList(L2);
	ListTraverse(L2, visit);
	std::cout << "L1��L3�ϲ�ΪL4�������ǵݼ���Ϊ�ǵݼ�����";
	MergeLists1(L1, L3, L4);
	ListTraverse(L4, visit);
	std::cout << "L1��L3�ϲ�ΪL5�������ǵݼ���Ϊ�ǵ�������";
	MergeLists2(L1, L3,L5);
	ListTraverse(L5, visit);
}