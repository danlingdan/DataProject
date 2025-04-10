#include <iostream>
#include <string>
#include <cstdlib> // Ϊ��ʹ�� system() ����
#include <limits> // ������ͷ�ļ����� numeric_limits

using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2


struct Student {
    int number = 0;
    char name[100];
    int age = 0;
    char gender[100];
};


typedef int Status;
typedef Student ElemType;

#define LIST_INIT_SIZE 100
#define LISTINCREMENT 10

typedef struct StudentNode {
    ElemType data;
    struct StudentNode* next;
} StudentNode, * StudentList;

// ���ڸ��ɿ���������뻺�����ĺ���
void clearInputBuffer() {
    cin.clear(); // �������״̬
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ���Ի���������������ֱ���������з�
}

// ��ʼ��������
Status CreateEmptyList(StudentList& L) {
    // ����ͷ�ڵ�
    L = (StudentNode*)malloc(sizeof(StudentNode));
    if (!L) exit(OVERFLOW);
    L->next = NULL;
    return OK;
}

//����ѧ����
Status InitList_L(StudentList& L) {
    int n;
    cout << "������ѧ������: ";
    cin >> n;
    clearInputBuffer(); // ��� cin.ignore()�����ɿ������������

    if (n <= 0) return ERROR;
    StudentList p = L;

    for (int i = 0; i < n; i++) {
        StudentNode* s = (StudentNode*)malloc(sizeof(StudentNode));
        if (!s) exit(OVERFLOW);

        std::cout << "������� " << i + 1 << "��ѧ������Ϣ" << std::endl;

        std::cout << "ѧ�ţ�";
        std::cin >> s->data.number;
        clearInputBuffer();

        std::cout << "���䣺";
        std::cin >> s->data.age;
        clearInputBuffer();

        std::cout << "������";
        std::cin.getline(s->data.name, 100);

        std::cout << "�Ա�";
        std::cin.getline(s->data.gender, 100);

        s->next = NULL;
        p->next = s;
        p = s;
    }
    cout << "ѧ�������ɹ���" << endl;
    return OK;
}

// �ж�ѧ����Ϣ�б��Ƿ�Ϊ��
Status ListEmpty(StudentList L) {
    return (L->next == NULL) ? TRUE : FALSE;
}

Status SearchStudentById(StudentList L, int number) {
    if (ListEmpty(L)) {
        std::cout << "ѧ����Ϣ�б�Ϊ�գ�" << std::endl;
        return ERROR;
    }

    StudentList p = L->next;
    while (p && p->data.number != number) {
        p = p->next;
    }

    if (p) {
        std::cout << "�ҵ�ѧ��Ϊ " << number << " ��ѧ����" << std::endl;
        std::cout << "ѧ��\t����\t����\t�Ա�" << std::endl;
        std::cout << p->data.number << "\t"
            << p->data.name << "\t"
            << p->data.age << "\t"
            << p->data.gender << std::endl;
        return OK;
    }
    else {
        std::cout << "δ�ҵ�ѧ��Ϊ " << number << " ��ѧ����" << std::endl;
        return ERROR;
    }
}


//�������Ա�
Status DestroyList_L(StudentList& L) {
    StudentList p;
    while (L) {
        p = L;
        L = L->next;
        free(p);
    }
    return OK;
}


//���ѧ����Ϣ
Status AddStudent(StudentList& L) {
    StudentList s = (StudentList)malloc(sizeof(StudentNode));
    if (!s) exit(OVERFLOW);

    std::cout << "������Ҫ��ӵ�ѧ����Ϣ��" << std::endl;

    std::cout << "ѧ�ţ�";
    std::cin >> s->data.number;
    clearInputBuffer();

    std::cout << "���䣺";
    std::cin >> s->data.age;
    clearInputBuffer();

    std::cout << "������";
    std::cin.getline(s->data.name, 100);

    std::cout << "�Ա�";
    std::cin.getline(s->data.gender, 100);

    // β�巨���ѧ��
    StudentList p = L;
    while (p->next) p = p->next;
    s->next = NULL;
    p->next = s;

    std::cout << "ѧ����Ϣ��ӳɹ���" << std::endl;
    return OK;
}


