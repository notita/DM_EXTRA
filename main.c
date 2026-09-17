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
int* parent_edges;
int timer = 0;

int* bridges;
int bridge_count;

typedef struct {
	int v;
	int idx_e; //аналог счетчика i в for цикле, чтобы dfs знал куда вернуться
}
Stack;

void dfs(int start, int n) {
	Stack* stack = (Stack*)malloc((n + 1) * sizeof(Stack));
	int top = 0;

	visited[start] = 1;
	disc[start] = low[start] = timer;
	timer++;
	parent_edges[start] = -1;
	
	stack[top].v = start;
	stack[top].idx_e = 0;
	top++;

	while (top > 0) {
		int v = stack[top - 1].v; 
		int idx_e = stack[top - 1].idx_e;

		if(idx_e < adj[v].deg) {
			int to = adj[v].adj_v[idx_e];
			int e = adj[v].adj_e[idx_e];
			stack[top - 1].idx_e++;

			if (e == parent_edges[v]) continue;

			if (visited[to]) {
				if (disc[to] < low[v]) low[v] = disc[to];
			}
			else {
				visited[to] = 1;
				timer++;
				disc[to] = low[to] = timer;
				parent_edges[to] = e;

				stack[top].v = to;
				stack[top].idx_e = 0;
				top++;
			}

		}
		else {
			top--; //top фактически указывает на индекс следующей свободной ячейки, при этом при переходе он автоматически увеличивается на 1, как бы предсказывая этот индекс. Если предсказание неверное, то top нужно обратно уменьшить.
			if (top > 0) {
				int parent_v = stack[top - 1].v;
				if (low[v] < low[parent_v]) low[parent_v] = low[v];
				if (low[v] > disc[parent_v]) bridges[bridge_count++] = parent_edges[v];
			
			}
		}
	}
	free(stack);
}

int cmp(const void* a, const void* b) {
	return (*(int*)a - *(int*)b);
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
	parent_edges = (int*)malloc((n + 1) * sizeof(int));

	for (int i = 1;i < n + 1;i++) {
		adj[i].deg = 0;
		visited[i] = 0;
		parent_edges[i] = -1;
	}

	for (int i = 1; i <= m; i++) {
		scanf("%d %d", &graph[i].u, &graph[i].v);
		graph[i].idx = i;
		adj[graph[i].u].deg++;
		adj[graph[i].v].deg++;
	}

	for (int i = 1; i < n + 1; i++) {
		adj[i].adj_v = (int*)malloc(adj[i].deg * sizeof(int));
		adj[i].adj_e = (int*)malloc(adj[i].deg * sizeof(int));
		adj[i].deg = 0; // Сбрасываем, чтобы использовать как счётчик при заполнении
	}

	for (int i = 1; i < m + 1; i++) {
		int u = graph[i].u;
		int v = graph[i].v;
		int idx = graph[i].idx;

		adj[u].adj_v[adj[u].deg] = v;
		adj[u].adj_e[adj[u].deg] = idx;
		adj[u].deg++;

		adj[v].adj_v[adj[v].deg] = u;
		adj[v].adj_e[adj[v].deg] = idx;
		adj[v].deg++;
	}

	for (int i = 1; i < n + 1; i++) if (!visited[i]) dfs(i, n); 

	qsort(bridges, bridge_count, sizeof(int), cmp);

	printf("%d\n", bridge_count);
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
	free(parent_edges);
}
