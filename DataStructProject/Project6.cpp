/*
1����ֱ�Ӳ�������ͼ�ѡ�������㷨���йؼ��ֱȽϴ����͹ؼ����ƶ������ıȽϡ�
2��������ʽ�洢�ṹ����д����ʵ��ֱ�Ӳ��������ð������
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
    Node* sorted = nullptr; // �����������ͷָ��
    Node* curr = L.next;    // ��ǰ������ڵ�
    compareCount = 0;
    moveCount = 0;

    while (curr != nullptr) {
        Node* next = curr->next; // ������һ��������ڵ�
        Node** p = &sorted;      // ָ�������������ָ��

        // ���������������ҵ�����λ��
        while (*p && (*p)->data < curr->data) {
            compareCount++;
            p = &((*p)->next);
        }
        if (*p) compareCount++; // ���һ�αȽ�

        // ����curr������������
        curr->next = *p;
        *p = curr;
        moveCount++;

        curr = next;
    }

    L.next = sorted; // ����ͷ���ָ��������
    return OK;
}

int selectSort(Node& L, int& compareCount, int& moveCount) {
    Node dummy; 
    dummy.next = nullptr;
    Node* sortedTail = &dummy; // �����������β��
    compareCount = 0;
    moveCount = 0;

    while (L.next != nullptr) {
        // �ҵ���С�ڵ㼰��ǰ��
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
        // minPrev->next ����С�ڵ�
        Node* minNode = minPrev->next;
        minPrev->next = minNode->next; // ��ԭ����ժ��
        minNode->next = nullptr;
        sortedTail->next = minNode;    // ���뵽����������ĩβ
        sortedTail = minNode;
        moveCount++;
    }
    L.next = dummy.next; // ����ԭ����ͷָ��
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

    std::cout << "ֱ�Ӳ�������Ƚϴ����͹ؼ����ƶ�����:" << compareCount1 << moveCount1 << std::endl;
    std::cout << "��ѡ������Ƚϴ����͹ؼ����ƶ�����:" << compareCount2 << moveCount2;

    return 0;
}