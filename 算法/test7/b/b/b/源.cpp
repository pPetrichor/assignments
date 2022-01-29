#define _CRT_SECURE_NO_WARNINGS//Ñ¹ÖÆºê
#include<iostream>
#include<vector>
using namespace std;
struct edge
{
	int next;
	int weight;
};
struct node
{
	int unseen;
	int candidate_edge;
	int position;
	vector<edge> neighbors;
};
struct ele
{
	int node;
	int priority;
};
node* nodes;
class minheap
{
	int size;
	ele *heap;
	int currentsize;
public:
	minheap()
	{

	}
	minheap(int n)
	{
		size = n;
		heap = new ele[size];
		currentsize = 0;
	}
	void siftup(int start)
	{
		int j = start;
		int i = (j - 1) / 2;
		ele temp = heap[j];
		while (j > 0)
		{
			if (heap[i].priority <= temp.priority) break;
			else
			{
				
				nodes[heap[i].node].position = j;
				heap[j] = heap[i];
				j = i;
				i = (i - 1) / 2;
			}
			
		}
		heap[j] = temp;
		nodes[temp.node].position = j;
	}
	void insert(ele x)
	{
		heap[currentsize] = x;
		nodes[x.node].position = currentsize;
		siftup(currentsize);
		currentsize++;
	}
	void siftdown(int start, int end)
	{
		int i = start;
		int j = 2 * i + 1;
		ele temp = heap[i];
		while (j <= end)
		{
			if (j<end&&heap[j].priority>heap[j + 1].priority) j++;
			if (temp.priority <= heap[j].priority) break;
			else
			{
				nodes[heap[j].node].position = i;
				//nodes[heap[i].node].position = j;
				heap[i] = heap[j];
				i = j;
				j = 2 * j + 1;
			}
		}
		heap[i] = temp;
		nodes[temp.node].position = i;
	}
	void remove(ele &x)
	{
		x = heap[0];
		nodes[x.node].position = -1;

		heap[0] = heap[currentsize - 1];
		currentsize--;
		if (currentsize != 0)
		{
			nodes[heap[currentsize - 1].node].position = 0;

			siftdown(0, currentsize - 1);
		}
	}
	bool empty() { return currentsize == 0; }
	void decrease(int x, int w)
	{
		heap[x].priority = w;
		siftup(x);
	}
	ele getx(int pos) { return heap[pos]; }
};
int main()
{
	cout << "I have read the rules about plagiarism punishment" << endl;
	int n, start, end;
	scanf("%d%d%d", &n, &start, &end);
	int weight = 0;
	nodes = new node[n];
	for (int i = 0; i < n; ++i)
	{
		nodes[i].unseen = 0;
		nodes[i].position = -1;
	}
	while (scanf("%d", &weight) != EOF)
	{
		if (weight == -1) break;
		int second, first;
		scanf("%d%d", &first, &second);
		edge temp;
		temp.next = second;
		temp.weight = weight;
		nodes[first].neighbors.push_back(temp);
		temp.next = first;
		nodes[second].neighbors.push_back(temp);
	}
	nodes[start].candidate_edge = -1;
	nodes[start].unseen = 1;
	minheap mst(n);
	ele t;
	t.node = start; t.priority = 0;
	mst.insert(t);
	int max = -1;
	while (!mst.empty())
	{
		ele v;
		mst.remove(v);
		nodes[v.node].unseen = 2;
		if (v.node == end)
		{
			while (nodes[end].candidate_edge != -1)
			{
				int c = 0;
				for (int i = 0; i < nodes[end].neighbors.size(); ++i)
				{
					if (nodes[end].neighbors[i].next == nodes[end].candidate_edge)
					{
						c = nodes[end].neighbors[i].weight;
						break;
					}
				}
				if (max < c) max = c;
				end = nodes[end].candidate_edge;
			}
			break;
		}
		else
		{
			for (int i = 0; i < nodes[v.node].neighbors.size(); ++i)
			{
				int newweight = nodes[v.node].neighbors[i].weight;
				int w = nodes[v.node].neighbors[i].next;
				if (nodes[w].unseen == 0)
				{
					nodes[w].candidate_edge = v.node;
					ele temp;
					temp.node = w; temp.priority = newweight;
					mst.insert(temp);
					nodes[w].unseen = 1;
				}
				else if (nodes[w].unseen == 1)
				{

					if (newweight < (mst.getx(nodes[w].position)).priority)
					{
						nodes[w].candidate_edge = v.node;
						mst.decrease(nodes[w].position, newweight);
					}
				}
			}
		}
	}
	printf("%d", max);
	delete[] nodes;
	system("pause");
	return 0;
}