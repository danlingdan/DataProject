#include <iostream>
#include <string>

#define TElemType std::string
#define OK 1

typedef int Status;

typedef struct BiTNode {
	TElemType data;
	struct BiTNode* lchild, * rchild; 

	// 构造函数
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
	delete[] S.data; // 释放栈内存
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

// 添加销毁二叉树的函数
void DestroyTree(BiTree& T) {
	if (T == NULL) return;
	DestroyTree(T->lchild);
	DestroyTree(T->rchild);
	delete T;
	T = NULL;
}

void Menu() {
	std::cout << "****************************" << std::endl;
	std::cout << "1. 输入字符序列，建立二叉链表" << std::endl;
	std::cout << "2. 非递归算法中序遍历" << std::endl;
	std::cout << "3. 求叶子结点个数" << std::endl;
	std::cout << "4. 退出" << std::endl;
	std::cout << "请选择: ";
}

int main() {
	BiTree T = NULL;
	int choice;
	do {
		Menu();
		std::cin >> choice;
		switch (choice) {
			case 1:
				if (T != NULL) DestroyTree(T); // 增加旧树销毁
				std::cout << "请输入字符序列（#表示空结点）: ";
				CreateBitree(T);
				break;
			case 2:
				std::cout << "非递归中序遍历结果: ";
				InOrderTraverse(T);
				std::cout << std::endl;
				break;
			case 3:
			{
				std::cout << "叶子结点: ";
				int leafCount = GetLeafNode(T);
				std::cout << "\n叶子结点个数: " << leafCount << std::endl;
				std::cout << std::endl;
			}
				break;
			case 4:
				std::cout << "退出程序" << std::endl;
				break;
			default:
				std::cout << "无效选择，请重新输入" << std::endl;
		}
	} while (choice != 4);
	// 释放二叉树内存
	DestroyTree(T);
	return 0;
}


/* test
选择1，输入：A B # D # # C # #
选择2，输出应为：B D A C
选择3，输出应为：D C，叶子结点个数为2
*/