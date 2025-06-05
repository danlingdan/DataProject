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
        std::cout << "�������" << i+1 << "�����е�����";
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

// Dijkstra�㷨����v0��������������·��������prev�����¼·��
Status ShortestPath_DIJ(MGraph G, city v0, ShortPathTable& D, int prev[MAX_VERTEX_NUM]) {
    bool final[MAX_VERTEX_NUM];
    int v0Index = LocatVex(G, v0);
    if (v0Index == -1) return ERROR;
    for (int v = 0; v < G.vexnum; v++) {
        final[v] = FALSE;
        D[v] = G.arcs[v0Index][v].adj;
        if (D[v] < INF)
            prev[v] = v0Index;
        else
            prev[v] = -1;
    }
    D[v0Index] = 0;
    final[v0Index] = TRUE;
    prev[v0Index] = -1;

    for (int i = 1; i < G.vexnum; ++i) {
        int min = INF, v = -1;
        for (int w = 0; w < G.vexnum; ++w) {
            if (!final[w] && D[w] < min) {
                v = w; min = D[w];
            }
        }
        if (v == -1) break;
        final[v] = TRUE;
        for (int w = 0; w < G.vexnum; w++) {
            if (!final[w] && G.arcs[v][w].adj < INF && min + G.arcs[v][w].adj < D[w]) {
                D[w] = min + G.arcs[v][w].adj;
                prev[w] = v;
            }
        }
    }
    return OK;
}

// �����start��end��·��
void PrintPath(MGraph& G, int prev[], int start, int end) {
    if (end == -1) return;
    if (end != start) {
        PrintPath(G, prev, start, prev[end]);
        std::cout << " -> ";
    }
    std::cout << G.vexs[end];
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
    do {
        Menu();
        std::cin >> choice;
        if (choice == 1) {
            CreatDG(G);

            city startCity, endCity;
            std::cout << "���������������ƣ�";
            std::cin >> startCity;
            std::cout << "�������յ�������ƣ�";
            std::cin >> endCity;

            int startIdx = LocatVex(G, startCity);
            int endIdx = LocatVex(G, endCity);
            if (startIdx == -1 || endIdx == -1) {
                std::cout << "����������������" << std::endl;
                continue;
            }

            ShortPathTable D;
            int prev[MAX_VERTEX_NUM];
            if (ShortestPath_DIJ(G, startCity, D, prev) == ERROR) {
                std::cout << "���·������ʧ�ܣ�" << std::endl;
                continue;
            }

            if (D[endIdx] == INF) {
                std::cout << "�� " << startCity << " �� " << endCity << " ���ɴ" << std::endl;
            }
            else {
                std::cout << "���·������Ϊ: " << D[endIdx] << std::endl;
                std::cout << "�������ĳ���Ϊ: ";
                PrintPath(G, prev, startIdx, endIdx);
                std::cout << std::endl;
            }

            DestoryMGraph(G);
        }
    } while (choice != 2);
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