#define _CRT_SECURE_NO_DEPRECATE
#include<iostream>
#include<vector>
using namespace std;
class minheap
{
	vector<int> heap;
	int currentsize;
public:
	minheap()
	{
		currentsize = 0;
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
		heap.push_back(x);
		siftup(currentsize);
		currentsize++;
	}
	void siftdown(int start, int end)
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
		heap.pop_back();
		siftdown(0, currentsize - 1);
	}
	int get_currentsize() { return currentsize; }
	int get_top() { return heap[0]; }
};
class maxheap
{
	vector<int> heap;
	int currentsize;
public:
	maxheap()
	{
		currentsize = 0;
	}
	
	void siftup(int start)
	{
		int j = start;
		int i = (j - 1) / 2, temp = heap[j];
		while (j > 0)
		{
			if (heap[i] >= temp) break;
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
		heap.push_back(x);
		siftup(currentsize);
		currentsize++;
	}
	void siftdown(int start, int end)
		
	{
		int i = start;
		int j = 2 * i + 1;
		int temp = heap[i];
		while (j <= end)
		{
			if (j < end&&heap[j] < heap[j + 1]) j++;
			if (temp >= heap[j]) break;
			else { heap[i] = heap[j]; i = j; j = 2 * j + 1; }
		}
		heap[i] = temp;
	}
	void remove(int &x)
	{
		x = heap[0];
		heap[0] = heap[currentsize - 1];
		currentsize--;
		heap.pop_back();
		siftdown(0, currentsize - 1);
	}
	int get_currentsize() { return currentsize; }
	int get_top() { return heap[0]; }
};
int main()
{
	minheap mymin;
	maxheap mymax;
	int current = 0;
	scanf("%d",&current);
	mymax.insert(current);
	printf("%d ",current);
	while (scanf("%d", &current)!=EOF)
	{
		if (current < mymax.get_top()) mymax.insert(current);
		else mymin.insert(current);
		int gap = mymax.get_currentsize() - mymin.get_currentsize();
		if (gap > 1)
		{
			int x = 0;
			mymax.remove(x);
			mymin.insert(x);
			printf("%d ", mymax.get_top());
		}
		else if (gap == 1)
			printf("%d ", mymax.get_top());
		else if (gap == 0)
			printf("%d ", mymax.get_top());
		else if (gap == -1)
			printf("%d ", mymin.get_top());
		else
		{
			int x = 0;
			mymin.remove(x);
			mymax.insert(x);
			printf("%d ", mymax.get_top());
		}
	}
	system("pause");
	return 0;
}