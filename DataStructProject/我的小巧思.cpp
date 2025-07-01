// ͷ�ļ�
#include <iostream>     
#include <string>       
#include <limits>       
#include <iomanip>      

// ȫ�ֱ���
// 
// ȫ��״̬����
#define OK 1            // ��������ɹ�״̬��
#define ERROR 0         // �������ʧ��״̬��
#define TRUE 1          // ������ֵ
#define FALSE 0         // �����ֵ
#define INFEASIBLE (-1) // ���岻����״̬��
#define OVERFLOW (-2)   // �������״̬��
#define PARKED 1        // ������ͣ��״̬
#define WAITING 0       // ����ȴ�״̬
#define INCREASEMENT 100 // ��������ֵ
// ȫ�ֶ���
typedef int Status;     // ����״̬����
// ���ݽṹ�嶨��
// 
// �����ṹ�嶨��
struct Car
{
    bool status;        // ����״̬
    std::string number; // ���ƺ�
    int position;       // ��λλ��
};
// �����ж���
// �����нڵ�ṹ�嶨��
typedef struct QNode {  // ���нڵ�ṹ
    Car data;           // ��������
    struct QNode* next; // ָ����һ�ڵ��ָ��
} QNode, * QueuePtr;
// �����нṹ�嶨��
typedef struct {        // �����нṹ
    QueuePtr front;     // ��ͷָ��
    QueuePtr rear;      // ��βָ��
    int size;           // ���д�С
} LinkQueue;
// ջ�ᶨ��
// ͣ����ջ�ṹ�嶨��
typedef struct ParkStack {  // ͣ����ջ�ṹ
    int max_car;            // ���λ��
    Car* base;              // ջ��ָ��
    Car* top;               // ջ��ָ��
}ParkStack;


// ��������
// 
// ������������
Status InitQueue(LinkQueue& Q); // ��ʼ�����к���
bool QueueEmpty(LinkQueue& Q); // �ж϶����Ƿ�Ϊ�պ���
int QueueSize(LinkQueue& Q); // ��ȡ���д�С����
Status EnQueue(LinkQueue& Q, Car e); // ��Ӻ���
Status DeQueue(LinkQueue& Q, Car& e); // ���Ӻ���
Status GetFront(LinkQueue& Q, Car& e); // ��ȡ��ͷԪ�غ���
Status InitParkStack(ParkStack& S, int max_car); // ��ʼ��ͣ����ջ����
Status ParkCar(ParkStack& S, LinkQueue& wait_queue, Car car, int& assigned_position); // ͣ������
// ������������
void ClearBuffer(); // ������뻺��������
void DrawLine(char c, int length); // ���Ʒָ���
void DrawTitle(const std::string& title); // ���Ʊ��⺯��
// ���幦��ʵ�ֺ�������
Status RemoveCarFromWaitQueue(LinkQueue& wait_queue); // �ӵȴ������Ƴ���������
Status DriveOutOfTheParkingLot(ParkStack& S, LinkQueue& wait_queue, std::string& number); // �����뿪ͣ��������
// ������������
Status DestroyParkStack(ParkStack& S); // ����ͣ����ջ����
Status DestroyQueue(LinkQueue& Q); // ���ٶ��к���

