#include <iostream>
#include <string>
#include <cstdlib> // 为了使用 system() 函数
#include <limits> // 添加这个头文件用于 numeric_limits

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

// 用于更可靠地清除输入缓冲区的函数
void clearInputBuffer() {
    cin.clear(); // 清除错误状态
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 忽略缓冲区中所有内容直到遇到换行符
}

// 初始化空链表
Status CreateEmptyList(StudentList& L) {
    // 创建头节点
    L = (StudentNode*)malloc(sizeof(StudentNode));
    if (!L) exit(OVERFLOW);
    L->next = NULL;
    return OK;
}

//创建学生表
Status InitList_L(StudentList& L) {
    int n;
    cout << "请输入学生人数: ";
    cin >> n;
    clearInputBuffer(); // 替代 cin.ignore()，更可靠地清除缓冲区

    if (n <= 0) return ERROR;
    StudentList p = L;

    for (int i = 0; i < n; i++) {
        StudentNode* s = (StudentNode*)malloc(sizeof(StudentNode));
        if (!s) exit(OVERFLOW);

        std::cout << "请输入第 " << i + 1 << "个学生的信息" << std::endl;

        std::cout << "学号：";
        std::cin >> s->data.number;
        clearInputBuffer();

        std::cout << "年龄：";
        std::cin >> s->data.age;
        clearInputBuffer();

        std::cout << "姓名：";
        std::cin.getline(s->data.name, 100);

        std::cout << "性别：";
        std::cin.getline(s->data.gender, 100);

        s->next = NULL;
        p->next = s;
        p = s;
    }
    cout << "学生表创建成功！" << endl;
    return OK;
}

// 判断学生信息列表是否为空
Status ListEmpty(StudentList L) {
    return (L->next == NULL) ? TRUE : FALSE;
}

Status SearchStudentById(StudentList L, int number) {
    if (ListEmpty(L)) {
        std::cout << "学生信息列表为空！" << std::endl;
        return ERROR;
    }

    StudentList p = L->next;
    while (p && p->data.number != number) {
        p = p->next;
    }

    if (p) {
        std::cout << "找到学号为 " << number << " 的学生：" << std::endl;
        std::cout << "学号\t姓名\t年龄\t性别" << std::endl;
        std::cout << p->data.number << "\t"
            << p->data.name << "\t"
            << p->data.age << "\t"
            << p->data.gender << std::endl;
        return OK;
    }
    else {
        std::cout << "未找到学号为 " << number << " 的学生！" << std::endl;
        return ERROR;
    }
}


//销毁线性表
Status DestroyList_L(StudentList& L) {
    StudentList p;
    while (L) {
        p = L;
        L = L->next;
        free(p);
    }
    return OK;
}


//添加学生信息
Status AddStudent(StudentList& L) {
    StudentList s = (StudentList)malloc(sizeof(StudentNode));
    if (!s) exit(OVERFLOW);

    std::cout << "请输入要添加的学生信息：" << std::endl;

    std::cout << "学号：";
    std::cin >> s->data.number;
    clearInputBuffer();

    std::cout << "年龄：";
    std::cin >> s->data.age;
    clearInputBuffer();

    std::cout << "姓名：";
    std::cin.getline(s->data.name, 100);

    std::cout << "性别：";
    std::cin.getline(s->data.gender, 100);

    // 尾插法添加学生
    StudentList p = L;
    while (p->next) p = p->next;
    s->next = NULL;
    p->next = s;

    std::cout << "学生信息添加成功！" << std::endl;
    return OK;
}


//删除元素
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
        std::cout << "成功删除学号为 " << number << " 的学生信息！" << std::endl;
        return OK;
    }
    else {
        std::cout << "未找到学号为 " << number << " 的学生，删除失败！" << std::endl;
        return ERROR;
    }
}

//遍历
Status ListTraverse_L(StudentList L, Status(*visit)(ElemType)) {
    if (ListEmpty(L)) {
        std::cout << "学生信息列表为空！" << std::endl;
        return ERROR;
    }

    StudentList p = L->next;
    while (p) {
        visit(p->data);
        p = p->next;
    }
    return OK;
}

//访问函数
Status visit(ElemType e) {
    cout << "学号：" << e.number << "\t姓名：" << e.name << "\t年龄：" << e.age << "\t性别：" << e.gender << endl;
    return OK;
}

// 清屏函数
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

//主界面函数
void Menu() {
    cout << "****************************************" << endl;
    cout << "*           学生信息管理系统           *" << endl;
    cout << "****************************************" << endl;
    cout << "1.创建学生表" << endl;
    cout << "2.添加学生信息" << endl;
    cout << "3.删除学生信息" << endl;
    cout << "4.查看所有学生信息" << endl;
    cout << "5.根据学号查询某个学生" << endl;
    cout << "0.退出且销毁表" << endl;
    cout << "请输入你的选择: ";
}

int main() {
    int choice;
    Student student;
    StudentList studentList = NULL; // 初始化为NULL
    CreateEmptyList(studentList);

    while (true) { // 改为无限循环，由case 0来控制退出
        clearScreen(); // 清屏
        Menu();
        cin >> choice;
        clearInputBuffer(); // 使用更可靠的方式清除缓冲区

        switch (choice) {
        case 1:
            clearScreen(); // 清屏
            // 如果已存在链表，先销毁它
            if (studentList) {
                DestroyList_L(studentList);
                CreateEmptyList(studentList);
            }
            InitList_L(studentList);
            cout << "按Enter键继续...";
            cin.get(); // 等待用户按键
            break;
        case 2:
            clearScreen(); // 清屏
            AddStudent(studentList);
            cout << "按Enter键继续...";
            cin.get(); // 等待用户按键
            break;
        case 3:
            clearScreen(); // 清屏
            cout << "请输入要删除的学生学号: ";
            cin >> student.number;
            clearInputBuffer();
            ListDelete_L(studentList, student.number);
            cout << "按Enter键继续...";
            cin.get(); // 等待用户按键
            break;
        case 4:
            clearScreen(); // 清屏
            cout << "学生信息列表：" << endl;
            cout << "----------------------------" << endl;
            ListTraverse_L(studentList, visit);
            cout << "按Enter键继续...";
            cin.get(); // 等待用户按键
            break;
        case 5:
            clearScreen(); // 清屏
            int id;
            cout << "请输入要查询的学生学号: ";
            cin >> id;
            clearInputBuffer();
            SearchStudentById(studentList, id);
            cout << "按Enter键继续...";
            cin.get(); // 等待用户按键
            break;
        case 0:
            if (studentList) {
                DestroyList_L(studentList);
            }
            clearScreen(); // 清屏
            cout << "退出成功！" << endl;
            return 0;
        default:
            cout << "无效的选择，请重新输入！" << endl;
            cout << "按Enter键继续...";
            cin.get(); // 等待用户按键
        }
    }
    return 0;
}