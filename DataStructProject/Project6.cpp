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
// �������
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

    std::cout << "ֱ�Ӳ���������: ";
    PrintList(*Test1);
    std::cout << "�Ƚϴ���: " << compareCount1 << " �ƶ�����: " << moveCount1 << std::endl;

    std::cout << "��ѡ��������: ";
    PrintList(*Test2);
    std::cout << "�Ƚϴ���: " << compareCount2 << " �ƶ�����: " << moveCount2 << std::endl;

    std::cout << "ð��������: ";
    PrintList(*Test3);
    std::cout << "�Ƚϴ���: " << compareCount3 << " �ƶ�����: " << moveCount3 << std::endl;

    return 0;
}