// ��������ʵ��
// 
// ������뻺��������
void ClearBuffer() {    // ������뻺��������
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
// ���Ʒָ���
void DrawLine(char c = '-', int length = 50) {
    std::cout << std::string(length, c) << std::endl;
}
// ���Ʊ���
void DrawTitle(const std::string& title) {
    DrawLine('=');      // �����ϱ߿�
    std::cout << std::setw(25 + title.length() / 2) << title << std::endl; // ������ʾ����
    DrawLine('=');      // �����±߿�
}



// ��������ʵ��
Status InitQueue(LinkQueue& Q) {  // ��ʼ������
    Q.front = Q.rear = new QNode;
    if (!Q.front) return OVERFLOW;
    Q.front->next = nullptr;
    Q.size = 0;
    return OK;
}
bool QueueEmpty(LinkQueue& Q) {    // �ж϶����Ƿ�Ϊ��
    return Q.front == Q.rear;
}
int QueueSize(LinkQueue& Q) {      // ��ȡ���д�С
    return Q.size;
}
Status EnQueue(LinkQueue& Q, Car e) { // ��Ӳ���
    QNode* p = new QNode;
    if (!p) return OVERFLOW;
    p->data = e;
    p->next = nullptr;
    Q.rear->next = p;
    Q.rear = p;
    Q.size++;
    return OK;
}
Status DeQueue(LinkQueue& Q, Car& e) { // ���Ӳ���
    if (Q.front == Q.rear) return ERROR;
    QNode* p = Q.front->next;
    e = p->data;
    Q.front->next = p->next;
    if (Q.rear == p) Q.rear = Q.front;
    delete p;
    Q.size--;
    return OK;
}
Status GetFront(LinkQueue& Q, Car& e) { // ��ȡ��ͷԪ��
    if (Q.front == Q.rear) return ERROR;
    e = Q.front->next->data;
    return OK;
}
Status InitParkStack(ParkStack& S, int max_car) { // ��ʼ��ͣ����ջ
    S.base = new Car[max_car];
    if (!S.base) return OVERFLOW;
    S.top = S.base;
    S.max_car = max_car;
    return OK;
}
Status ParkCar(ParkStack& S, LinkQueue& wait_queue, Car car, int& assigned_position) { // ͣ������
    if (S.top - S.base >= S.max_car) { // ͣ��������
        car.position = -(QueueSize(wait_queue) + 1);
        EnQueue(wait_queue, car);      // ����ȴ�����
        assigned_position = car.position;
        return WAITING;
    }   
    car.position = S.top - S.base + 1; // ���㳵λ��
    *S.top = car;                       // ������ջ
    S.top++;
    assigned_position = car.position;
    return PARKED;
}

// ��Ӵӵȴ��������Ƴ��ض������ĺ���
Status RemoveCarFromQueue(LinkQueue& Q, const std::string& number) {
    if (QueueEmpty(Q)) return ERROR;  // ����Ϊ��

    // ����һ����ʱ�������洢����Ҫ�Ƴ��ĳ���
    LinkQueue tempQueue;
    if (InitQueue(tempQueue) != OK) return ERROR;

    bool found = false;
    Car car;
    int position = 0;

    std::cout << "\n�ڵȴ������ҳ���: " << number << std::endl;

    // ����ԭ����
    while (!QueueEmpty(Q)) {
        DeQueue(Q, car);

        if (car.number == number) {
            found = true;
            position = -car.position;  // ��¼λ��(��ֵ��ʾ�ȴ���)
            std::cout << "�ҵ�����: " << car.number << " (�ȴ�λ��: " << position << ")" << std::endl;
        }
        else {
            // ����Ŀ�공����������ʱ����
            EnQueue(tempQueue, car);
        }
    }

    // ����ʱ�����еĳ����Ż�ԭ����
    while (!QueueEmpty(tempQueue)) {
        DeQueue(tempQueue, car);
        EnQueue(Q, car);
    }

    // ���µȴ������г�����λ��
    if (found) {
        QNode* current = Q.front->next;
        int pos = 1;
        while (current) {
            current->data.position = -pos++;  // ��ֵ��ʾ�ȴ���
            current = current->next;
        }

        std::cout << "\n[�ɹ�] ���� " << number << " �Ѵӵȴ���ֱ��ʻ��" << std::endl;
    }

    DestroyQueue(tempQueue);
    return found ? OK : ERROR;
}

// �޸ĳ����뿪�������ȼ��ͣ�������ټ��ȴ���
Status DriveOutOfTheParkingLot(ParkStack& S, LinkQueue& wait_queue, std::string& number) {
    // ����ͣ�����в���
    ParkStack tempStack;
    if (InitParkStack(tempStack, S.max_car) != OK) {
        return ERROR;
    }

    bool found = false;
    int count = S.top - S.base;

    std::cout << "\n��ʼ��ͣ����Ѱ�ҳ���: " << number << std::endl;

    // ��ͣ�����в��Ҳ��Ƴ�
    while (S.top > S.base && !found) {
        S.top--;
        Car currentCar = *S.top;

        if (currentCar.number == number) {
            found = true;
            std::cout << "�ҵ�����: " << currentCar.number << " (ͣ��λ��: " << currentCar.position << ")" << std::endl;
        }
        else {
            *tempStack.top = currentCar;
            tempStack.top++;
            std::cout << "��ʱ�Ƴ�����: " << currentCar.number << std::endl;
        }
    }

    if (found) {
        std::cout << "\n���� " << number << " �ɹ���ͣ����ʻ����" << std::endl;

        // ����ʱջ�еĳ����Ż�ԭͣ����
        std::cout << "\n����ʱ�Ƴ��ĳ����Ż�ͣ����:" << std::endl;
        while (tempStack.top > tempStack.base) {
            tempStack.top--;
            *S.top = *tempStack.top;
            std::cout << "���� " << tempStack.top->number << " ����ͣ����" << std::endl;
            S.top++;
        }

        // ���³�λ��
        for (int i = 0; i < (S.top - S.base); i++) {
            S.base[i].position = i + 1;
        }

        // ���ȴ����У�����г����ȴ��������������ͣ����
        if (!QueueEmpty(wait_queue)) {
            Car nextCar;
            DeQueue(wait_queue, nextCar);
            nextCar.position = S.top - S.base + 1;
            nextCar.status = PARKED;
            *S.top = nextCar;
            S.top++;
            std::cout << "\n�ȴ������еĳ��� " << nextCar.number << " ����ͣ���������䳵λ: " << nextCar.position << std::endl;

            // ���µȴ������е�λ��
            QNode* current = wait_queue.front->next;
            int pos = 1;
            while (current) {
                current->data.position = -pos++;
                current = current->next;
            }
        }

        DestroyParkStack(tempStack);
        return OK;
    }

    // ���ͣ������δ�ҵ��������ڵȴ������в���
    DestroyParkStack(tempStack);
    std::cout << "\nͣ������δ�ҵ����� " << number << "�����ڼ��ȴ���..." << std::endl;

    // �ӵȴ������в��Ҳ��Ƴ�
    return RemoveCarFromQueue(wait_queue, number);
}

// ��������ʵ��
Status DestroyParkStack(ParkStack& S) {  // ����ͣ����ջ
    delete[] S.base;
    S.base = nullptr;
    S.top = nullptr;
    S.max_car = 0;
    return OK;
}
Status DestroyQueue(LinkQueue& Q) { // ���ٶ���
    while (Q.front) {
        Q.rear = Q.front->next;
        delete Q.front;
        Q.front = Q.rear;
    }
    Q.size = 0;
    return OK;
}

int main() {
    LinkQueue wait_queue;              // �ȴ�����
    ParkStack parking_lot;             // ͣ����ջ

    using namespace std;

    // ��ʾ��ӭ����
    system("cls");                     // ���� 
    DrawTitle("����ͣ��������ϵͳ");
    cout << "\n" << setw(35) << "��ӭʹ��ͣ��������ϵͳ" << endl;
    DrawLine('-');

    // ��ʼ���ȴ�����
    if (InitQueue(wait_queue) != OK) {
        cout << "\n[����] �ȴ����г�ʼ��ʧ�ܣ�" << endl;
        return OVERFLOW;
    }

    cout << "\n������ͣ��������\n" << endl;
    cout << "������ͣ�����������: ";
    int max_capacity;                  // �������
    cin >> max_capacity;
    ClearBuffer();

    if (InitParkStack(parking_lot, max_capacity) != OK) { // ��ʼ��ͣ����
        cout << "\n[����] ͣ������ʼ��ʧ�ܣ�" << endl;
        DestroyQueue(wait_queue);
        return OVERFLOW;
    }

    cout << "\n[�ɹ�] ͣ������ʼ����ɣ�����: " << max_capacity << " ����" << endl;
    cout << "\n���س�������...";
    cin.get();

    while (true) {                     // ��ѭ��
        //system("cls");                 // ����
        DrawTitle("ͣ��������ϵͳ");

        // ��ʾ��ǰ״̬����
        int parked_count = parking_lot.top - parking_lot.base; // ��ͣ������
        int waiting_count = QueueSize(wait_queue);            // �ȴ���������

        cout << "\n��ǰ״̬: " << endl;
        cout << "��������������������������������������������������������������������������������������" << endl;
        cout << "�� ͣ������ͣ: " << setw(3) << parked_count << " ��    ";
        cout << "���ó�λ: " << setw(3) << max_capacity - parked_count << " ��  ��" << endl;
        cout << "�� �ȴ�������: " << setw(3) << waiting_count << " ��                      ��" << endl;
        cout << "��������������������������������������������������������������������������������������" << endl;

        // ���˵�
        cout << "\n���˵�:" << endl;
        cout << "��������������������������������������������������������������������������������������" << endl;
        cout << "��  1. ����ʻ��                            ��" << endl;
        cout << "��  2. ����ʻ��                            ��" << endl;
        cout << "��  3. �鿴ͣ����״̬                      ��" << endl;
        cout << "��  4. �鿴�ȴ���״̬                      ��" << endl;
        cout << "��  5. �˳�ϵͳ                            ��" << endl;
        cout << "��������������������������������������������������������������������������������������" << endl;

        cout << "\n��ѡ����� [1-5]: ";
        int choice;                    // �û�ѡ��
        cin >> choice;
        ClearBuffer();

        switch (choice) {              // �����û�ѡ��
        case 1: {                      // ����ʻ��
            system("cls");
            DrawTitle("����ʻ��");

            Car car;                   // �³���Ϣ
            cout << "\n�����복�ƺ�: ";
            cin >> car.number;
            ClearBuffer();
            car.status = PARKED;

            int assigned_position;     // �����λ��
            Status res = ParkCar(parking_lot, wait_queue, car, assigned_position);

            DrawLine('-');
            if (res == PARKED) {       // ͣ���ɹ�
                cout << "\n[�ɹ�] ���� " << car.number << " ��ͣ�복λ " << assigned_position << endl;
            }
            else if (res == WAITING) { // ����ȴ���
                cout << "\n[֪ͨ] ͣ�������������� " << car.number << " �Ѽ���ȴ���" << endl;
                cout << "       �ȴ����: " << -assigned_position << endl;
            }
            else {                     // ͣ��ʧ��
                cout << "\n[����] ͣ��ʧ�ܣ�" << endl;
            }

            cout << "\n���س����������˵�...";
            cin.get();
            break;
        }

        case 2: {                      // ����ʻ��
            system("cls");
            DrawTitle("����ʻ��");

            string number;             // ���ƺ�
            cout << "\n������Ҫʻ��ĳ��ƺ�: ";
            cin >> number;
            ClearBuffer();

            DrawLine('-');
            Status res = DriveOutOfTheParkingLot(parking_lot, wait_queue, number);
            if (res == OK) {           // ʻ��ɹ�
            }
            else {                     // δ�ҵ�����
                cout << "\n[����] δ�ҵ����ƺ�Ϊ " << number << " �ĳ�����" << endl;
                cout << "       ��ȷ�ϳ��ƺ��Ƿ���ȷ�������Ƿ�����ͣ����/�ȴ�����" << endl;
            }

            cout << "\n���س����������˵�...";
            cin.get();
            break;
        }

        case 3: {                      // �鿴ͣ����״̬
            system("cls");
            DrawTitle("ͣ����״̬");

            int count = parking_lot.top - parking_lot.base; // ��ǰ������
            cout << "\n��ǰͣ����������: " << count << " / " << max_capacity << endl;

            if (count > 0) {           // �г���
                DrawLine('-');
                cout << "�����������������Щ���������������������������������" << endl;
                cout << "�� ��λ  ��    ���ƺ�      ��" << endl;
                cout << "�����������������੤��������������������������������" << endl;

                for (int i = 0; i < count; ++i) { // ��ʾ���г���
                    cout << "�� " << setw(5) << parking_lot.base[i].position
                        << " �� " << setw(14) << parking_lot.base[i].number << " ��" << endl;
                }

                cout << "�����������������ة���������������������������������" << endl;
            }
            else {                     // �޳���
                cout << "\n[֪ͨ] ͣ���������޳���" << endl;
            }

            cout << "\n���س����������˵�...";
            cin.get();
            break;
        }

        case 4: {                      // �鿴�ȴ���״̬
            system("cls");
            DrawTitle("�ȴ���״̬");

            if (QueueEmpty(wait_queue)) { // �ȴ���Ϊ��
                cout << "\n[֪ͨ] �ȴ���Ϊ�գ�û�еȴ��ĳ���" << endl;
            }
            else {                     // ��ʾ�ȴ�����
                cout << "\n�ȴ������� (�� " << QueueSize(wait_queue) << " ��):" << endl;
                DrawLine('-');

                cout << "�����������������Щ���������������������������������" << endl;
                cout << "�� ���  ��    ���ƺ�      ��" << endl;
                cout << "�����������������੤��������������������������������" << endl;

                QNode* p = wait_queue.front->next; // ��������
                int idx = 1;
                while (p) {
                    cout << "�� " << setw(5) << idx++
                        << " �� " << setw(14) << p->data.number << " ��" << endl;
                    p = p->next;
                }

                cout << "�����������������ة���������������������������������" << endl;
                cout << "\nע: ������ǰ��ĳ��������Ƚ���ͣ����" << endl;
            }

            cout << "\n���س����������˵�...";
            cin.get();
            break;
        }

        case 5: {
            system("cls");
            DrawTitle("ϵͳ�˳�");
            cout << "\n��лʹ��ͣ��������ϵͳ���ټ���\n" << endl;
            DestroyParkStack(parking_lot);
            DestroyQueue(wait_queue);
            return 0;
        }

        default: {
            cout << "\n[����] ��Чѡ����������룡" << endl;
            cout << "\n���س�������...";
            cin.get();
            break;
        }
        }
    }

    DestroyParkStack(parking_lot);
    DestroyQueue(wait_queue);
    return 0;
}