#include <iostream>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE (-1)
#define OVERFLOW (-2)

#define STACK_INIT_SIZE 100
#define STACK_INCREMENT 10

typedef int Status;
typedef int SElemType;

typedef struct {
	SElemType* base; 
	int evenNumTop;
	int oddNumTop;
	int stacksize;   
} SqStack;

//函数声明
Status InitSqStack(SqStack& S);
Status DestroySqStack(SqStack& S);
Status ClearStack(SqStack& S, int stackNum);
Status StackEmpty(SqStack S, int stackNum);
int StackLength(SqStack S, int stackNum);
Status GetTop(SqStack S, int stackNum, SElemType& e);
Status Push(SqStack& S, int stackNum, SElemType e);
Status Pop(SqStack& S, int stackNum, SElemType& e);
Status StackTraverse(SqStack S, int stackNum, Status(*visit)(SElemType));
Status visit(SElemType e);

//函数定义
// 函数定义
Status InitSqStack(SqStack& S) {
    S.base = new SElemType[STACK_INIT_SIZE];
    if (!S.base) {
        exit(OVERFLOW);
    }
    S.evenNumTop = 0;              // 栈1底部，向右增长
    S.oddNumTop = STACK_INIT_SIZE - 1; // 栈2底部，向左增长
    S.stacksize = STACK_INIT_SIZE;
    return OK;
}

Status DestroySqStack(SqStack& S) {
    if (S.base) {
        delete[] S.base;
        S.base = nullptr;
    }
    S.evenNumTop = 0;
    S.oddNumTop = 0;
    S.stacksize = 0;
    return OK;
}

Status ClearStack(SqStack& S, int stackNum) {
    if (stackNum == 1) {
        S.evenNumTop = 0;
    }
    else if (stackNum == 2) {
        S.oddNumTop = S.stacksize - 1;
    }
    return OK;
}

Status StackEmpty(SqStack S, int stackNum) {
    if (stackNum == 1) {
        return S.evenNumTop == 0 ? TRUE : FALSE;
    }
    else if (stackNum == 2) {
        return S.oddNumTop == S.stacksize - 1 ? TRUE : FALSE;
    }
    return ERROR;
}

int StackLength(SqStack S, int stackNum) {
    if (stackNum == 1) {
        return S.evenNumTop;
    }
    else if (stackNum == 2) {
        return S.stacksize - 1 - S.oddNumTop;
    }
    return 0;
}

Status GetTop(SqStack S, int stackNum, SElemType& e) {
    if (stackNum == 1) {
        if (S.evenNumTop == 0) {
            return ERROR; // 栈1空
        }
        e = S.base[S.evenNumTop - 1];
    }
    else if (stackNum == 2) {
        if (S.oddNumTop == S.stacksize - 1) {
            return ERROR; // 栈2空
        }
        e = S.base[S.oddNumTop + 1];
    }
    else {
        return ERROR;
    }
    return OK;
}

Status Push(SqStack& S, int stackNum, SElemType e) {
    if (S.evenNumTop > S.oddNumTop) {
        return OVERFLOW; // 栈已满
    }

    if (stackNum == 1) {
        S.base[S.evenNumTop++] = e;
    }
    else if (stackNum == 2) {
        S.base[S.oddNumTop--] = e;
    }
    else {
        return ERROR;
    }
    return OK;
}

Status Pop(SqStack& S, int stackNum, SElemType& e) {
    if (stackNum == 1) {
        if (S.evenNumTop == 0) {
            return ERROR; // 栈1空
        }
        e = S.base[--S.evenNumTop];
    }
    else if (stackNum == 2) {
        if (S.oddNumTop == S.stacksize - 1) {
            return ERROR; // 栈2空
        }
        e = S.base[++S.oddNumTop];
    }
    else {
        return ERROR;
    }
    return OK;
}

Status StackTraverse(SqStack S, int stackNum, Status(*visit)(SElemType)) {
    if (stackNum == 1) {
        for (int i = 0; i < S.evenNumTop; i++) {
            if (!visit(S.base[i])) {
                return ERROR;
            }
        }
    }
    else if (stackNum == 2) {
        for (int i = S.stacksize - 1; i > S.oddNumTop; i--) {
            if (!visit(S.base[i])) {
                return ERROR;
            }
        }
    }
    else {
        return ERROR;
    }
    return OK;
}

Status visit(SElemType e) {
    std::cout << e << " ";
    return OK;
}

int main() {
	SqStack numStack;
	InitSqStack(numStack);
	int n;
	std::cout << "请问需要输入多少个数？" ;
	std::cin >> n;
	for (int i = 0; i < n; i++) {
		int num;
		std::cout << "请输入第" << i + 1 << "个数：";
		std::cin >> num;
		if (num % 2 == 0) {
			Push(numStack, 1, num);
		}
		else {
			Push(numStack, 2, num);
		}
	}
	std::cout << "偶数栈的元素为：";
	StackTraverse(numStack, 1, visit);
	std::cout << std::endl;
	std::cout << "奇数栈的元素为：";
	StackTraverse(numStack, 2, visit);
	std::cout << std::endl;
	return 0;
}