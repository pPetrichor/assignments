#include<iostream>
using namespace std;
struct mylink
{
	int x;
	mylink *next;
	mylink()
	{
		x = 0;
		next = NULL;
	}
	mylink(int a, mylink* b)
	{
		x = a;
		next = b;
	}
};
struct mynode
{
	int val;//¼ÇÂ¼¶È
	mylink *next;
};
class minheap
{
	int size;
	int *heap;
	int currentsize;
public:
	minheap()
	{
		
	}
	minheap(int n)
	{
		size = n;
		heap = new int[size]; currentsize = 0;
		for (int i = 0; i < size; ++i) heap[i] = -1;
	}
	void siftup(int start)
	{
		int j = start;
		int i = (j - 1) / 2, temp = heap[j];
		while (j > 0)
		{
			if (heap[i] <= temp) break;
			else
			{
				heap[j] = heap[i];
				j = i;
				i = (i - 1) / 2;
			}
			heap[j] = temp;
		}
	}
	void insert(int x)
	{
		heap[currentsize] = x;
		siftup(currentsize);
		currentsize++;
	}
	void siftdown(int start,int end)
	{
		int i = start;
		int j = 2 * i + 1;
		int temp = heap[i];
		while (j <= end)
		{
			if (j<end&&heap[j]>heap[j + 1]) j++;
			if (temp <= heap[j]) break;
			else { heap[i] = heap[j]; i = j; j = 2 * j + 1; }
		}
		heap[i] = temp;
	}
	void remove(int &x)
	{
		x = heap[0];
		heap[0] = heap[currentsize - 1];
		currentsize--;
		siftdown(0, currentsize-1);
	}
	bool if_empty() { return currentsize == 0; }
	
};
int main()
{
	int n, m;
	scanf("%d", &n);
	scanf("%d", &m);
	mynode *mylist = new mynode[n];
	for (int i = 0; i < n; ++i)
	{
		mylist[i].val = 0;
		mylist[i].next = NULL;
	}
	for (int i = 0; i < m; ++i)
	{
		int first, second;
		scanf("%d", &first);
		scanf("%d", &second);
		mylist[second].val++;
		mylink* pa = new mylink(second, mylist[first].next);
		mylist[first].next = pa;
	}
	minheap heap(n);
	for(int i=0;i<n;++i)
	{
		if (mylist[i].val == 0){ heap.insert(i); mylist[i].val--; }
	}
	while(1)
	{
		if (heap.if_empty()) break;
		int t = 0;
		heap.remove(t);
		mylink *current = mylist[t].next;
		while (current != NULL)
		{
			mylist[current->x].val--;
			if(mylist[current->x].val == 0) { heap.insert(current->x); mylist[current->x].val--; }
			current = current->next;
		}
		cout << t << " ";
	}
	system("pause");
	return 0;
}