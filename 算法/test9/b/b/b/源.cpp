#define _CRT_SECURE_NO_WARNINGS//Ñ¹ÖÆºê
#include<iostream>
#include<vector>
#include<queue>
using namespace std;
struct mynode
{
	int val;
	vector<int> neighbors;
	int weight;
};

int main()
{
	int n,m;
	scanf("%d%d",&n,&m);
	int* solve = new int[n];
	for (int i = 0; i < n; ++i)
		solve[i] = 100000;
	mynode* mylist = new mynode[n];
	queue<int> todos;
	int number, weight;
	for (int i = 0; i < n; ++i)
	{
		scanf("%d%d", &number, &weight);
		mylist[number - 1].val = 0;
		mylist[number - 1].weight = weight;
	}
	int first = 0; int second = 0;
	for(int i=0;i<m;++i)
	{
		scanf("%d%d", &first,&second);
		mylist[first-1].val++;
		mylist[second-1].neighbors.push_back(first-1);
	}
	for(int i=0;i<n;++i)
	{
		if (mylist[i].val == 0)
			todos.push(i);
	}
	while (!todos.empty())
	{
		int current = todos.front();
		todos.pop();
		int new_weight = mylist[current].weight;
		if (new_weight < solve[current]) solve[current] = new_weight;
		for (int j = 0; j < mylist[current].neighbors.size(); ++j)
		{
			int c = mylist[current].neighbors[j];
			mylist[c].val--;
			if (mylist[c].val == 0) todos.push(c);	
			if (solve[current] < solve[c]) solve[c] = solve[current];
		}
	}
	for (int i = 0; i < n; ++i)
		printf("%d ", solve[i]);
	delete[] solve;
	system("pause");
	return 0;
}