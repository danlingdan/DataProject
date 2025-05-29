#include <iostream>
#include <string>

// ��������
#define OK 1
#define ERROR 0
#define FALSE 0
#define TRUE 1

#define INF 2147483647           // ����󣬱�ʾ���ɴ�
#define MAX_VERTEX_NUM 20        // ��󶥵���

typedef int Status;
typedef int weight;
typedef std::string city;

// ������Ϣ�ṹ��
typedef struct InfoType {
    int none;
}InfoType;

// �ߵĽṹ�壬adjΪȨֵ��infoΪ������Ϣָ��
typedef struct ArcCell {
    weight adj;
    InfoType* info;
}ArcCell, AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

// ͼ�Ľṹ��
typedef struct {
    city vexs[MAX_VERTEX_NUM];      // ��������
    AdjMatrix arcs;                 // �ڽӾ���
    int vexnum, arcnum;             // �������ͱ���
}MGraph;

// ���Ҷ���v��ͼG�е��±꣬�ҵ������±꣬δ�ҵ�����-1
Status LocatVex(MGraph G, city v) {
    for (int i = 0; i < G.vexnum; i++) {
        if (G.vexs[i] == v) {
            return i;
        }
    }
    return -1;
}

// ��������ͼ�����붥���������������������ƺ�ÿ���ߵ���Ϣ
Status CreatDG(MGraph& G) {
    std::cout << "�����붥�����ͱ�����";
    std::cin >> G.vexnum >> G.arcnum;

    // �������������
    for (int i = 0; i < G.vexnum; i++) {
        std::cin >> G.vexs[i];
    }
    // ��ʼ���ڽӾ���
    for (int i = 0; i < G.vexnum; i++) {
        for (int j = 0; j < G.vexnum; j++) {
            G.arcs[i][j] = { INF, nullptr };
        }
    }
    city city1, city2;
    weight w;
    // ����ÿ���ߵ���Ϣ
    for (int k = 0; k < G.arcnum; k++) {
        std::cout << "�������������е����ֺ;��루Ȩֵ����";
        std::cin >> city1 >> city2 >> w;
        auto i = LocatVex(G, city1);
        auto j = LocatVex(G, city2);
        if (i == -1 || j == -1) {
            std::cerr << "����δ�ҵ�: " << city1 << " �� " << city2 << std::endl;
            return ERROR;
        }
        G.arcs[i][j].adj = w;
    }
    return OK;
}

// ·����������·�������Ͷ���
typedef bool PathMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
typedef int ShortPathTable[MAX_VERTEX_NUM]; // �洢���·������

// Dijkstra�㷨����v0��������������·��
Status ShortestPath_DIJ(MGraph G, city v0, PathMatrix& P, ShortPathTable& D) {
    bool final[MAX_VERTEX_NUM]; // ����Ƿ���������·��
    int v0Index = LocatVex(G, v0);
    if (v0Index == -1) return ERROR;
    // ��ʼ��
    for (int v = 0; v < G.vexnum; v++) {
        final[v] = FALSE;
        D[v] = G.arcs[v0Index][v].adj;
        for (int w = 0; w < G.vexnum; ++w) {
            P[v][w] = FALSE;
        }
        if (D[v] < INF) {
            P[v][v0Index] = TRUE; P[v][v] = TRUE;
        }
    }

    D[v0Index] = 0; final[v0Index] = TRUE;

    // ��ѭ����ÿ��ȷ��һ����������·��
    for (int i = 1; i < G.vexnum; ++i) {
        int min = INF;
        int v = -1;
        // ѡȡ��ǰδȷ�������·���о�����С�Ķ���
        for (int w = 0; w < G.vexnum; ++w) {
            if (!final[w] && D[w] < min) {
                v = w; min = D[w];
            }
        }
        if (v == -1) break; // ʣ�ඥ�㲻�ɴ�
        final[v] = TRUE;
        // ������v���ڵĶ�������·��
        for (int w = 0; w < G.vexnum; w++) {
            if (!final[w] && (min + G.arcs[v][w].adj < D[w])) {
                D[w] = min + G.arcs[v][w].adj;
                for (int k = 0; k < G.vexnum; ++k) {
                    P[w][k] = P[v][k];
                }
                P[w][w] = TRUE;
            }
        }
    }
    return OK;
}

// �����v0������������������·��������
void PrintShortestPaths(MGraph& G, city v0, PathMatrix& P, ShortPathTable& D) {
    int v0Index = LocatVex(G, v0);
    for (int v = 0; v < G.vexnum; ++v) {
        if (v == v0Index) continue;
        if (D[v] == INF) {
            std::cout << "�� " << v0 << " �� " << G.vexs[v] << " ���ɴ�" << std::endl;
            continue;
        }
        std::cout << "�� " << v0 << " �� " << G.vexs[v] << " �����·������Ϊ: " << D[v] << "��·��Ϊ: ";
        // ���·��
        for (int i = 0; i < G.vexnum; ++i) {
            if (P[v][i]) {
                std::cout << G.vexs[i];
                if (i != v) std::cout << " -> ";
            }
        }
        std::cout << std::endl;
    }
}

// �ͷ�ͼ�ж�̬������ڴ�
void DestoryMGraph(MGraph& G) {
    for (int i = 0; i < G.vexnum; ++i) {
        for (int j = 0; j < G.vexnum; ++j) {
            if (G.arcs[i][j].info != nullptr) {
                delete G.arcs[i][j].info;
                G.arcs[i][j].info = nullptr;
            }
        }
    }
    G.vexnum = 0;
    G.arcnum = 0;
}

// �˵�����
void Menu() {
    std::cout << "����1����ͼ����ȡ���·��������2�˳�";
}

// ������
int main() {
    MGraph G;
    int choice;
    Menu();
    std::cin >> choice;
    while (choice != 2) {
        Menu();
        CreatDG(G);
        PathMatrix P;
        ShortPathTable D;
        ShortestPath_DIJ(G, G.vexs[0], P, D);
        PrintShortestPaths(G, G.vexs[0], P, D);
        DestoryMGraph(G);
        getchar();
        system("cls");
    }
    return 0;
}

/*test examples*/
/*
Example 1: Simple 3-city graph:
3 3
Beijing
Shanghai
Guangzhou
Beijing Shanghai 1200
Shanghai Guangzhou 1500
Beijing Guangzhou 2000

Example 2: 4 cities, 5 roads:
4 5
A
B
C
D
A B 5
A C 10
B C 3
B D 7
C D 1

Example 3: Disconnected city:
4 2
Xian
Chengdu
Wuhan
Shenzhen
Xian Chengdu 800
Chengdu Wuhan 900
*/