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

int main() {
    Node* TestList;
    InitList(TestList);

    InsrtList(TestList, 78);
    InsrtList(TestList, 45);
    InsrtList(TestList, 834);
    InsrtList(TestList, 213354);
    InsrtList(TestList, 3435);
    InsrtList(TestList, 01245);
    InsrtList(TestList, 24453);
    InsrtList(TestList, 254);
    InsrtList(TestList, 42678);
    InsrtList(TestList, 452678);
    InsrtList(TestList, 422245);

    Node Test1 = *TestList;
    Node Test2 = *TestList;
    int compareCount1, compareCount2;
    int moveCount1, moveCount2;

    InsertSort(Test1, compareCount1, moveCount1);
    selectSort(Test2, compareCount2, moveCount2);

    std::cout << "直接插入排序比较次数和关键字移动次数:" << compareCount1 << moveCount1 << std::endl;
    std::cout << "简单选择排序比较次数和关键字移动次数:" << compareCount2 << moveCount2;

    return 0;
}