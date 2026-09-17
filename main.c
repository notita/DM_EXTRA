#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int u;
	int v;
	int idx;
} Edge;

typedef struct {
	int* adj_v;
	int* adj_e;
	int deg;
} Vertex;

Edge* graph;
Vertex* adj;

int* visited;
int* disc;
int* low;
int timer = 0;

int* bridges;
int bridge_count;

void dfs(int v, int parent_e) {
	visited[v] = 1;
	disc[v] = low[v] = timer;
	timer++;

	for (int i = 0;i < adj[v].deg; i++) {
		int to = adj[v].adj_v[i];
		int e = adj[v].adj_e[i];

		if (e == parent_e) continue;

		if (visited[to]) {
			if (disc[to] < low[v]) low[v] = disc[to];
		}
		else {
			dfs(to, e);
			if (low[v] > low[to]) low[v] = low[to];
		}

		if (low[to] > disc[v]) {
			bridges[bridge_count] = e;
			bridge_count++;
		}

	}
}

void main() {
	int n, m;
	scanf("%d %d", &n, &m, 2);

	graph = (Edge*)malloc((m+1) * sizeof(Edge));
	adj = (Vertex*)malloc((n+1) * sizeof(Vertex));

	disc = (int*)malloc((n + 1) * sizeof(int));
	low = (int*)malloc((n + 1) * sizeof(int));
	visited = (int*)malloc((n + 1) * sizeof(int));
	bridges = (int*)malloc((m + 1) * sizeof(int));

	for (int i = 1;i < n + 1;i++) {
		adj[i].deg = 0;
		visited[i] = 0;
	}

	// Первый проход по рёбрам: считывание и подсчёт степеней вершин
	for (int i = 1; i <= m; i++) {
		scanf("%d %d", &graph[i].u, &graph[i].v);
		graph[i].idx = i; // Номер ребра по порядку
		adj[graph[i].u].deg++;
		adj[graph[i].v].deg++;
	}

	for (int i = 1; i < n + 1; i++) {
		adj[i].adj_v = (int*)malloc(adj[i].deg * sizeof(int));
		adj[i].adj_e = (int*)malloc(adj[i].deg * sizeof(int));
		adj[i].deg = 0; // Сбрасываем, чтобы использовать как счётчик при заполнении
	}

	// Второй проход: заполнение списков смежности
	for (int i = 1; i < m + 1; i++) {
		int u = graph[i].u;
		int v = graph[i].v;
		int idx = graph[i].idx;

		// Добавляем для вершины u
		adj[u].adj_v[adj[u].deg] = v;
		adj[u].adj_e[adj[u].deg] = idx;
		adj[u].deg++;

		// Добавляем для вершины v
		adj[v].adj_v[adj[v].deg] = u;
		adj[v].adj_e[adj[v].deg] = idx;
		adj[v].deg++;
	}

	for (int i = 1; i < n + 1; i++) if (!visited[i]) dfs(i, -1); 


	for (int i = 0; i < bridge_count; i++) {
		printf("%d", bridges[i]);
		if (i < bridge_count - 1) printf(" ");
	}
	printf("\n");


	for (int i = 1; i < n + 1; i++) {
		free(adj[i].adj_v);
		free(adj[i].adj_e);
	}
	free(graph);
	free(adj);
	free(disc);
	free(low);
	free(visited);
	free(bridges);
}
