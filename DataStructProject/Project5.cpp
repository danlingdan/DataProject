#include <iostream>
#include <string>

// 常量定义
#define OK 1
#define ERROR 0
#define FALSE 0
#define TRUE 1

#define INF 2147483647           // 无穷大，表示不可达
#define MAX_VERTEX_NUM 20        // 最大顶点数

typedef int Status;
typedef int weight;
typedef std::string city;

// 附加信息结构体
typedef struct InfoType {
    int none;
}InfoType;

// 边的结构体，adj为权值，info为附加信息指针
typedef struct ArcCell {
    weight adj;
    InfoType* info;
}ArcCell, AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

// 图的结构体
typedef struct {
    city vexs[MAX_VERTEX_NUM];      // 顶点名称
    AdjMatrix arcs;                 // 邻接矩阵
    int vexnum, arcnum;             // 顶点数和边数
}MGraph;

// 查找顶点v在图G中的下标，找到返回下标，未找到返回-1
Status LocatVex(MGraph G, city v) {
    for (int i = 0; i < G.vexnum; i++) {
        if (G.vexs[i] == v) {
            return i;
        }
    }
    return -1;
}

// 创建有向图，输入顶点数、边数、各顶点名称和每条边的信息
Status CreatDG(MGraph& G) {
    std::cout << "请输入顶点数和边数：";
    std::cin >> G.vexnum >> G.arcnum;

    // 输入各顶点名称
    for (int i = 0; i < G.vexnum; i++) {
        std::cin >> G.vexs[i];
    }
    // 初始化邻接矩阵
    for (int i = 0; i < G.vexnum; i++) {
        for (int j = 0; j < G.vexnum; j++) {
            G.arcs[i][j] = { INF, nullptr };
        }
    }
    city city1, city2;
    weight w;
    // 输入每条边的信息
    for (int k = 0; k < G.arcnum; k++) {
        std::cout << "请输入两座城市的名字和距离（权值）：";
        std::cin >> city1 >> city2 >> w;
        auto i = LocatVex(G, city1);
        auto j = LocatVex(G, city2);
        if (i == -1 || j == -1) {
            std::cerr << "城市未找到: " << city1 << " 或 " << city2 << std::endl;
            return ERROR;
        }
        G.arcs[i][j].adj = w;
    }
    return OK;
}

// 路径矩阵和最短路径表类型定义
typedef bool PathMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
typedef int ShortPathTable[MAX_VERTEX_NUM]; // 存储最短路径长度

// Dijkstra算法，求v0到其他各点的最短路径
Status ShortestPath_DIJ(MGraph G, city v0, PathMatrix& P, ShortPathTable& D) {
    bool final[MAX_VERTEX_NUM]; // 标记是否已求得最短路径
    int v0Index = LocatVex(G, v0);
    if (v0Index == -1) return ERROR;
    // 初始化
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

    // 主循环，每次确定一个顶点的最短路径
    for (int i = 1; i < G.vexnum; ++i) {
        int min = INF;
        int v = -1;
        // 选取当前未确定的最短路径中距离最小的顶点
        for (int w = 0; w < G.vexnum; ++w) {
            if (!final[w] && D[w] < min) {
                v = w; min = D[w];
            }
        }
        if (v == -1) break; // 剩余顶点不可达
        final[v] = TRUE;
        // 更新与v相邻的顶点的最短路径
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

// 输出从v0到所有其他顶点的最短路径及长度
void PrintShortestPaths(MGraph& G, city v0, PathMatrix& P, ShortPathTable& D) {
    int v0Index = LocatVex(G, v0);
    for (int v = 0; v < G.vexnum; ++v) {
        if (v == v0Index) continue;
        if (D[v] == INF) {
            std::cout << "从 " << v0 << " 到 " << G.vexs[v] << " 不可达" << std::endl;
            continue;
        }
        std::cout << "从 " << v0 << " 到 " << G.vexs[v] << " 的最短路径长度为: " << D[v] << "，路径为: ";
        // 输出路径
        for (int i = 0; i < G.vexnum; ++i) {
            if (P[v][i]) {
                std::cout << G.vexs[i];
                if (i != v) std::cout << " -> ";
            }
        }
        std::cout << std::endl;
    }
}

// 释放图中动态分配的内存
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

// 菜单函数
void Menu() {
    std::cout << "输入1构造图并求取最短路径，输入2退出";
}

// 主函数
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