//ɾ��Ԫ��
Status ListDelete_L(StudentList& L, int number) {
    StudentList p = L;
    int found = 0;

    while (p->next) {
        if (p->next->data.number == number) {
            StudentList q = p->next;
            p->next = q->next;
            free(q);
            found = 1;
            break;
        }
        p = p->next;
    }

    if (found) {
        std::cout << "�ɹ�ɾ��ѧ��Ϊ " << number << " ��ѧ����Ϣ��" << std::endl;
        return OK;
    }
    else {
        std::cout << "δ�ҵ�ѧ��Ϊ " << number << " ��ѧ����ɾ��ʧ�ܣ�" << std::endl;
        return ERROR;
    }
}

//����
Status ListTraverse_L(StudentList L, Status(*visit)(ElemType)) {
    if (ListEmpty(L)) {
        std::cout << "ѧ����Ϣ�б�Ϊ�գ�" << std::endl;
        return ERROR;
    }

    StudentList p = L->next;
    while (p) {
        visit(p->data);
        p = p->next;
    }
    return OK;
}

//���ʺ���
Status visit(ElemType e) {
    cout << "ѧ�ţ�" << e.number << "\t������" << e.name << "\t���䣺" << e.age << "\t�Ա�" << e.gender << endl;
    return OK;
}

// ��������
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

//�����溯��
void Menu() {
    cout << "****************************************" << endl;
    cout << "*           ѧ����Ϣ����ϵͳ           *" << endl;
    cout << "****************************************" << endl;
    cout << "1.����ѧ����" << endl;
    cout << "2.���ѧ����Ϣ" << endl;
    cout << "3.ɾ��ѧ����Ϣ" << endl;
    cout << "4.�鿴����ѧ����Ϣ" << endl;
    cout << "5.����ѧ�Ų�ѯĳ��ѧ��" << endl;
    cout << "0.�˳������ٱ�" << endl;
    cout << "���������ѡ��: ";
}

int main() {
    int choice;
    Student student;
    StudentList studentList = NULL; // ��ʼ��ΪNULL
    CreateEmptyList(studentList);

    while (true) { // ��Ϊ����ѭ������case 0�������˳�
        clearScreen(); // ����
        Menu();
        cin >> choice;
        clearInputBuffer(); // ʹ�ø��ɿ��ķ�ʽ���������

        switch (choice) {
        case 1:
            clearScreen(); // ����
            // ����Ѵ���������������
            if (studentList) {
                DestroyList_L(studentList);
                CreateEmptyList(studentList);
            }
            InitList_L(studentList);
            cout << "��Enter������...";
            cin.get(); // �ȴ��û�����
            break;
        case 2:
            clearScreen(); // ����
            AddStudent(studentList);
            cout << "��Enter������...";
            cin.get(); // �ȴ��û�����
            break;
        case 3:
            clearScreen(); // ����
            cout << "������Ҫɾ����ѧ��ѧ��: ";
            cin >> student.number;
            clearInputBuffer();
            ListDelete_L(studentList, student.number);
            cout << "��Enter������...";
            cin.get(); // �ȴ��û�����
            break;
        case 4:
            clearScreen(); // ����
            cout << "ѧ����Ϣ�б�" << endl;
            cout << "----------------------------" << endl;
            ListTraverse_L(studentList, visit);
            cout << "��Enter������...";
            cin.get(); // �ȴ��û�����
            break;
        case 5:
            clearScreen(); // ����
            int id;
            cout << "������Ҫ��ѯ��ѧ��ѧ��: ";
            cin >> id;
            clearInputBuffer();
            SearchStudentById(studentList, id);
            cout << "��Enter������...";
            cin.get(); // �ȴ��û�����
            break;
        case 0:
            if (studentList) {
                DestroyList_L(studentList);
            }
            clearScreen(); // ����
            cout << "�˳��ɹ���" << endl;
            return 0;
        default:
            cout << "��Ч��ѡ�����������룡" << endl;
            cout << "��Enter������...";
            cin.get(); // �ȴ��û�����
        }
    }
    return 0;
}