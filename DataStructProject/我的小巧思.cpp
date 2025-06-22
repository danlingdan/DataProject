// 头文件
#include <iostream>     
#include <string>       
#include <limits>       
#include <iomanip>      

// 全局变量
// 
// 全局状态变量
#define OK 1            // 定义操作成功状态码
#define ERROR 0         // 定义操作失败状态码
#define TRUE 1          // 定义真值
#define FALSE 0         // 定义假值
#define INFEASIBLE (-1) // 定义不可行状态码
#define OVERFLOW (-2)   // 定义溢出状态码
#define PARKED 1        // 定义已停车状态
#define WAITING 0       // 定义等待状态
#define INCREASEMENT 100 // 定义增量值
// 全局定义
typedef int Status;     // 定义状态类型

// 函数声明
// 
// 基本函数声明
Status InitQueue(LinkQueue& Q); // 初始化队列函数
bool QueueEmpty(LinkQueue& Q); // 判断队列是否为空函数
int QueueSize(LinkQueue& Q); // 获取队列大小函数
Status EnQueue(LinkQueue& Q, Car e); // 入队函数
Status DeQueue(LinkQueue& Q, Car& e); // 出队函数
Status GetFront(LinkQueue& Q, Car& e); // 获取队头元素函数
Status InitParkStack(ParkStack& S, int max_car); // 初始化停车场栈函数
Status ParkCar(ParkStack& S, LinkQueue& wait_queue, Car car, int& assigned_position); // 停车函数
// 辅助函数声明
void ClearBuffer(); // 清空输入缓冲区函数
void DrawLine(char c = '-', int length = 50); // 绘制分隔线
void DrawTitle(const std::string& title); // 绘制标题函数
// 具体功能实现函数声明
Status AddCarToWaitQueue(LinkQueue& wait_queue, Car car); // 添加车辆到等待队列函数
Status RemoveCarFromWaitQueue(LinkQueue& wait_queue); // 从等待队列移除车辆函数
Status GetCarFromWaitQueue(LinkQueue& wait_queue, Car& car); // 获取等待队列中的车辆函数
Status DriveOutOfTheParkingLot(ParkStack& S, LinkQueue& wait_queue, std::string& number); // 车辆离开停车场函数
// 结束函数声明
Status DestroyParkStack(ParkStack& S); // 销毁停车场栈函数
Status DestroyQueue(LinkQueue& Q); // 销毁队列函数

