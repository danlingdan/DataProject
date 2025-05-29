#include <iostream>
#include <string>

#define OK 1
#define ERROR 0

#define MAX_VERTEX_NUM 100
typedef int Status;
typedef int weight;
typedef std::string city;

typedef struct ArcBox {
	int  tailvex, headtex; // head arc and tail arc's position
	struct ArcBox* hlink, * tlink; //next arc strcuct position 
	weight* info; //arc's info(weight)
}ArcBox;

typedef struct VexNode {
	city data; //node's info
	ArcBox* firstin, * firstout; //first income arc and outcome arc
}VexNode;

typedef struct {
	VexNode xlist[MAX_VERTEX_NUM]; 
	int vexnum, arcnum; //current vexnum and arcnum
}MGraph;

int LocateVex(MGraph& G, city v) {
	int i;
	//遍历一维数组，找到变量v
	for (i = 0; i < G.vexnum; i++) {
		if (G.xlist[i].data == v) {
			break;
		}
	}
	//如果找不到，输出提示语句，返回 -1
	if (i >= G.vexnum) {
		printf("no such vertex.\n");
		return ERROR;
	}
	return i;
}

Status CreatDG(MGraph& G) {
	std::cin >> G.vexnum >> G.arcnum; //input vexnum and arcnum

	for (int i = 0; i < G.vexnum; ++i) {
		std::cin >> G.xlist[i].data;
		G.xlist[i].firstin = nullptr;
		G.xlist[i].firstout = nullptr;
	}

	city v1, v2;
	int i, j;
	ArcBox* p = nullptr;
	for (int k = 0; k < G.arcnum; ++k) {
		std::cin >> v1 >> v2;
		i = LocateVex(G, v1);
		j = LocateVex(G, v2);
		p = new ArcBox;
		*p = { i,j,G.xlist[j].firstin,G.xlist[i].firstout,nullptr };
		G.xlist[j].firstin = G.xlist[i].firstout = p;
		p->info = new weight; // Allocate memory for the weight
		std::cin >> *(p->info); // Dereference the pointer to input the value
	}
	return OK;
}

typedef bool PathMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
typedef int ShortPathTable[MAX_VERTEX_NUM]; // tore shortest path distances for each vertex
Status ShortestFath_DIJ(MGraph G, int v0, PathMatrix& P, ShortPathTable& D) {
    const int INF = 0x3f3f3f3f;
    int n = G.vexnum;
    bool final[MAX_VERTEX_NUM] = { false };

    // Initialize D and P
    for (int v = 0; v < n; ++v) {
        D[v] = INF;
        final[v] = false;
        for (int w = 0; w < n; ++w)
            P[v][w] = false;
    }
    D[v0] = 0;
    final[v0] = true;

    // Set initial distances from v0
    ArcBox* arc = G.xlist[v0].firstout;
    while (arc) {
        D[arc->headtex] = *(arc->info);
        P[arc->headtex][v0] = true;
        P[arc->headtex][arc->headtex] = true;
        arc = arc->tlink;
    }

    // Main loop
    for (int i = 1; i < n; ++i) {
        int min = INF, u = -1;
        for (int w = 0; w < n; ++w) {
            if (!final[w] && D[w] < min) {
                u = w;
                min = D[w];
            }
        }
        if (u == -1) break;
        final[u] = true;

        // Update distances
        arc = G.xlist[u].firstout;
        while (arc) {
            int v = arc->headtex;
            if (!final[v] && D[u] + *(arc->info) < D[v]) {
                D[v] = D[u] + *(arc->info);
                for (int k = 0; k < n; ++k)
                    P[v][k] = P[u][k];
                P[v][u] = true;
            }
            arc = arc->tlink;
        }
    }
    return OK;
}

int main() {
	MGraph G;
	CreatDG(G);
}