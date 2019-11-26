/* Bobitan Denisa-Ioana

   Task:

   Implement correctly and efficiently the base operations for disjoint sets and the Kruskal’s algorithm.

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Profiler.h"

#pragma warning(disable : 4996)

int opMakeSet, opFindSet, opUnite; //global variables to calculate performance
Profiler p("demo");

struct node //node structure with rank, id and parent pointer
{
	int rank;
	int id;
	struct node *parent;
};

struct edge //edge structure with weigth(cost) and pointers to 2 nodes
{
	int cost;
	node *src;
	node *dest;
};

struct graph //graph structure with nb of nodes (v), edges (e) and 2 arrrays (for edges and for nodes pointers)
{
	int v;
	int e;
	edge *edges; //array of edges
	node **nodes; //array of pointers of nodes
};

graph *createGraph(int v, int e)
{
	graph *g = (graph*)malloc(sizeof(graph)); //alloc memory for graph
	g->e = e;
	g->v = v;
	g->edges = (edge*)malloc(e * sizeof(edge)); //alloc memory for edge array - nb of edges is e
	g->nodes = (node**)malloc(v * sizeof(node*)); //alloc memory for nodes pointers - nb of nodes is v
	return g;
}

node *makeSet(int id)
{
	node *x = (node*)malloc(sizeof(node));
	x->id = id; //set id
	opMakeSet++;
	x->parent = x; //set parent as itself
	opMakeSet++;
	x->rank = 0; //set rank 0
	opMakeSet++;
	return x;
}

graph *scanGraph() //to read the graph given as input and populate it
{
	int v, e, cost, src, dest;
	
	FILE *f = fopen("input.txt", "r");
	fscanf(f, "%d %d", &v, &e);
	graph *g = createGraph(v, e);
	for (int i = 0; i < v; i++)
	{
		g->nodes[i] = makeSet(i + 1);
	}
	for (int i = 0; i < e; i++)
	{
		fscanf(f, "%d %d %d", &cost, &src, &dest);
		g->edges[i].cost = cost;
		g->edges[i].src = g->nodes[src - 1];
		g->edges[i].dest = g->nodes[dest - 1];
	}
	fclose(f);
	return g;
}

node *findSet(node *x) //returns the node which represents the set
{
	opFindSet++;
	if (x != x->parent)
	{
		x->parent = findSet(x->parent);
		opFindSet++;
	}
	return x->parent;
}

void link(node *x, node *y) //links 2 nodes - sets the parent and rank increases
{
	opUnite++;
	if (x->rank > y->rank)
	{
		y->parent = x;
		opUnite++;
	}
	else
	{
		x->parent = y;
		opUnite++;
		opUnite++;
		if (x->rank == y->rank)
		{
			y->rank += 1;
			opUnite++;
		}
	}
}

void unite(node *x, node *y) //unites sets, each of them is represented by a node
{
	link(findSet(x), findSet(y));
}

void swapElements(edge *a, int i, int j) //swaps elements from position i and j in array a
{
	edge aux = a[i];
	a[i] = a[j];
	a[j] = aux;
}

int partitionRand(edge *a, int start, int end)
{
	srand(time(NULL));
	int pos = rand() % (end - start) + start;
	edge x = a[pos];
	swapElements(a, pos, end);
	int index = start - 1;
	for (int i = start; i <= end - 1; i++)
	{
		if (a[i].cost <= x.cost)
		{
			index++;
			swapElements(a, i, index);
		}
	}
	swapElements(a, end, index + 1);
	return index + 1;
}

void quickSort(edge *a, int start, int end)
{
	if (start < end)
	{
		int p = partitionRand(a, start, end);
		quickSort(a, start, p - 1);
		quickSort(a, p + 1, end);
	}
}

graph *Kruskal(graph *g, int *k)
{
	graph *a = createGraph(g->v, g->e); //create the graph
	quickSort(g->edges, 0, g->e - 1); //sort the elements in the array of edges increasingly
	for (int i = 0; i < g->v; i++)
	{
		a->nodes[i] = makeSet(g->nodes[i]->id); //create sets, fist as single nodes
	}
	for (int i = 0; i < g->v - 1; i++) //it goes up to the nb of nodes-1 because the result, a tree has n-1 edges
	{
		if (findSet(g->edges[i].src) != findSet(g->edges[i].dest))
		{
			//dynamically alloc memory for the array
			a->edges = (edge*)realloc(a->edges, ((*k) + 1) * sizeof(edge));
			//a->edges[(*k) - 1] = g->edges[i]; //without dynamic allocation
			a->edges[*k] = g->edges[i]; //insert the new value
			(*k)++; //increment index
			unite(findSet(g->edges[i].src), findSet(g->edges[i].dest));
		}
	}
	a->e = *k;
	return a;
}

void displayGraph(graph *g)
{
	for (int i = 0; i < g->v; i++)
	{
		printf("Node: %d Parent: %d\n", g->nodes[i]->id, g->nodes[i]->parent->id);
	}
	printf("\n");
}

void displayEdges(graph *a)
{
	for (int i = 0; i < a->e; i++)
	{
		printf("%d - %d : %d\n", a->edges[i].src->id, a->edges[i].dest->id, a->edges[i].cost);
	}
	printf("\n");
}

void displaySets(node **a, int size)
{
	for (int i = 0; i < size; i++)
	{
		printf("Node %d, set %d\n", a[i]->id, findSet(a[i])->id);
	}
	printf("\n");
}

void demo()
{
	node **arr = (node**)malloc(10 * sizeof(node*));
	for (int i = 0; i < 10; i++)
	{
		arr[i] = makeSet(i + 1);
	}
	printf("Original sets:\n");
	displaySets(arr, 10);
	unite(arr[1], arr[0]);
	unite(arr[2], arr[0]);
	unite(arr[3], arr[0]);
	unite(arr[5], arr[6]);
	unite(arr[7], arr[6]);
	printf("Sets after uniting 1-2, 1-3, 1-4, 6-7, 8-7\n");
	displaySets(arr, 10); //here, findSet is called
	//memory is freed
	for (int i = 0; i < 10; i++)
	{
		free(arr[i]);
	}
	free(arr);

	graph *g1 = scanGraph();
	printf("Original graph:\n");
	displayEdges(g1);
	int size = 0;
	printf("MST:\n");
	graph *a = Kruskal(g1, &size);
	displayEdges(a);
}

boolean isUnique(int *ed, graph *g, int size)
{
	node *src, *dest;
	for (int i = 0; i < size; i++) //for each edge, we have to check whether it is equal to the given edge or not
	{
		src = g->edges[i].src;
		dest = g->edges[i].dest;
		if ((g->nodes[ed[0]] == src && g->nodes[ed[1]] == dest) || (g->nodes[ed[0]] == dest && g->nodes[ed[1]] == src))
		{
			return false;
		}
	}
	return true;
}

void report()
{
	srand(time(NULL));
	int *ed = (int*)malloc(2 * sizeof(int));
	for (int n = 100; n < 10000; n += 100)
	{
		opMakeSet = opFindSet = opUnite = 0;
		printf("%d\n", n);
		graph *g = createGraph(n, 4 * n);
		for (int i = 0; i < n; i++) //n nodes
		{
			g->nodes[i] = makeSet(i + 1);
		}
		int k = 0;
		int *w = (int*)malloc(4 * n * sizeof(int));
		FillRandomArray(w, 4 * n, 1, 10000, false, 0);
		while (k < g->e) //4n edges
		{
			FillRandomArray(ed, 2, 0, n - 1, true, 0);
			if (isUnique(ed, g, k)) //if edge is unique, insert it in the graph's edges list
			{
				g->edges[k].src = g->nodes[ed[0]];
				g->edges[k].dest = g->nodes[ed[1]];
				g->edges[k].cost = w[k];
				k++;
			}
		}
		int size = 0;
		graph *rez = Kruskal(g, &size);
		free(g);
		int totalop = opMakeSet + opFindSet + opUnite;
		p.countOperation("opMakeSet", n, opMakeSet);
		p.countOperation("opFindSet", n, opFindSet);
		p.countOperation("opUnite", n, opUnite);
		p.countOperation("total", n, totalop);
	}
	p.createGroup("Operations", "opMakeSet", "opFindSet", "opUnite");
	p.createGroup("TotalOp", "Total");
	p.showReport();
}

int main()
{
	int i = 1;
	while (i != 0)
	{
		scanf("%d", &i);
		if (i == 1)
		{
			demo();
		}
		else if (i == 2)
		{
			report();
		}
	}
	return 0;
}