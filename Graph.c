#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
	int edge_index;
	struct node *next;
}Node;

typedef struct vertex {
	int name;
	Node  *header;
}Vertex;

typedef struct edge {
	int name;
	int endpoint[2];
	int weight_value;
}Edge;

typedef struct graph {
	Vertex *vertex;
	Edge *edge;
}Graph;

Node *getNode() {
	Node *p = NULL;
	p = (Node *)malloc(sizeof(Node));
	p->edge_index = -1;
	p->next = NULL;

	return p;
}

int FindEdge(Graph *g, int a, int b) {
	int i;
	int edge_index = -1;

	for (i = 0; i < 21; i++) {
		if ((g->edge[i].endpoint[0] == a
			&& g->edge[i].endpoint[1] == b)
			|| (g->edge[i].endpoint[0] == b
				&& g->edge[i].endpoint[1] == a)) {
			edge_index = i;
		}
	}
	return edge_index;
}

void addList(Graph *g, int vertex, int endpoint) {
	Node *newNode, *p;
	int edge_index;

	newNode = getNode();
	edge_index = FindEdge(g, vertex, endpoint);
	newNode->edge_index = edge_index;
	p = g->vertex[vertex - 1].header;
	while (p->next != NULL) {
		if (p->next->edge_index > edge_index) {
			break;
		}
		p = p->next;
	}
	newNode->next = p->next;
	p->next = newNode;

	newNode = getNode();
	edge_index = FindEdge(g, vertex, endpoint);
	newNode->edge_index = edge_index;
	p = g->vertex[endpoint - 1].header;
	while (p->next != NULL) {
		if (p->next->edge_index > edge_index) {
			break;
		}
		p = p->next;
	}
	newNode->next = p->next;
	p->next = newNode;
}

void deleteList(Graph *g, int vertex, int endpoint) {
	Node *p, *deletedNode = NULL;
	int edge_index;

	edge_index = FindEdge(g, vertex, endpoint);
	p = g->vertex[vertex - 1].header;
	while (p->next != NULL) {
		if (p->next->edge_index == edge_index) {
			break;
		}
		p = p->next;
	}
	deletedNode = p->next;
	p->next = p->next->next;
	free(deletedNode);

	p = g->vertex[endpoint - 1].header;
	while (p->next != NULL) {
		if (p->next->edge_index == edge_index) {
			break;
		}
		p = p->next;
	}
	deletedNode = p->next;
	p->next = p->next->next;
	free(deletedNode);
}

void ModifyWeightValue(Graph *g, int a, int b, int new_value) {
	Node *p;
	int edge_index, last_value;

	edge_index = FindEdge(g, a, b);
	if (edge_index == -1) {
		printf("-1\n");
		return;
	}
	last_value = g->edge[edge_index].weight_value = new_value;
	g->edge[edge_index].weight_value = new_value;

	if (last_value != 0 && new_value == 0) {
		deleteList(g, a, b);
	}
}

void initializeGraph(Graph *g) {
	int i, j, k;
	Node *p, *tail;
	g->vertex = (Vertex *)malloc(6 * sizeof(Vertex));
	g->edge = (Edge *)malloc((6 + 5 + 4 + 3 + 2 + 1) * sizeof(Edge));

	for (i = 0; i < 6; i++) {
		g->vertex[i].name = i + 1;
		g->vertex[i].header = getNode();
	}

	k = 0;
	for (i = 1; i < 7; i++) {
		for (j = i; j < 7; j++) {
			g->edge[k].endpoint[0] = i;
			g->edge[k].endpoint[1] = j;
			g->edge[k].weight_value = 0;
			k++;
		}
	}
}

void setGraph(Graph *g) {
	ModifyWeightValue(g, 1, 2, 1);
	ModifyWeightValue(g, 1, 3, 1);
	ModifyWeightValue(g, 1, 4, 1);
	ModifyWeightValue(g, 1, 6, 2);
	ModifyWeightValue(g, 2, 3, 1);
	ModifyWeightValue(g, 3, 5, 4);
	ModifyWeightValue(g, 5, 5, 4);
	ModifyWeightValue(g, 5, 6, 3);

	addList(g, 1, 2);
	addList(g, 1, 3);
	addList(g, 1, 4);
	addList(g, 1, 6);
	addList(g, 2, 3);
	addList(g, 3, 5);
	addList(g, 5, 5);
	addList(g, 5, 6);
}

void printNearVertex(Graph *g, int node_num) {
	Node *p;
	int i;
	int flag = 0;

	for (i = 0; i < 21; i++) {
		if (g->edge[i].weight_value != 0) {
			if (g->edge[i].endpoint[1] == node_num) {
				printf(" %d %d", g->edge[i].endpoint[0], g->edge[i].weight_value);
				flag++;
			}
			else if (g->edge[i].endpoint[0] == node_num) {
				printf(" %d %d", g->edge[i].endpoint[1], g->edge[i].weight_value);
				flag++;
			}
		}
	}
	if (flag == 0)
		printf("-1");
}

int main() {
	Graph g;
	char oper;
	int a, b, value;

	initializeGraph(&g);
	setGraph(&g);

	while (1) {
		scanf("%c", &oper);
		if (oper == 'q') break;

		switch (oper) {
		case 'a':
			scanf("%d", &a);
			printNearVertex(&g, a);
			printf("\n");
			break;
		case 'm':
			scanf("%d %d %d", &a, &b, &value);
			ModifyWeightValue(&g, a, b, value);
			break;
		}
	}

	return 0;
}