// 辅助函数实现
// 
// 清空输入缓冲区函数
void ClearBuffer() {    // 清空输入缓冲区函数
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
// 绘制分隔线
void DrawLine(char c = '-', int length = 50) {
    std::cout << std::string(length, c) << std::endl;
}
// 绘制标题
void DrawTitle(const std::string& title) {
    DrawLine('=');      // 绘制上边框
    std::cout << std::setw(25 + title.length() / 2) << title << std::endl; // 居中显示标题
    DrawLine('=');      // 绘制下边框
}

// 数据结构体定义
// 
// 车辆结构体定义
struct Car
{
    bool status;        // 车辆状态
    std::string number; // 车牌号
    int position;       // 车位位置
};
// 链队列定义
// 链队列节点结构体定义
typedef struct QNode {  // 队列节点结构
    Car data;           // 车辆数据
    struct QNode* next; // 指向下一节点的指针
} QNode, * QueuePtr;
// 链队列结构体定义
typedef struct {        // 链队列结构
    QueuePtr front;     // 队头指针
    QueuePtr rear;      // 队尾指针
    int size;           // 队列大小
} LinkQueue;
// 栈结定义
// 停车场栈结构体定义
typedef struct ParkStack {  // 停车场栈结构
    int max_car;            // 最大车位数
    Car* base;              // 栈底指针
    Car* top;               // 栈顶指针
}ParkStack;

// 基本函数实现
Status InitQueue(LinkQueue& Q) {  // 初始化队列
    Q.front = Q.rear = new QNode;
    if (!Q.front) return OVERFLOW;
    Q.front->next = nullptr;
    Q.size = 0;
    return OK;
}
bool QueueEmpty(LinkQueue& Q) {    // 判断队列是否为空
    return Q.front == Q.rear;
}
int QueueSize(LinkQueue& Q) {      // 获取队列大小
    return Q.size;
}
Status EnQueue(LinkQueue& Q, Car e) { // 入队操作
    QNode* p = new QNode;
    if (!p) return OVERFLOW;
    p->data = e;
    p->next = nullptr;
    Q.rear->next = p;
    Q.rear = p;
    Q.size++;
    return OK;
}
Status DeQueue(LinkQueue& Q, Car& e) { // 出队操作
    if (Q.front == Q.rear) return ERROR;
    QNode* p = Q.front->next;
    e = p->data;
    Q.front->next = p->next;
    if (Q.rear == p) Q.rear = Q.front;
    delete p;
    Q.size--;
    return OK;
}
Status GetFront(LinkQueue& Q, Car& e) { // 获取队头元素
    if (Q.front == Q.rear) return ERROR;
    e = Q.front->next->data;
    return OK;
}
Status InitParkStack(ParkStack& S, int max_car) { // 初始化停车场栈
    S.base = new Car[max_car];
    if (!S.base) return OVERFLOW;
    S.top = S.base;
    S.max_car = max_car;
    return OK;
}
Status ParkCar(ParkStack& S, LinkQueue& wait_queue, Car car, int& assigned_position) { // 停车操作
    if (S.top - S.base >= S.max_car) { // 停车场已满
        car.position = -(QueueSize(wait_queue) + 1);
        EnQueue(wait_queue, car);      // 加入等待队列
        assigned_position = car.position;
        return WAITING;
    }
    int pos = S.top - S.base + 1;      // 计算车位号
    car.position = pos;
    *S.top = car;                       // 车辆入栈
    S.top++;
    assigned_position = car.position;
    return PARKED;
}

// 具体功能实现函数实现
Status AddCarToWaitQueue(LinkQueue& wait_queue, Car car) { // 添加车辆到等待队列
    EnQueue(wait_queue, car);
    return WAITING;
}
Status RemoveCarFromWaitQueue(LinkQueue& wait_queue) { // 从等待队列移除车辆
    if (QueueEmpty(wait_queue)) return ERROR;
    Car temp;
    DeQueue(wait_queue, temp);
    return OK;
}
Status GetCarFromWaitQueue(LinkQueue& wait_queue, Car& car) { // 获取等待队列中的车辆
    if (QueueEmpty(wait_queue)) return ERROR;
    GetFront(wait_queue, car);
    return OK;
}
Status DriveOutOfTheParkingLot(ParkStack& S, LinkQueue& wait_queue,  std::string& number) { // 车辆离开停车场
    int count = S.top - S.base;
    int idx = -1;
    for (int i = 0; i < count; ++i) {  // 查找车辆位置
        if (S.base[i].number == number) {
            idx = i;
            break;
        }
    }
    if (idx == -1) return ERROR;       // 未找到车辆

    for (int i = idx; i < count - 1; ++i) { // 移动后面的车辆
        S.base[i] = S.base[i + 1];
        S.base[i].position = i + 1;
    }
    S.top--;                           // 栈顶指针减1

    if (!QueueEmpty(wait_queue)) {     // 等待队列不为空
        Car nextCar;
        GetFront(wait_queue, nextCar); // 获取队头车辆
        DeQueue(wait_queue, nextCar);  // 出队
        nextCar.position = count;      // 分配车位
        *S.top = nextCar;              // 进入停车场
        S.top++;
    }
    return OK;
}

// 结束函数实现
Status DestroyParkStack(ParkStack& S) {  // 销毁停车场栈
    delete[] S.base;
    S.base = nullptr;
    S.top = nullptr;
    S.max_car = 0;
    return OK;
}
Status DestroyQueue(LinkQueue& Q) { // 销毁队列
    while (Q.front) {
        Q.rear = Q.front->next;
        delete Q.front;
        Q.front = Q.rear;
    }
    Q.size = 0;
    return OK;
}

int main() {
    LinkQueue wait_queue;              // 等待队列
    ParkStack parking_lot;             // 停车场栈

    using namespace std;

    // 显示欢迎界面
    system("cls");                     // 清屏 
    DrawTitle("智能停车场管理系统");
    cout << "\n" << setw(35) << "欢迎使用停车场管理系统" << endl;
    cout << setw(30) << "版本: 1.1" << endl << endl;
    DrawLine('-');

    // 初始化等待队列
    if (InitQueue(wait_queue) != OK) {
        cout << "\n[错误] 等待队列初始化失败！" << endl;
        return OVERFLOW;
    }

    cout << "\n请设置停车场参数\n" << endl;
    cout << "请输入停车场最大容量: ";
    int max_capacity;                  // 最大容量
    cin >> max_capacity;
    ClearBuffer();

    if (InitParkStack(parking_lot, max_capacity) != OK) { // 初始化停车场
        cout << "\n[错误] 停车场初始化失败！" << endl;
        DestroyQueue(wait_queue);
        return OVERFLOW;
    }

    cout << "\n[成功] 停车场初始化完成！容量: " << max_capacity << " 辆车" << endl;
    cout << "\n按回车键继续...";
    cin.get();

    while (true) {                     // 主循环
        system("cls");                 // 清屏
        DrawTitle("停车场管理系统");

        // 显示当前状态概览
        int parked_count = parking_lot.top - parking_lot.base; // 已停车数量
        int waiting_count = QueueSize(wait_queue);            // 等待车辆数量

        cout << "\n当前状态: " << endl;
        cout << "┌─────────────────────────────────────────┐" << endl;
        cout << "│ 停车场已停: " << setw(3) << parked_count << " 辆    ";
        cout << "可用车位: " << setw(3) << max_capacity - parked_count << " 个  │" << endl;
        cout << "│ 等待区车辆: " << setw(3) << waiting_count << " 辆                      │" << endl;
        cout << "└─────────────────────────────────────────┘" << endl;

        // 主菜单
        cout << "\n主菜单:" << endl;
        cout << "┌─────────────────────────────────────────┐" << endl;
        cout << "│  1. 车辆驶入                            │" << endl;
        cout << "│  2. 车辆驶离                            │" << endl;
        cout << "│  3. 查看停车场状态                      │" << endl;
        cout << "│  4. 查看等待区状态                      │" << endl;
        cout << "│  5. 退出系统                            │" << endl;
        cout << "└─────────────────────────────────────────┘" << endl;

        cout << "\n请选择操作 [1-5]: ";
        int choice;                    // 用户选择
        cin >> choice;
        ClearBuffer();

        switch (choice) {              // 处理用户选择
        case 1: {                      // 车辆驶入
            system("cls");
            DrawTitle("车辆驶入");

            Car car;                   // 新车信息
            cout << "\n请输入车牌号: ";
            cin >> car.number;
            ClearBuffer();
            car.status = PARKED;

            int assigned_position;     // 分配的位置
            Status res = ParkCar(parking_lot, wait_queue, car, assigned_position);

            DrawLine('-');
            if (res == PARKED) {       // 停车成功
                cout << "\n[成功] 车辆 " << car.number << " 已停入车位 " << assigned_position << endl;
            }
            else if (res == WAITING) { // 进入等待区
                cout << "\n[通知] 停车场已满，车辆 " << car.number << " 已加入等待区" << endl;
                cout << "       等待编号: " << -assigned_position << endl;
            }
            else {                     // 停车失败
                cout << "\n[错误] 停车失败！" << endl;
            }

            cout << "\n按回车键返回主菜单...";
            cin.get();
            break;
        }

        case 2: {                      // 车辆驶离
            system("cls");
            DrawTitle("车辆驶离");

            string number;             // 车牌号
            cout << "\n请输入要驶离的车牌号: ";
            cin >> number;
            ClearBuffer();

            DrawLine('-');
            Status res = DriveOutOfTheParkingLot(parking_lot, wait_queue, number);
            if (res == OK) {           // 驶离成功
                cout << "\n[成功] 车辆 " << number << " 已驶离停车场" << endl;
            }
            else {                     // 未找到车辆
                cout << "\n[错误] 未找到车牌号为 " << number << " 的车辆！" << endl;
            }

            cout << "\n按回车键返回主菜单...";
            cin.get();
            break;
        }

        case 3: {                      // 查看停车场状态
            system("cls");
            DrawTitle("停车场状态");

            int count = parking_lot.top - parking_lot.base; // 当前车辆数
            cout << "\n当前停车场车辆数: " << count << " / " << max_capacity << endl;

            if (count > 0) {           // 有车辆
                DrawLine('-');
                cout << "┌───────┬────────────────┐" << endl;
                cout << "│ 车位  │    车牌号      │" << endl;
                cout << "├───────┼────────────────┤" << endl;

                for (int i = 0; i < count; ++i) { // 显示所有车辆
                    cout << "│ " << setw(5) << parking_lot.base[i].position
                        << " │ " << setw(14) << parking_lot.base[i].number << " │" << endl;
                }

                cout << "└───────┴────────────────┘" << endl;
            }
            else {                     // 无车辆
                cout << "\n[通知] 停车场内暂无车辆" << endl;
            }

            cout << "\n按回车键返回主菜单...";
            cin.get();
            break;
        }

        case 4: {                      // 查看等待区状态
            system("cls");
            DrawTitle("等待区状态");

            if (QueueEmpty(wait_queue)) { // 等待区为空
                cout << "\n[通知] 等待区为空，没有等待的车辆" << endl;
            }
            else {                     // 显示等待车辆
                cout << "\n等待区车辆 (共 " << QueueSize(wait_queue) << " 辆):" << endl;
                DrawLine('-');

                cout << "┌───────┬────────────────┐" << endl;
                cout << "│ 序号  │    车牌号      │" << endl;
                cout << "├───────┼────────────────┤" << endl;

                QNode* p = wait_queue.front->next; // 遍历队列
                int idx = 1;
                while (p) {
                    cout << "│ " << setw(5) << idx++
                        << " │ " << setw(14) << p->data.number << " │" << endl;
                    p = p->next;
                }

                cout << "└───────┴────────────────┘" << endl;
                cout << "\n注: 排在最前面的车辆将优先进入停车场" << endl;
            }

            cout << "\n按回车键返回主菜单...";
            cin.get();
            break;
        }

        case 5: {
            system("cls");
            DrawTitle("系统退出");
            cout << "\n感谢使用停车场管理系统，再见！\n" << endl;
            DestroyParkStack(parking_lot);
            DestroyQueue(wait_queue);
            return 0;
        }

        default: {
            cout << "\n[错误] 无效选项，请重新输入！" << endl;
            cout << "\n按回车键继续...";
            cin.get();
            break;
        }
        }
    }

    DestroyParkStack(parking_lot);
    DestroyQueue(wait_queue);
    return 0;
}