#include <iostream>
#include <string>

#define TElemType std::string
#define OK 1

typedef int Status;

typedef struct BiTNode {
	TElemType data;
	struct BiTNode* lchild, * rchild; 

	// ���캯��
	BiTNode() : lchild(nullptr), rchild(nullptr) {}
}BiTNode,*BiTree;

typedef struct Stack {
	BiTree* data;
	int top;
	int maxSize;
}Stack;

Status CreateBitree(BiTree& T) {
	TElemType ch;
	std::cin >> ch;
	if (ch == "#") {
		T = NULL;
	}
	else {
		T = new BiTNode;
		T->data = ch;
		CreateBitree(T->lchild);
		CreateBitree(T->rchild);
	}
	return OK;
}

Status InitStack(Stack& S, int maxSize) {
	S.data = new BiTree[maxSize];
	if (!S.data) return 0;
	S.top = -1;
	S.maxSize = maxSize;
	return OK;
}

Status Push(Stack& S, BiTree T) {
	if (S.top == S.maxSize - 1) return 0; // Stack is full
	S.data[++S.top] = T;
	return OK;
}

Status Pop(Stack& S, BiTree& T) {
	if (S.top == -1) return 0; 
	T = S.data[S.top--];
	return OK;
}

Status InOrderTraverse(BiTree T) {
	Stack S;
	InitStack(S, 100); 
	BiTree p = T;
	while (p || S.top != -1) {
		while (p) {
			Push(S, p);
			p = p->lchild;
		}
		if (S.top != -1) {
			Pop(S, p);
			std::cout << p->data << " ";
			p = p->rchild;
		}
	}
	delete[] S.data; // �ͷ�ջ�ڴ�
	return OK;
}

int GetLeafNode(BiTree T) {
	if (T == NULL) return 0;

	if (T->lchild == NULL && T->rchild == NULL) {
		std::cout << T->data << " ";
		return 1;
	}

	return GetLeafNode(T->lchild) + GetLeafNode(T->rchild);
}

// ������ٶ������ĺ���
void DestroyTree(BiTree& T) {
	if (T == NULL) return;
	DestroyTree(T->lchild);
	DestroyTree(T->rchild);
	delete T;
	T = NULL;
}

void Menu() {
	std::cout << "****************************" << std::endl;
	std::cout << "1. �����ַ����У�������������" << std::endl;
	std::cout << "2. �ǵݹ��㷨�������" << std::endl;
	std::cout << "3. ��Ҷ�ӽ�����" << std::endl;
	std::cout << "4. �˳�" << std::endl;
	std::cout << "��ѡ��: ";
}

int main() {
	BiTree T = NULL;
	int choice;
	do {
		Menu();
		std::cin >> choice;
		switch (choice) {
			case 1:
				if (T != NULL) DestroyTree(T); // ���Ӿ�������
				std::cout << "�������ַ����У�#��ʾ�ս�㣩: ";
				CreateBitree(T);
				break;
			case 2:
				std::cout << "�ǵݹ�����������: ";
				InOrderTraverse(T);
				std::cout << std::endl;
				break;
			case 3:
			{
				std::cout << "Ҷ�ӽ��: ";
				int leafCount = GetLeafNode(T);
				std::cout << "\nҶ�ӽ�����: " << leafCount << std::endl;
				std::cout << std::endl;
			}
				break;
			case 4:
				std::cout << "�˳�����" << std::endl;
				break;
			default:
				std::cout << "��Чѡ������������" << std::endl;
		}
	} while (choice != 4);
	// �ͷŶ������ڴ�
	DestroyTree(T);
	return 0;
}


/* test
ѡ��1�����룺A B # D # # C # #
ѡ��2�����ӦΪ��B D A C
ѡ��3�����ӦΪ��D C��Ҷ�ӽ�����Ϊ2
*/