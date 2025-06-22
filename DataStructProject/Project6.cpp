/*
1、对直接插入排序和简单选择排序算法进行关键字比较次数和关键字移动次数的比较。
2、利用链式存储结构，编写程序，实现直接插入排序和冒泡排序。
*/

#define OK 1
#define FALSE 0

#include <iostream>


typedef struct Node {
	int data;
	struct Node* next;
}Node, LinkList;

int InitList(Node*& L) {
    L = new Node;
    if (!L) return FALSE;
    L->next = nullptr;
    return OK;
}

int InsrtList(Node* L, int e) {
    Node* s = new Node;
    if (!s) return FALSE;
    s->data = e;
    s->next = L->next;
    L->next = s;
    return OK;
}

int InsertSort(Node& L, int& compareCount, int& moveCount) {
    Node* sorted = nullptr; // 已排序链表的头指针
    Node* curr = L.next;    // 当前待插入节点
    compareCount = 0;
    moveCount = 0;

    while (curr != nullptr) {
        Node* next = curr->next; // 保存下一个待处理节点
        Node** p = &sorted;      // 指向已排序链表的指针

        // 在已排序链表中找到插入位置
        while (*p && (*p)->data < curr->data) {
            compareCount++;
            p = &((*p)->next);
        }
        if (*p) compareCount++; // 最后一次比较

        // 插入curr到已排序链表
        curr->next = *p;
        *p = curr;
        moveCount++;

        curr = next;
    }

    L.next = sorted; // 更新头结点指向新链表
    return OK;
}

int selectSort(Node& L, int& compareCount, int& moveCount) {
    Node dummy; 
    dummy.next = nullptr;
    Node* sortedTail = &dummy; // 已排序链表的尾部
    compareCount = 0;
    moveCount = 0;

    while (L.next != nullptr) {
        // 找到最小节点及其前驱
        Node* minPrev = &L;
        Node* prev = &L;
        Node* curr = L.next;
        while (curr != nullptr) {
            compareCount++;
            if (curr->data < minPrev->next->data) {
                minPrev = prev;
            }
            prev = curr;
            curr = curr->next;
        }
        // minPrev->next 是最小节点
        Node* minNode = minPrev->next;
        minPrev->next = minNode->next; // 从原链表摘除
        minNode->next = nullptr;
        sortedTail->next = minNode;    // 插入到已排序链表末尾
        sortedTail = minNode;
        moveCount++;
    }
    L.next = dummy.next; // 更新原链表头指针
    return OK;
}

int BubbleSort(Node& L, int& compareCount, int& moveCount) {
    compareCount = 0;
    moveCount = 0;
    if (!L.next || !L.next->next) return OK; 

    bool swapped;
    do {
        swapped = false;
        Node* prev = &L;
        Node* curr = L.next;
        Node* next = curr->next;
        while (next) {
            compareCount++;
            if (curr->data > next->data) {
                curr->next = next->next;
                next->next = curr;
                prev->next = next;
                moveCount++;
                swapped = true;
                prev = next;
                next = curr->next;
            }
            else {
                prev = curr;
                curr = next;
                next = next->next;
            }
        }
    } while (swapped);
    return OK;
}

void PrintList(const Node& L) {
    const Node* p = L.next;
    while (p) {
        std::cout << p->data << " ";
        p = p->next;
    }
    std::cout << std::endl;
}
// 深拷贝链表
Node* CopyList(const Node* src) {
    Node* newHead = new Node;
    newHead->next = nullptr;
    Node* tail = newHead;
    const Node* p = src->next;
    while (p) {
        Node* s = new Node;
        s->data = p->data;
        s->next = nullptr;
        tail->next = s;
        tail = s;
        p = p->next;
    }
    return newHead;
}

int main() {
    Node* TestList;
    InitList(TestList);

    InsrtList(TestList, 78);
    InsrtList(TestList, 45);
    InsrtList(TestList, 834);
    InsrtList(TestList, 213354);
    InsrtList(TestList, 3435);
    InsrtList(TestList, 1245);
    InsrtList(TestList, 24453);
    InsrtList(TestList, 254);
    InsrtList(TestList, 42678);
    InsrtList(TestList, 452678);
    InsrtList(TestList, 422245);

    Node* Test1 = CopyList(TestList);
    Node* Test2 = CopyList(TestList);
    Node* Test3 = CopyList(TestList);
    int compareCount1, compareCount2, compareCount3;
    int moveCount1, moveCount2, moveCount3;

    InsertSort(*Test1, compareCount1, moveCount1);
    selectSort(*Test2, compareCount2, moveCount2);
    BubbleSort(*Test3, compareCount3, moveCount3);

    std::cout << "直接插入排序结果: ";
    PrintList(*Test1);
    std::cout << "比较次数: " << compareCount1 << " 移动次数: " << moveCount1 << std::endl;

    std::cout << "简单选择排序结果: ";
    PrintList(*Test2);
    std::cout << "比较次数: " << compareCount2 << " 移动次数: " << moveCount2 << std::endl;

    std::cout << "冒泡排序结果: ";
    PrintList(*Test3);
    std::cout << "比较次数: " << compareCount3 << " 移动次数: " << moveCount3 << std::endl;

    return 0;
}